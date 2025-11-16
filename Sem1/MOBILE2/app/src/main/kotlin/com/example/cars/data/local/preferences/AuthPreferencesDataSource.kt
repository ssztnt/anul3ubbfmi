package com.example.cars.data.local.preferences

import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.core.booleanPreferencesKey
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.stringPreferencesKey
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.map
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class AuthPreferencesDataSource @Inject constructor(
    private val dataStore: DataStore<Preferences>
) {
    private object PreferencesKeys {
        val AUTH_TOKEN = stringPreferencesKey("auth_token")
        val USE_CAMERA_MODE = booleanPreferencesKey("use_camera_mode")
    }

    fun getAuthToken(): Flow<String?> = dataStore.data.map { preferences ->
        preferences[PreferencesKeys.AUTH_TOKEN]
    }

    suspend fun saveAuthToken(token: String) {
        dataStore.edit { preferences ->
            preferences[PreferencesKeys.AUTH_TOKEN] = token
        }
    }

    suspend fun clearAuthToken() {
        dataStore.edit { preferences ->
            preferences.remove(PreferencesKeys.AUTH_TOKEN)
        }
    }

    fun isUseCameraMode(): Flow<Boolean> = dataStore.data.map { preferences ->
        preferences[PreferencesKeys.USE_CAMERA_MODE] ?: false
    }

    suspend fun setUseCameraMode(enabled: Boolean) {
        dataStore.edit { preferences ->
            preferences[PreferencesKeys.USE_CAMERA_MODE] = enabled
        }
    }
}

