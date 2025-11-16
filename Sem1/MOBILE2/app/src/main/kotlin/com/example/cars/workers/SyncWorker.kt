package com.example.cars.workers

import android.content.Context
import androidx.hilt.work.HiltWorker
import androidx.work.CoroutineWorker
import androidx.work.ForegroundInfo
import androidx.work.WorkerParameters
import com.example.cars.data.repository.CarsRepository
import com.example.cars.util.NetworkMonitor
import com.example.cars.util.NotificationHelper
import com.example.cars.util.Result
import dagger.assisted.Assisted
import dagger.assisted.AssistedInject

@HiltWorker
class SyncWorker @AssistedInject constructor(
    @Assisted appContext: Context,
    @Assisted workerParams: WorkerParameters,
    private val carsRepository: CarsRepository,
    private val networkMonitor: NetworkMonitor,
    private val notificationHelper: NotificationHelper
) : CoroutineWorker(appContext, workerParams) {

    override suspend fun doWork(): androidx.work.ListenableWorker.Result {
        // Check if online
        if (!networkMonitor.isCurrentlyOnline()) {
            return androidx.work.ListenableWorker.Result.retry()
        }

        // Set foreground for long-running sync
        setForeground(createForegroundInfo())

        return try {
            val result = carsRepository.syncCars()
            
            notificationHelper.cancelSyncNotification()
            
            if (result.isSuccess) {
                // Show completion notification
                notificationHelper.showSyncCompleteNotification(0) // Could count synced items
                androidx.work.ListenableWorker.Result.success()
            } else {
                androidx.work.ListenableWorker.Result.retry()
            }
        } catch (e: Exception) {
            notificationHelper.cancelSyncNotification()
            androidx.work.ListenableWorker.Result.retry()
        }
    }

    private fun createForegroundInfo(): ForegroundInfo {
        return ForegroundInfo(
            NotificationHelper.NOTIFICATION_ID_SYNC,
            notificationHelper.showSyncNotification(isOngoing = true)
        )
    }

    companion object {
        const val WORK_NAME = "sync_cars_work"
    }
}

