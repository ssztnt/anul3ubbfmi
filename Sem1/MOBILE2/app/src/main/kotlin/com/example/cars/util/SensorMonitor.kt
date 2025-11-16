package com.example.cars.util

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import dagger.hilt.android.qualifiers.ApplicationContext
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.callbackFlow
import kotlinx.coroutines.flow.distinctUntilChanged
import javax.inject.Inject
import javax.inject.Singleton
import kotlin.math.sqrt

@Singleton
class SensorMonitor @Inject constructor(
    @ApplicationContext private val context: Context
) {
    private val sensorManager = context.getSystemService(Context.SENSOR_SERVICE) as SensorManager

    enum class MotionState {
        STABLE, MOVING
    }

    enum class LightLevel {
        DARK, DIM, NORMAL, BRIGHT
    }

    enum class DeviceOrientation {
        FLAT, PORTRAIT, LANDSCAPE, UPSIDE_DOWN
    }

    // Motion detection (accelerometer)
    val motionState: Flow<MotionState> = callbackFlow {
        val sensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER)
            ?: sensorManager.getDefaultSensor(Sensor.TYPE_STEP_COUNTER)

        if (sensor == null) {
            trySend(MotionState.STABLE)
            close()
            return@callbackFlow
        }

        val listener = object : SensorEventListener {
            private var lastUpdate = 0L
            private val threshold = 1.5f

            override fun onSensorChanged(event: SensorEvent) {
                val currentTime = System.currentTimeMillis()
                
                if (currentTime - lastUpdate < 200) return
                
                lastUpdate = currentTime

                when (event.sensor.type) {
                    Sensor.TYPE_ACCELEROMETER -> {
                        val x = event.values[0]
                        val y = event.values[1]
                        val z = event.values[2]

                        val acceleration = sqrt(x * x + y * y + z * z) - SensorManager.GRAVITY_EARTH
                        
                        val state = if (kotlin.math.abs(acceleration) > threshold) {
                            MotionState.MOVING
                        } else {
                            MotionState.STABLE
                        }
                        
                        trySend(state)
                    }
                    Sensor.TYPE_STEP_COUNTER -> {
                        trySend(MotionState.MOVING)
                    }
                }
            }

            override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) {}
        }

        sensorManager.registerListener(
            listener,
            sensor,
            SensorManager.SENSOR_DELAY_NORMAL
        )

        awaitClose {
            sensorManager.unregisterListener(listener)
        }
    }.distinctUntilChanged()

    // Light sensor
    val lightLevel: Flow<LightLevel> = callbackFlow {
        val sensor = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT)

        if (sensor == null) {
            trySend(LightLevel.NORMAL)
            close()
            return@callbackFlow
        }

        val listener = object : SensorEventListener {
            override fun onSensorChanged(event: SensorEvent) {
                val lux = event.values[0]
                
                val level = when {
                    lux < 10 -> LightLevel.DARK
                    lux < 100 -> LightLevel.DIM
                    lux < 1000 -> LightLevel.NORMAL
                    else -> LightLevel.BRIGHT
                }
                
                trySend(level)
            }

            override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) {}
        }

        sensorManager.registerListener(
            listener,
            sensor,
            SensorManager.SENSOR_DELAY_NORMAL
        )

        awaitClose {
            sensorManager.unregisterListener(listener)
        }
    }.distinctUntilChanged()

    // Gyroscope for device orientation
    val deviceOrientation: Flow<DeviceOrientation> = callbackFlow {
        val sensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER)

        if (sensor == null) {
            trySend(DeviceOrientation.PORTRAIT)
            close()
            return@callbackFlow
        }

        val listener = object : SensorEventListener {
            private var lastUpdate = 0L

            override fun onSensorChanged(event: SensorEvent) {
                val currentTime = System.currentTimeMillis()
                if (currentTime - lastUpdate < 500) return
                lastUpdate = currentTime

                val x = event.values[0]
                val y = event.values[1]
                val z = event.values[2]

                val orientation = when {
                    kotlin.math.abs(z) > 8 -> DeviceOrientation.FLAT
                    kotlin.math.abs(x) > kotlin.math.abs(y) && x > 0 -> DeviceOrientation.LANDSCAPE
                    kotlin.math.abs(x) > kotlin.math.abs(y) && x < 0 -> DeviceOrientation.LANDSCAPE
                    y < -3 -> DeviceOrientation.UPSIDE_DOWN
                    else -> DeviceOrientation.PORTRAIT
                }

                trySend(orientation)
            }

            override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) {}
        }

        sensorManager.registerListener(
            listener,
            sensor,
            SensorManager.SENSOR_DELAY_NORMAL
        )

        awaitClose {
            sensorManager.unregisterListener(listener)
        }
    }.distinctUntilChanged()

    fun hasAccelerometer(): Boolean {
        return sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER) != null
    }

    fun hasLightSensor(): Boolean {
        return sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT) != null
    }

    fun hasGyroscope(): Boolean {
        return sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE) != null
    }
}
