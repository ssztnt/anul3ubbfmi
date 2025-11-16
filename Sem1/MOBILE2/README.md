# Cars - Android App

A production-ready Android application for managing cars with JWT authentication, offline-first architecture, background synchronization, and Google Maps integration.

## Features

✅ **Authentication (JWT)** - Secure login with token-based authentication  
✅ **Master-Detail UI** - Browse and edit cars with a beautiful Material 3 interface  
✅ **Offline-First** - Full offline support with Room database and DataStore  
✅ **Background Sync** - Automatic synchronization with WorkManager  
✅ **System Services** - Network status monitoring, notifications, sensor integration  
✅ **Location & Maps** - Google Maps integration with location picker  
✅ **Camera Mode** - Alternative camera mode (feature flag)  
✅ **Animations** - Smooth transitions and Material motion  
✅ **Sensors** - Accelerometer integration for motion detection  

## Tech Stack

- **Language**: Kotlin
- **UI**: Jetpack Compose + Material 3
- **Architecture**: MVVM with Repository pattern
- **DI**: Hilt
- **Async**: Coroutines + Flow
- **Networking**: Retrofit + OkHttp + kotlinx.serialization
- **Database**: Room + DataStore
- **Background**: WorkManager
- **Maps**: Google Maps Compose
- **Image Loading**: Coil
- **Min SDK**: 26 (Android 8.0)
- **Target SDK**: 35 (Android 15)

## Prerequisites

1. **Android Studio** Hedgehog (2023.1.1) or newer
2. **JDK** 17 or newer
3. **Android SDK** with API 35
4. **Google Maps API Key** (see setup below)
5. **Gradle** 8.2+ (bundled with Android Studio)

## Setup Instructions

### 1. Clone the Repository

```bash
git clone <repository-url>
cd MOBILE2
```

### 2. Get a Google Maps API Key

