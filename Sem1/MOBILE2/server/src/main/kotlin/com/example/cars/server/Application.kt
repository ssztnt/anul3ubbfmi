package com.example.cars.server

import io.ktor.http.*
import io.ktor.serialization.kotlinx.json.*
import io.ktor.server.application.*
import io.ktor.server.engine.*
import io.ktor.server.netty.*
import io.ktor.server.plugins.contentnegotiation.*
import io.ktor.server.plugins.cors.routing.*
import io.ktor.server.plugins.defaultheaders.*
import io.ktor.server.request.*
import io.ktor.server.response.*
import io.ktor.server.routing.*
import kotlinx.datetime.Clock
import kotlinx.datetime.Instant
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json
import java.util.*
import java.util.concurrent.ConcurrentHashMap
import kotlin.random.Random

@Serializable
data class CarDto(
    val id: String,
    val make: String,
    val model: String,
    val year: Int,
    val price: Double,
    val mileage: Int,
    val fuelType: String,
    val transmission: String,
    val imageUrl: String? = null,
    val description: String,
    val updatedAt: String,
    val lat: Double? = null,
    val lng: Double? = null
)

@Serializable
data class LoginRequest(
    val email: String,
    val password: String
)

@Serializable
data class LoginResponse(
    val token: String
)

@Serializable
data class CarsResponse(
    val cars: List<CarDto>,
    val page: Int,
    val pageSize: Int,
    val total: Int
)

@Serializable
data class ErrorResponse(
    val error: String
)

// In-memory storage
val carsDb = ConcurrentHashMap<String, CarDto>()

fun main() {
    // Initialize with sample data
    initializeSampleData()

    embeddedServer(Netty, port = 8080, host = "0.0.0.0") {
        configureServer()
    }.start(wait = true)
}

fun initializeSampleData() {
    val sampleCars = listOf(
        CarDto(
            id = "1",
            make = "Tesla",
            model = "Model 3 Long Range AWD",
            year = 2022,
            price = 38999.0,
            mileage = 24500,
            fuelType = "Electric",
            transmission = "Automatic",
            imageUrl = "https://images.unsplash.com/photo-1560958089-b8a1929cea89?w=400",
            description = "Clean, one owner. Full self-driving package included.",
            updatedAt = Clock.System.now().toString(),
            lat = 44.4268,
            lng = 26.1025
        ),
        CarDto(
            id = "2",
            make = "BMW",
            model = "M5 Competition",
            year = 2023,
            price = 105000.0,
            mileage = 5000,
            fuelType = "Gasoline",
            transmission = "Automatic",
            imageUrl = "https://images.unsplash.com/photo-1555215695-3004980ad54e?w=400",
            description = "Like new, barely driven. Pristine condition.",
            updatedAt = Clock.System.now().toString(),
            lat = 44.4320,
            lng = 26.1040
        ),
        CarDto(
            id = "3",
            make = "Mercedes-Benz",
            model = "AMG GT 63 S 4MATIC+",
            year = 2021,
            price = 135000.0,
            mileage = 12000,
            fuelType = "Gasoline",
            transmission = "Automatic",
            imageUrl = "https://images.unsplash.com/photo-1618843479313-40f8afb4b4d8?w=400",
            description = "Stunning performance sedan with luxury appointments.",
            updatedAt = Clock.System.now().toString(),
            lat = 44.4400,
            lng = 26.0900
        ),
        CarDto(
            id = "4",
            make = "Porsche",
            model = "911 Carrera S",
            year = 2023,
            price = 125000.0,
            mileage = 3000,
            fuelType = "Gasoline",
            transmission = "Manual",
            imageUrl = "https://images.unsplash.com/photo-1503376780353-7e6692767b70?w=400",
            description = "Manual transmission, sport chrono package.",
            updatedAt = Clock.System.now().toString(),
            lat = 44.4150,
            lng = 26.1200
        ),
        CarDto(
            id = "5",
            make = "Audi",
            model = "RS6 Avant",
            year = 2022,
            price = 115000.0,
            mileage = 8000,
            fuelType = "Gasoline",
            transmission = "Automatic",
            imageUrl = "https://images.unsplash.com/photo-1606664515524-ed2f786a0bd6?w=400",
            description = "The ultimate performance wagon. Carbon ceramic brakes.",
            updatedAt = Clock.System.now().toString(),
            lat = 44.4500,
            lng = 26.1100
        )
    )

    sampleCars.forEach { car ->
        carsDb[car.id] = car
    }
}

