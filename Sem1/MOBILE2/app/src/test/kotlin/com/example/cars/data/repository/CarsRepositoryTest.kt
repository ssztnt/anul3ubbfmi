package com.example.cars.data.repository

import com.example.cars.data.local.dao.CarDao
import com.example.cars.data.local.entity.CarEntity
import com.example.cars.data.remote.api.CarsApi
import com.example.cars.data.remote.dto.CarDto
import com.example.cars.data.remote.dto.CarsResponse
import com.example.cars.util.Result
import io.mockk.coEvery
import io.mockk.coVerify
import io.mockk.mockk
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.flow.flowOf
import kotlinx.coroutines.test.runTest
import org.junit.Assert.*
import org.junit.Before
import org.junit.Test
import java.time.Instant

class CarsRepositoryTest {

    private lateinit var repository: CarsRepository
    private lateinit var carDao: CarDao
    private lateinit var api: CarsApi

    @Before
    fun setup() {
        carDao = mockk(relaxed = true)
        api = mockk()
        repository = CarsRepository(carDao, api)
    }

    @Test
    fun `observeCars returns flow of cars from dao`() = runTest {
        // Given
        val entities = listOf(
            createCarEntity("1", "Tesla", "Model 3"),
            createCarEntity("2", "BMW", "M5")
        )
        coEvery { carDao.observeAll() } returns flowOf(entities)

        // When
        val cars = repository.observeCars().first()

        // Then
        assertEquals(2, cars.size)
        assertEquals("Tesla", cars[0].make)
        assertEquals("BMW", cars[1].make)
    }

    @Test
    fun `saveCar inserts with pending sync flag`() = runTest {
        // Given
        val car = createCar("1", "Tesla", "Model 3")

        // When
        val result = repository.saveCar(car)

        // Then
        assertTrue(result is Result.Success)
        coVerify { carDao.insert(match { it.isPendingSync }) }
    }

    @Test
    fun `syncCars fetches and upserts remote cars`() = runTest {
        // Given
        val remoteCars = listOf(
            createCarDto("1", "Tesla", "Model 3")
        )
        coEvery { carDao.getLastUpdatedTimestamp() } returns null
        coEvery { api.getCars(since = null) } returns CarsResponse(
            cars = remoteCars,
            page = 1,
            pageSize = 20,
            total = 1
        )
        coEvery { carDao.getPendingSync() } returns emptyList()
        coEvery { carDao.getDeleted() } returns emptyList()

        // When
        val result = repository.syncCars()

        // Then
        assertTrue(result is Result.Success)
        coVerify { carDao.upsertCar(any()) }
    }

    @Test
    fun `deleteCar marks car as deleted`() = runTest {
        // When
        val result = repository.deleteCar("1")

        // Then
        assertTrue(result is Result.Success)
        coVerify { carDao.markDeleted("1") }
    }

    private fun createCarEntity(
        id: String,
        make: String,
        model: String
    ) = CarEntity(
        id = id,
        make = make,
        model = model,
        year = 2022,
        price = 50000.0,
        mileage = 10000,
        fuelType = "Electric",
        transmission = "Automatic",
        imageUrl = null,
        description = "Test car",
        updatedAt = Instant.now().toEpochMilli(),
        lat = null,
        lng = null
    )

    private fun createCar(
        id: String,
        make: String,
        model: String
    ) = com.example.cars.domain.model.Car(
        id = id,
        make = make,
        model = model,
        year = 2022,
        price = 50000.0,
        mileage = 10000,
        fuelType = "Electric",
        transmission = "Automatic",
        imageUrl = null,
        description = "Test car",
        updatedAt = Instant.now(),
        lat = null,
        lng = null
    )

    private fun createCarDto(
        id: String,
        make: String,
        model: String
    ) = CarDto(
        id = id,
        make = make,
        model = model,
        year = 2022,
        price = 50000.0,
        mileage = 10000,
        fuelType = "Electric",
        transmission = "Automatic",
        imageUrl = null,
        description = "Test car",
        updatedAt = Instant.now().toString(),
        lat = null,
        lng = null
    )
}

