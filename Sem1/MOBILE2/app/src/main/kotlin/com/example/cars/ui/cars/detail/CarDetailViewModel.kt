package com.example.cars.ui.cars.detail

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.cars.data.repository.AuthRepository
import com.example.cars.data.repository.CarsRepository
import com.example.cars.domain.model.Car
import com.example.cars.util.Result
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.launch
import java.time.Instant
import javax.inject.Inject

data class CarDetailUiState(
    val car: Car? = null,
    val make: String = "",
    val model: String = "",
    val year: String = "",
    val price: String = "",
    val mileage: String = "",
    val fuelType: String = "Gasoline",
    val transmission: String = "Automatic",
    val imageUrl: String = "",
    val description: String = "",
    val lat: Double? = null,
    val lng: Double? = null,
    val isLoading: Boolean = false,
    val isSaving: Boolean = false,
    val error: String? = null,
    val isUseCameraMode: Boolean = false
)

@HiltViewModel
class CarDetailViewModel @Inject constructor(
    private val carsRepository: CarsRepository,
    private val authRepository: AuthRepository
) : ViewModel() {

    private val _uiState = MutableStateFlow(CarDetailUiState())
    val uiState: StateFlow<CarDetailUiState> = _uiState.asStateFlow()

    val isUseCameraMode = authRepository.isUseCameraMode()
        .stateIn(
            scope = viewModelScope,
            started = SharingStarted.WhileSubscribed(5000),
            initialValue = false
        )

    fun loadCar(carId: String?) {
        if (carId == null) {
            // New car - leave empty
            return
        }

        viewModelScope.launch {
            _uiState.value = _uiState.value.copy(isLoading = true)
            
            carsRepository.observeCar(carId).collect { car ->
                if (car != null) {
                    _uiState.value = CarDetailUiState(
                        car = car,
                        make = car.make,
                        model = car.model,
                        year = car.year.toString(),
                        price = car.price.toString(),
                        mileage = car.mileage.toString(),
                        fuelType = car.fuelType,
                        transmission = car.transmission,
                        imageUrl = car.imageUrl ?: "",
                        description = car.description,
                        lat = car.lat,
                        lng = car.lng,
                        isLoading = false
                    )
                } else {
                    _uiState.value = _uiState.value.copy(isLoading = false)
                }
            }
        }
    }

    fun onMakeChange(value: String) {
        _uiState.value = _uiState.value.copy(make = value)
    }

    fun onModelChange(value: String) {
        _uiState.value = _uiState.value.copy(model = value)
    }

    fun onYearChange(value: String) {
        _uiState.value = _uiState.value.copy(year = value)
    }

    fun onPriceChange(value: String) {
        _uiState.value = _uiState.value.copy(price = value)
    }

    fun onMileageChange(value: String) {
        _uiState.value = _uiState.value.copy(mileage = value)
    }

    fun onFuelTypeChange(value: String) {
        _uiState.value = _uiState.value.copy(fuelType = value)
    }

    fun onTransmissionChange(value: String) {
        _uiState.value = _uiState.value.copy(transmission = value)
    }

    fun onImageUrlChange(value: String) {
        _uiState.value = _uiState.value.copy(imageUrl = value)
    }

    fun onDescriptionChange(value: String) {
        _uiState.value = _uiState.value.copy(description = value)
    }

    fun onLocationChange(lat: Double, lng: Double) {
        _uiState.value = _uiState.value.copy(lat = lat, lng = lng)
    }

    fun saveCar(onSuccess: () -> Unit) {
        val state = _uiState.value

        // Validation
        if (state.make.isBlank() || state.model.isBlank()) {
            _uiState.value = state.copy(error = "Make and model are required")
            return
        }

        val year = state.year.toIntOrNull()
        if (year == null || year < 1900 || year > 2100) {
            _uiState.value = state.copy(error = "Invalid year")
            return
        }

        val price = state.price.toDoubleOrNull()
        if (price == null || price < 0) {
            _uiState.value = state.copy(error = "Invalid price")
            return
        }

        val mileage = state.mileage.toIntOrNull()
        if (mileage == null || mileage < 0) {
            _uiState.value = state.copy(error = "Invalid mileage")
            return
        }

        viewModelScope.launch {
            _uiState.value = state.copy(isSaving = true, error = null)

            val car = Car(
                id = state.car?.id ?: "",
                make = state.make,
                model = state.model,
                year = year,
                price = price,
                mileage = mileage,
                fuelType = state.fuelType,
                transmission = state.transmission,
                imageUrl = state.imageUrl.ifBlank { null },
                description = state.description,
                updatedAt = Instant.now(),
                lat = state.lat,
                lng = state.lng
            )

            when (carsRepository.saveCar(car)) {
                is Result.Success -> {
                    _uiState.value = state.copy(isSaving = false)
                    onSuccess()
                }
                is Result.Error -> {
                    _uiState.value = state.copy(
                        isSaving = false,
                        error = "Failed to save car"
                    )
                }
            }
        }
    }

    fun deleteCar(onSuccess: () -> Unit) {
        val carId = _uiState.value.car?.id ?: return

        viewModelScope.launch {
            _uiState.value = _uiState.value.copy(isSaving = true)
            
            when (carsRepository.deleteCar(carId)) {
                is Result.Success -> {
                    _uiState.value = _uiState.value.copy(isSaving = false)
                    onSuccess()
                }
                is Result.Error -> {
                    _uiState.value = _uiState.value.copy(
                        isSaving = false,
                        error = "Failed to delete car"
                    )
                }
            }
        }
    }
}

