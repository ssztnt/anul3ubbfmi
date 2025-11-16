package com.example.cars.data.local.entity

import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "cars")
data class CarEntity(
    @PrimaryKey
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
    val updatedAt: Long, // Unix timestamp in milliseconds
    val lat: Double?,
    val lng: Double?,
    val isPendingSync: Boolean = false,
    val isDeleted: Boolean = false
)

