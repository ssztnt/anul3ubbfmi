package com.example.cars.data.repository

import com.example.cars.data.local.dao.CarDao
import com.example.cars.data.remote.api.CarsApi
import com.example.cars.domain.mapper.toDomain
import com.example.cars.domain.mapper.toDto
import com.example.cars.domain.mapper.toEntity
import com.example.cars.domain.model.Car
import com.example.cars.util.Result
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.map
import java.time.Instant
import java.util.UUID
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class CarsRepository @Inject constructor(
    private val carDao: CarDao,
    private val api: CarsApi
) {
    fun observeCars(): Flow<List<Car>> {
        return carDao.observeAll().map { entities ->
            entities.map { it.toDomain() }
        }
    }

    fun observeCar(id: String): Flow<Car?> {
        return carDao.observeById(id).map { it?.toDomain() }
    }

    suspend fun getCar(id: String): Car? {
        return carDao.getById(id)?.toDomain()
    }

    suspend fun saveCar(car: Car): Result<Car> {
        return try {
            val carToSave = if (car.id.isEmpty()) {
                car.copy(
                    id = UUID.randomUUID().toString(),
                    updatedAt = Instant.now()
                )
            } else {
                car.copy(updatedAt = Instant.now())
            }

            // Save locally with pending sync flag
            carDao.insert(carToSave.toEntity().copy(isPendingSync = true))
            Result.Success(carToSave)
        } catch (e: Exception) {
            Result.Error(e)
        }
    }

    suspend fun deleteCar(id: String): Result<Unit> {
        return try {
            carDao.markDeleted(id)
            Result.Success(Unit)
        } catch (e: Exception) {
            Result.Error(e)
        }
    }

    suspend fun syncCars(): Result<Unit> {
        return try {
            // Get last sync timestamp
            val lastUpdated = carDao.getLastUpdatedTimestamp()?.let {
                Instant.ofEpochMilli(it).toString()
            }

            // Fetch delta from server
            val response = api.getCars(since = lastUpdated)
            
            // Upsert received cars (conflict resolution by updatedAt)
            response.cars.forEach { dto ->
                carDao.upsertCar(dto.toEntity())
            }

            // Push pending changes
            val pendingCars = carDao.getPendingSync()
            pendingCars.forEach { entity ->
                val car = entity.toDomain()
                try {
                    if (entity.id.contains("-")) { // UUID = new car
                        api.createCar(car.toDto())
                    } else {
                        api.updateCar(entity.id, car.toDto())
                    }
                    carDao.clearPendingSync(entity.id)
                } catch (e: Exception) {
                    // Keep pending flag on error
                }
            }

            // Push deletions
            val deletedCars = carDao.getDeleted()
            deletedCars.forEach { entity ->
                try {
                    api.deleteCar(entity.id)
                    carDao.delete(entity.id)
                } catch (e: Exception) {
                    // Keep marked as deleted on error
                }
            }

            Result.Success(Unit)
        } catch (e: Exception) {
            Result.Error(e)
        }
    }

    suspend fun refreshCars(): Result<Unit> {
        return try {
            val response = api.getCars()
            carDao.insertAll(response.cars.map { it.toEntity() })
            Result.Success(Unit)
        } catch (e: Exception) {
            Result.Error(e)
        }
    }
}

