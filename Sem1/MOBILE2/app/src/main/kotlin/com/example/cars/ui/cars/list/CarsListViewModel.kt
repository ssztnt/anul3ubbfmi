package com.example.cars.ui.cars.list

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import androidx.work.OneTimeWorkRequestBuilder
import androidx.work.WorkManager
import com.example.cars.data.repository.AuthRepository
import com.example.cars.data.repository.CarsRepository
import com.example.cars.domain.model.Car
import com.example.cars.util.NetworkMonitor
import com.example.cars.util.SensorMonitor
import com.example.cars.workers.SyncWorker
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.combine
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.launch
import javax.inject.Inject

data class CarsListUiState(
    val cars: List<Car> = emptyList(),
    val filteredCars: List<Car> = emptyList(),
    val searchQuery: String = "",
    val isOnline: Boolean = false,
    val motionState: SensorMonitor.MotionState = SensorMonitor.MotionState.STABLE,
    val lightLevel: SensorMonitor.LightLevel = SensorMonitor.LightLevel.NORMAL,
    val deviceOrientation: SensorMonitor.DeviceOrientation = SensorMonitor.DeviceOrientation.PORTRAIT,
    val isLoading: Boolean = false,
    val error: String? = null
)

@HiltViewModel
class CarsListViewModel @Inject constructor(
    private val carsRepository: CarsRepository,
    private val authRepository: AuthRepository,
    private val networkMonitor: NetworkMonitor,
    private val sensorMonitor: SensorMonitor,
    private val workManager: WorkManager
) : ViewModel() {

    private val _searchQuery = MutableStateFlow("")
    val searchQuery: StateFlow<String> = _searchQuery.asStateFlow()

    val uiState: StateFlow<CarsListUiState> = combine(
        carsRepository.observeCars(),
        _searchQuery,
        networkMonitor.isOnline,
        sensorMonitor.motionState,
        sensorMonitor.lightLevel
    ) { cars, query, isOnline, motion, light ->
        Triple(cars, query, Triple(isOnline, motion, light))
    }.combine(sensorMonitor.deviceOrientation) { (cars, query, networkAndSensors), orientation ->
        val (isOnline, motion, light) = networkAndSensors
        
        val filtered = if (query.isBlank()) {
            cars
        } else {
            cars.filter {
                it.make.contains(query, ignoreCase = true) ||
                        it.model.contains(query, ignoreCase = true)
            }
        }

        CarsListUiState(
            cars = cars,
            filteredCars = filtered,
            searchQuery = query,
            isOnline = isOnline,
            motionState = motion,
            lightLevel = light,
            deviceOrientation = orientation
        )
    }.stateIn(
        scope = viewModelScope,
        started = SharingStarted.WhileSubscribed(5000),
        initialValue = CarsListUiState()
    )

    fun onSearchQueryChange(query: String) {
        _searchQuery.value = query
    }

    fun syncNow() {
        val syncRequest = OneTimeWorkRequestBuilder<SyncWorker>().build()
        workManager.enqueue(syncRequest)
    }

    fun logout() {
        viewModelScope.launch {
            authRepository.logout()
        }
    }
}