fun Application.configureServer() {
    install(ContentNegotiation) {
        json(Json {
            prettyPrint = true
            isLenient = true
            ignoreUnknownKeys = true
        })
    }

    install(CORS) {
        anyHost()
        allowHeader(HttpHeaders.ContentType)
        allowHeader(HttpHeaders.Authorization)
        allowMethod(HttpMethod.Get)
        allowMethod(HttpMethod.Post)
        allowMethod(HttpMethod.Put)
        allowMethod(HttpMethod.Delete)
        allowMethod(HttpMethod.Options)
    }

    install(DefaultHeaders) {
        header("X-Engine", "Ktor")
    }

    routing {
        // Health check
        get("/") {
            call.respondText("Cars Mock Server is running!")
        }

        // Auth
        post("/auth/login") {
            val request = call.receive<LoginRequest>()
            
            // Simulate occasional 401 for testing
            if (Random.nextInt(10) == 0) {
                call.respond(HttpStatusCode.Unauthorized, ErrorResponse("Invalid credentials"))
                return@post
            }
            
            if (request.email == "demo@demo.com" && request.password == "demo1234") {
                call.respond(LoginResponse(token = "mock_jwt_token_${UUID.randomUUID()}"))
            } else {
                call.respond(HttpStatusCode.Unauthorized, ErrorResponse("Invalid credentials"))
            }
        }

        // Cars endpoints (require auth header)
        route("/cars") {
            // Middleware to check auth
            intercept(ApplicationCallPipeline.Call) {
                val authHeader = call.request.header(HttpHeaders.Authorization)
                if (authHeader == null || !authHeader.startsWith("Bearer ")) {
                    call.respond(HttpStatusCode.Unauthorized, ErrorResponse("Missing or invalid token"))
                    finish()
                }
            }

            get {
                val page = call.request.queryParameters["page"]?.toIntOrNull() ?: 1
                val pageSize = call.request.queryParameters["pageSize"]?.toIntOrNull() ?: 20
                val since = call.request.queryParameters["since"]

                var cars = carsDb.values.toList()

                // Delta sync
                if (since != null) {
                    try {
                        val sinceInstant = Instant.parse(since)
                        cars = cars.filter {
                            Instant.parse(it.updatedAt) > sinceInstant
                        }
                    } catch (e: Exception) {
                        // Invalid date format, return all
                    }
                }

                // Pagination
                val startIndex = (page - 1) * pageSize
                val paginatedCars = cars.drop(startIndex).take(pageSize)

                call.respond(
                    CarsResponse(
                        cars = paginatedCars,
                        page = page,
                        pageSize = pageSize,
                        total = cars.size
                    )
                )
            }

            get("/{id}") {
                val id = call.parameters["id"]!!
                val car = carsDb[id]
                
                if (car != null) {
                    call.respond(car)
                } else {
                    call.respond(HttpStatusCode.NotFound, ErrorResponse("Car not found"))
                }
            }

            post {
                val car = call.receive<CarDto>()
                val newCar = car.copy(
                    id = UUID.randomUUID().toString(),
                    updatedAt = Clock.System.now().toString()
                )
                carsDb[newCar.id] = newCar
                call.respond(HttpStatusCode.Created, newCar)
            }

            put("/{id}") {
                val id = call.parameters["id"]!!
                val car = call.receive<CarDto>()
                
                if (carsDb.containsKey(id)) {
                    val updatedCar = car.copy(
                        id = id,
                        updatedAt = Clock.System.now().toString()
                    )
                    carsDb[id] = updatedCar
                    call.respond(updatedCar)
                } else {
                    call.respond(HttpStatusCode.NotFound, ErrorResponse("Car not found"))
                }
            }

            delete("/{id}") {
                val id = call.parameters["id"]!!
                
                if (carsDb.remove(id) != null) {
                    call.respond(HttpStatusCode.NoContent)
                } else {
                    call.respond(HttpStatusCode.NotFound, ErrorResponse("Car not found"))
                }
            }
        }
    }
}

