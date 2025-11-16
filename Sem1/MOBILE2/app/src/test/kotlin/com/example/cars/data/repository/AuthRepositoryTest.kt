package com.example.cars.data.repository

import com.example.cars.data.local.preferences.AuthPreferencesDataSource
import com.example.cars.data.remote.api.CarsApi
import com.example.cars.data.remote.dto.LoginRequest
import com.example.cars.data.remote.dto.LoginResponse
import com.example.cars.util.Result
import io.mockk.coEvery
import io.mockk.coVerify
import io.mockk.mockk
import kotlinx.coroutines.flow.flowOf
import kotlinx.coroutines.test.runTest
import org.junit.Assert.*
import org.junit.Before
import org.junit.Test

class AuthRepositoryTest {

    private lateinit var repository: AuthRepository
    private lateinit var api: CarsApi
    private lateinit var authPreferences: AuthPreferencesDataSource

    @Before
    fun setup() {
        api = mockk()
        authPreferences = mockk(relaxed = true)
        repository = AuthRepository(api, authPreferences)
    }

    @Test
    fun `login success saves token and returns success`() = runTest {
        // Given
        val email = "test@test.com"
        val password = "password"
        val token = "test_token"
        
        coEvery { api.login(LoginRequest(email, password)) } returns LoginResponse(token)

        // When
        val result = repository.login(email, password)

        // Then
        assertTrue(result is Result.Success)
        assertEquals(token, (result as Result.Success).data)
        coVerify { authPreferences.saveAuthToken(token) }
    }

    @Test
    fun `login failure returns error`() = runTest {
        // Given
        val email = "test@test.com"
        val password = "wrong"
        
        coEvery { api.login(any()) } throws Exception("Unauthorized")

        // When
        val result = repository.login(email, password)

        // Then
        assertTrue(result is Result.Error)
    }

    @Test
    fun `logout clears auth token`() = runTest {
        // When
        repository.logout()

        // Then
        coVerify { authPreferences.clearAuthToken() }
    }

    @Test
    fun `getAuthToken returns flow from preferences`() = runTest {
        // Given
        val token = "test_token"
        coEvery { authPreferences.getAuthToken() } returns flowOf(token)

        // When
        val flow = repository.getAuthToken()

        // Then
        flow.collect { value ->
            assertEquals(token, value)
        }
    }
}

