package com.example.cars.di

import android.content.Context
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.preferencesDataStore
import androidx.room.Room
import com.example.cars.data.local.CarsDatabase
import com.example.cars.data.local.dao.CarDao
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

private val Context.dataStore: DataStore<Preferences> by preferencesDataStore(name = "auth_prefs")

@Module
@InstallIn(SingletonComponent::class)
object AppModule {

    @Provides
    @Singleton
    fun provideDataStore(@ApplicationContext context: Context): DataStore<Preferences> {
        return context.dataStore
    }

    @Provides
    @Singleton
    fun provideCarsDatabase(@ApplicationContext context: Context): CarsDatabase {
        return Room.databaseBuilder(
            context,
            CarsDatabase::class.java,
            CarsDatabase.DATABASE_NAME
        )
            .fallbackToDestructiveMigration() // Drop and recreate on version change
            .addCallback(com.example.cars.data.local.DatabasePrepopulator())
            .build()
    }

    @Provides
    @Singleton
    fun provideCarDao(database: CarsDatabase): CarDao {
        return database.carDao()
    }
}

