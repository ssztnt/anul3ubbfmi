package com.example.cars.domain.model

import java.time.Instant

data class Car(
    val id: String,
    val make: String,
    val model: String,
    val year: Int,
    val price: Double,
    val mileage: Int,
    val fuelType: String,
    val transmission: String,
    val imageUrl: String?,
    val description: String,
    val updatedAt: Instant,
    val lat: Double?,
    val lng: Double?,
    val isPendingSync: Boolean = false
)

