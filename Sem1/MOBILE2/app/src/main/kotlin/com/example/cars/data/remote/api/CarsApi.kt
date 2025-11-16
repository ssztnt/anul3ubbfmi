package com.example.cars.data.remote.api

import com.example.cars.data.remote.dto.CarDto
import com.example.cars.data.remote.dto.CarsResponse
import com.example.cars.data.remote.dto.LoginRequest
import com.example.cars.data.remote.dto.LoginResponse
import retrofit2.http.Body
import retrofit2.http.DELETE
import retrofit2.http.GET
import retrofit2.http.POST
import retrofit2.http.PUT
import retrofit2.http.Path
import retrofit2.http.Query

interface CarsApi {
    @POST("auth/login")
    suspend fun login(@Body request: LoginRequest): LoginResponse

    @GET("cars")
    suspend fun getCars(
        @Query("page") page: Int = 1,
        @Query("pageSize") pageSize: Int = 20,
        @Query("since") since: String? = null
    ): CarsResponse

    @GET("cars/{id}")
    suspend fun getCar(@Path("id") id: String): CarDto

    @POST("cars")
    suspend fun createCar(@Body car: CarDto): CarDto

    @PUT("cars/{id}")
    suspend fun updateCar(@Path("id") id: String, @Body car: CarDto): CarDto

    @DELETE("cars/{id}")
    suspend fun deleteCar(@Path("id") id: String)
}

