package com.example.cars.data.local

import androidx.room.Database
import androidx.room.RoomDatabase
import com.example.cars.data.local.dao.CarDao
import com.example.cars.data.local.entity.CarEntity

@Database(
    entities = [CarEntity::class],
    version = 2,
    exportSchema = false
)
abstract class CarsDatabase : RoomDatabase() {
    abstract fun carDao(): CarDao

    companion object {
        const val DATABASE_NAME = "cars_db"
    }
}

