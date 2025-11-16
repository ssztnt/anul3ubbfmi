package com.example.cars.domain.mapper

import com.example.cars.data.local.entity.CarEntity
import com.example.cars.data.remote.dto.CarDto
import com.example.cars.domain.model.Car
import java.time.Instant

fun CarEntity.toDomain(): Car {
    return Car(
        id = id,
        make = make,
        model = model,
        year = year,
        price = price,
        mileage = mileage,
        fuelType = fuelType,
        transmission = transmission,
        imageUrl = imageUrl,
        description = description,
        updatedAt = Instant.ofEpochMilli(updatedAt),
        lat = lat,
        lng = lng,
        isPendingSync = isPendingSync
    )
}

fun Car.toEntity(): CarEntity {
    return CarEntity(
        id = id,
        make = make,
        model = model,
        year = year,
        price = price,
        mileage = mileage,
        fuelType = fuelType,
        transmission = transmission,
        imageUrl = imageUrl,
        description = description,
        updatedAt = updatedAt.toEpochMilli(),
        lat = lat,
        lng = lng,
        isPendingSync = isPendingSync
    )
}

fun CarDto.toEntity(): CarEntity {
    return CarEntity(
        id = id,
        make = make,
        model = model,
        year = year,
        price = price,
        mileage = mileage,
        fuelType = fuelType,
        transmission = transmission,
        imageUrl = imageUrl,
        description = description,
        updatedAt = Instant.parse(updatedAt).toEpochMilli(),
        lat = lat,
        lng = lng,
        isPendingSync = false,
        isDeleted = false
    )
}

fun Car.toDto(): CarDto {
    return CarDto(
        id = id,
        make = make,
        model = model,
        year = year,
        price = price,
        mileage = mileage,
        fuelType = fuelType,
        transmission = transmission,
        imageUrl = imageUrl,
        description = description,
        updatedAt = updatedAt.toString(),
        lat = lat,
        lng = lng
    )
}

fun CarDto.toDomain(): Car {
    return Car(
        id = id,
        make = make,
        model = model,
        year = year,
        price = price,
        mileage = mileage,
        fuelType = fuelType,
        transmission = transmission,
        imageUrl = imageUrl,
        description = description,
        updatedAt = Instant.parse(updatedAt),
        lat = lat,
        lng = lng,
        isPendingSync = false
    )
}

