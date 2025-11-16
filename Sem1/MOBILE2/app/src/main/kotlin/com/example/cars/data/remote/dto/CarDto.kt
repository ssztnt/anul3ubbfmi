package com.example.cars.data.remote.dto

import kotlinx.serialization.Serializable

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
    val updatedAt: String, // ISO 8601 timestamp
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

