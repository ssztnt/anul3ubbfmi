package com.example.cars.data.repository

import com.example.cars.data.local.preferences.AuthPreferencesDataSource
import com.example.cars.data.remote.api.CarsApi
import com.example.cars.data.remote.dto.LoginRequest
import com.example.cars.util.Result
import kotlinx.coroutines.flow.Flow
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class AuthRepository @Inject constructor(
    private val api: CarsApi,
    private val authPreferences: AuthPreferencesDataSource
) {
    fun getAuthToken(): Flow<String?> = authPreferences.getAuthToken()

    suspend fun login(email: String, password: String): Result<String> {
        return try {
            val response = api.login(LoginRequest(email, password))
            authPreferences.saveAuthToken(response.token)
            Result.Success(response.token)
        } catch (e: Exception) {
            Result.Error(e)
        }
    }

    suspend fun logout() {
        authPreferences.clearAuthToken()
    }

    fun isUseCameraMode(): Flow<Boolean> = authPreferences.isUseCameraMode()

    suspend fun setUseCameraMode(enabled: Boolean) {
        authPreferences.setUseCameraMode(enabled)
    }
}

