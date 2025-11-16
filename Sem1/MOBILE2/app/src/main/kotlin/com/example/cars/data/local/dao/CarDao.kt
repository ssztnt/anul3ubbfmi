package com.example.cars.data.local.dao

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query
import androidx.room.Transaction
import androidx.room.Update
import com.example.cars.data.local.entity.CarEntity
import kotlinx.coroutines.flow.Flow

@Dao
interface CarDao {
    @Query("SELECT * FROM cars WHERE isDeleted = 0 ORDER BY updatedAt DESC")
    fun observeAll(): Flow<List<CarEntity>>

    @Query("SELECT * FROM cars WHERE isDeleted = 0 AND id = :id")
    suspend fun getById(id: String): CarEntity?

    @Query("SELECT * FROM cars WHERE isDeleted = 0 AND id = :id")
    fun observeById(id: String): Flow<CarEntity?>

    @Query("SELECT * FROM cars WHERE isPendingSync = 1 AND isDeleted = 0")
    suspend fun getPendingSync(): List<CarEntity>

    @Query("SELECT * FROM cars WHERE isDeleted = 1")
    suspend fun getDeleted(): List<CarEntity>

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(car: CarEntity)

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertAll(cars: List<CarEntity>)

    @Update
    suspend fun update(car: CarEntity)

    @Query("UPDATE cars SET isPendingSync = 0 WHERE id = :id")
    suspend fun clearPendingSync(id: String)

    @Query("UPDATE cars SET isDeleted = 1, isPendingSync = 1 WHERE id = :id")
    suspend fun markDeleted(id: String)

    @Query("DELETE FROM cars WHERE id = :id")
    suspend fun delete(id: String)

    @Query("DELETE FROM cars")
    suspend fun deleteAll()

    @Query("SELECT MAX(updatedAt) FROM cars")
    suspend fun getLastUpdatedTimestamp(): Long?

    @Transaction
    suspend fun upsertCar(car: CarEntity) {
        val existing = getById(car.id)
        if (existing == null) {
            insert(car)
        } else {
            // Last-write-wins based on updatedAt
            if (car.updatedAt >= existing.updatedAt) {
                insert(car)
            }
        }
    }
}