1. Go to [Google Cloud Console](https://console.cloud.google.com/)
2. Create a new project or select an existing one
3. Enable the **Maps SDK for Android** API
4. Create credentials → API Key
5. Restrict the API key to Android apps (optional but recommended)

### 3. Configure the API Key

Create or edit `local.properties` in the project root:

```properties
MAPS_API_KEY=YOUR_ACTUAL_API_KEY_HERE
```

**Important**: Never commit your API key to version control!

### 4. Run the Mock Server

The app requires a mock backend server for development.

#### Option A: Using Gradle (Recommended)

```bash
# From project root
./gradlew :server:run
```

The server will start on `http://localhost:8080`

#### Option B: Using IntelliJ IDEA / Android Studio

1. Open the `server` module
2. Run `com.example.cars.server.ApplicationKt`

#### Verify the Server

Open in browser: `http://localhost:8080/` - you should see "Cars Mock Server is running!"

### 5. Run the Android App

#### From Android Studio:

1. Open the project in Android Studio
2. Wait for Gradle sync to complete
3. Select an emulator or connected device
4. Click **Run** ▶️

#### From Command Line:

```bash
./gradlew :app:installDebug
```

### 6. Login Credentials

Use these demo credentials to login:

- **Email**: `demo@demo.com`
- **Password**: `demo1234`

## Project Structure

```
app/src/main/kotlin/com/example/cars/
├── app/                    # Application class, theme, navigation
├── data/
│   ├── local/             # Room entities, DAOs, DataStore
│   ├── remote/            # Retrofit API, DTOs, interceptors
│   └── repository/        # Repository implementations
├── domain/
│   ├── model/             # Domain models
│   └── mapper/            # Data mappers
├── ui/
│   ├── auth/              # Login screen & ViewModel
│   ├── cars/
│   │   ├── list/          # Cars list screen
│   │   └── detail/        # Car detail/edit screen
│   └── common/            # Reusable UI components
├── workers/               # Background sync worker
├── util/                  # Utilities (network monitor, sensors, etc.)
└── di/                    # Hilt dependency injection modules
```

## Key Features Explained

### Offline-First Architecture

The app uses Room as the single source of truth:
- All data is cached locally
- UI reads from local database
- Changes are marked for sync and pushed when online
- Conflict resolution uses last-write-wins based on `updatedAt`

### Background Synchronization

- **Periodic Sync**: Every 15 minutes (WorkManager)
- **Manual Sync**: Pull-to-refresh or tap "Sync Now"
- **Smart Sync**: Only fetches changes since last sync (`?since=` query)
- **Foreground Service**: Long syncs show a notification

### Network Status

- Real-time network monitoring
- Online/Offline indicator in the app bar (left side)
- Remote actions disabled when offline
- Graceful offline experience

### Sensors

- Uses accelerometer to detect device motion
- Shows "Stable" or "Moving" indicator on the list screen
- Fallback to step counter if accelerometer unavailable
- Throttled updates for battery efficiency

### Location & Maps

- **Default Mode**: Google Maps with location picker
- Tap the map to set car location
- Smooth marker animations
- Runtime permission handling with rationale

### Camera Mode (Feature Flag)

Toggle between Maps and Camera mode:

1. Set the feature flag in DataStore:
   ```kotlin
   // In AuthPreferencesDataSource
   setUseCameraMode(true)
   ```

2. Or add a Settings screen to toggle via UI

When enabled:
- Replaces map picker with camera capture
- Uses CameraX for photo capture
- Saves images to app cache

### Animations

- **List Items**: Staggered fade-in, slide animations
- **Screen Transitions**: AnimatedContent with slide/fade
- **Map Markers**: Drop and bounce animation
- **Motion**: Spring animations for interactive elements

## Configuration

### Feature Flags

Edit `AuthPreferencesDataSource` to enable/disable features:

```kotlin
// Toggle camera mode
authRepository.setUseCameraMode(true)  // Camera mode
authRepository.setUseCameraMode(false) // Maps mode (default)
```

### API Endpoint

Default: `http://10.0.2.2:8080/` (Android emulator → localhost)

For physical device, edit `NetworkModule.kt`:

```kotlin
.baseUrl("http://YOUR_COMPUTER_IP:8080/")
```

### WorkManager Sync Interval

Edit `WorkModule.kt`:

```kotlin
PeriodicWorkRequestBuilder<SyncWorker>(
    15, TimeUnit.MINUTES  // Change interval here
)
```

## Testing

### Run Unit Tests

```bash
./gradlew :app:testDebugUnitTest
```

### Run Instrumented Tests

```bash
./gradlew :app:connectedDebugAndroidTest
```

Tests cover:
- Repository logic (auth, cars, sync)
- ViewModels
- UI components (login, list)

## Building

### Debug Build

```bash
./gradlew :app:assembleDebug
```

Output: `app/build/outputs/apk/debug/app-debug.apk`

### Release Build

```bash
./gradlew :app:assembleRelease
```

**Note**: Configure signing in `app/build.gradle.kts` for production releases.

## Emulator Setup

### Recommended Settings:

- **Device**: Pixel 5 or newer
- **API Level**: 34 or 35
- **System Image**: Google APIs (for Maps)
- **RAM**: 2GB+
- **Internal Storage**: 2GB+

### Enable Location:

1. Open emulator extended controls (⋯)
2. Location → Set custom location
3. Or use "Playback" with GPX/KML files

## Troubleshooting

### Maps Not Showing?

1. Verify API key is correct in `local.properties`
2. Ensure Maps SDK for Android is enabled in Google Cloud Console
3. Check Logcat for API key errors
4. Make sure you're using a Google Play system image

### Server Connection Failed?

1. Ensure mock server is running (`./gradlew :server:run`)
2. For emulator, use `10.0.2.2` (already configured)
3. For physical device, use your computer's IP address
4. Check firewall settings

### Sync Not Working?

1. Check network status indicator
2. Verify you're logged in
3. Look for WorkManager logs in Logcat: `tag:SyncWorker`
4. Try manual sync (tap sync icon)

### Permission Denied?

1. Grant location permission when prompted
2. For camera mode, grant camera permission
3. Check Android settings if denied

## Known Limitations

1. **Camera Mode**: Basic implementation provided; full CameraX integration requires additional UI work
2. **Pagination**: Infinite scroll implemented, but server returns all items (simple mock)
3. **Conflict Resolution**: Last-write-wins only; no merge conflict UI
4. **Image Upload**: Camera images saved locally; upload to CDN not implemented in mock
5. **Refresh Token**: Not implemented (mock returns long-lived tokens)

## API Endpoints

Mock server endpoints:

| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/auth/login` | Login with email/password |
| GET | `/cars` | List all cars (supports `?page=`, `?pageSize=`, `?since=`) |
| GET | `/cars/{id}` | Get single car |
| POST | `/cars` | Create new car |
| PUT | `/cars/{id}` | Update car |
| DELETE | `/cars/{id}` | Delete car |

## License

This project is created for educational purposes.

## Support

For issues or questions:
1. Check the troubleshooting section
2. Review Logcat output
3. Verify all prerequisites are installed
4. Ensure mock server is running

---

**Happy Coding! 🚗💨**

