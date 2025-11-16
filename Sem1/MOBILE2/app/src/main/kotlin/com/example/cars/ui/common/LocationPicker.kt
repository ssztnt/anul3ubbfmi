package com.example.cars.ui.common

import androidx.compose.animation.core.animateFloatAsState
import androidx.compose.animation.core.tween
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import com.example.cars.R
import com.google.android.gms.maps.model.CameraPosition
import com.google.android.gms.maps.model.LatLng
import com.google.maps.android.compose.GoogleMap
import com.google.maps.android.compose.Marker
import com.google.maps.android.compose.MarkerState
import com.google.maps.android.compose.rememberCameraPositionState
import kotlin.random.Random

@Composable
fun LocationPicker(
    initialLat: Double?,
    initialLng: Double?,
    onLocationSelected: (Double, Double) -> Unit,
    modifier: Modifier = Modifier
) {
    // Default location: Bucharest, Romania
    val defaultLocation = LatLng(44.4268, 26.1025)
    
    // Generate random location around Bucharest if no initial location
    var markerPosition by remember {
        mutableStateOf(
            if (initialLat != null && initialLng != null) {
                LatLng(initialLat, initialLng)
            } else {
                // Random location around Bucharest (within ~5km radius)
                val randomLat = 44.4268 + (Random.nextDouble(-0.05, 0.05))
                val randomLng = 26.1025 + (Random.nextDouble(-0.05, 0.05))
                LatLng(randomLat, randomLng)
            }
        )
    }

    val cameraPositionState = rememberCameraPositionState {
        position = CameraPosition.fromLatLngZoom(markerPosition, 12f)
    }

    // Marker drop animation
    val markerAlpha by animateFloatAsState(
        targetValue = 1f,
        animationSpec = tween(durationMillis = 500),
        label = "marker_alpha"
    )

    Column(modifier = modifier) {
        Text(
            text = stringResource(R.string.location),
            style = MaterialTheme.typography.titleMedium
        )

        Spacer(modifier = Modifier.height(8.dp))
        
        Text(
            text = "Tap on the map to set car location",
            style = MaterialTheme.typography.bodySmall,
            color = MaterialTheme.colorScheme.onSurfaceVariant
        )

        Spacer(modifier = Modifier.height(8.dp))

        Box(
            modifier = Modifier
                .fillMaxWidth()
                .height(200.dp)
        ) {
            GoogleMap(
                modifier = Modifier.fillMaxWidth(),
                cameraPositionState = cameraPositionState,
                onMapClick = { latLng ->
                    markerPosition = latLng
                    onLocationSelected(latLng.latitude, latLng.longitude)
                }
            ) {
                Marker(
                    state = MarkerState(position = markerPosition),
                    title = "Car Location",
                    alpha = markerAlpha
                )
            }
        }

        LaunchedEffect(markerPosition) {
            onLocationSelected(markerPosition.latitude, markerPosition.longitude)
        }
    }
}

