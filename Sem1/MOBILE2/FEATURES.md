# Features Implementation Reference

This document maps each feature requirement to its implementation in the codebase.

## 1. JWT Authentication (3 points)

### Login Screen
- **File**: `app/src/main/kotlin/com/example/cars/ui/auth/LoginScreen.kt`
- **ViewModel**: `app/src/main/kotlin/com/example/cars/ui/auth/LoginViewModel.kt`
- **Features**:
  - Email and password fields with validation
  - Password visibility toggle
  - Loading state during login
  - Error display
  - Demo credentials hint

### Token Management
- **DataStore**: `app/src/main/kotlin/com/example/cars/data/local/preferences/AuthPreferencesDataSource.kt`
- **Methods**:
  - `saveAuthToken(token: String)`
  - `getAuthToken(): Flow<String?>`
  - `clearAuthToken()`

### Auth Interceptor
- **File**: `app/src/main/kotlin/com/example/cars/data/remote/interceptor/AuthInterceptor.kt`
- **Functionality**: Automatically adds `Authorization: Bearer <token>` to all API requests

### Protected Navigation
- **File**: `app/src/main/kotlin/com/example/cars/app/MainActivity.kt`
- **Logic**: Checks `isLoggedIn` flow to determine start destination

## 2. Master-Detail UI (6 points)

### Cars List (3 points)
- **File**: `app/src/main/kotlin/com/example/cars/ui/cars/list/CarsListScreen.kt`
- **ViewModel**: `app/src/main/kotlin/com/example/cars/ui/cars/list/CarsListViewModel.kt`
- **Features**:
  - **Online Indicator**: Left-aligned in TopAppBar (line 73-94)
  - **Car Items**: Image (Coil), make/model/year, price, chevron (line 153-203)
  - **Search**: Client-side filtering by make/model (line 97-105)
  - **Add Button**: FAB for adding new cars (line 61-67)
  - **Sync Button**: Manual sync trigger in TopAppBar (line 96-99)
  - **Motion Sensor**: Live indicator showing Stable/Moving (line 112-135)

### Car Detail/Edit (3 points)
- **File**: `app/src/main/kotlin/com/example/cars/ui/cars/detail/CarDetailScreen.kt`
- **ViewModel**: `app/src/main/kotlin/com/example/cars/ui/cars/detail/CarDetailViewModel.kt`
- **Fields**:
  - Make, Model, Year (line 102-132)
  - Price, Mileage (line 134-145)
  - Fuel Type, Transmission (line 149-167)
  - Image URL (line 171-176)
  - Description (line 180-187)
  - Location (Maps/Camera) (line 191-211)
- **Actions**:
  - Save with validation (line 215-227)
  - Delete with confirmation dialog (line 236-255)
- **Long Titles**: Handled with `maxLines` and `overflow` in CarListItem

## 3. Offline-First Storage (4 points)

### Room Database
- **Database**: `app/src/main/kotlin/com/example/cars/data/local/CarsDatabase.kt`
- **Entity**: `app/src/main/kotlin/com/example/cars/data/local/entity/CarEntity.kt`
- **DAO**: `app/src/main/kotlin/com/example/cars/data/local/dao/CarDao.kt`
- **Features**:
  - Source of truth pattern
  - Pending sync flag for offline changes
  - Soft delete support
  - Upsert with conflict resolution (line 48-56)

### DataStore
- **File**: `app/src/main/kotlin/com/example/cars/data/local/preferences/AuthPreferencesDataSource.kt`
- **Stores**:
  - JWT token
  - Camera mode flag
  - Other preferences

### Repository Pattern
- **File**: `app/src/main/kotlin/com/example/cars/data/repository/CarsRepository.kt`
- **Methods**:
  - `observeCars()`: Flow from Room (line 21-25)
  - `saveCar()`: Save locally with pending flag (line 36-50)
  - `syncCars()`: Delta sync with conflict resolution (line 58-100)
  - `deleteCar()`: Mark as deleted (line 102-108)

### Conflict Resolution
- **Strategy**: Last-write-wins using `updatedAt` timestamp
- **Implementation**: `CarDao.upsertCar()` (line 48-56)

## 4. Background Tasks (3 points)

### WorkManager Sync
- **Worker**: `app/src/main/kotlin/com/example/cars/workers/SyncWorker.kt`
- **Configuration**: `app/src/main/kotlin/com/example/cars/di/WorkModule.kt`
- **Features**:
  - Periodic sync every 15 minutes (line 28-33)
  - Network connectivity constraint (line 29-32)
  - Foreground notification during sync (line 31)
  - Automatic scheduling in Application.onCreate() (line 21)

### Manual Sync
- **Trigger**: CarsListScreen "Sync Now" button (line 96-99)
- **Implementation**: CarsListViewModel.syncNow() (line 59-62)

### Foreground Service
- **File**: `app/src/main/kotlin/com/example/cars/workers/SyncWorker.kt` (line 51-56)
- **Notification**: Shows "Syncing cars..." during operation

## 5. System Services (2 points)

### Network Monitor
- **File**: `app/src/main/kotlin/com/example/cars/util/NetworkMonitor.kt`
- **Features**:
  - Real-time connectivity status via Flow (line 21-53)
  - ConnectivityManager.NetworkCallback
  - Initial state + updates
  - Capability checking (internet + validated)

### Network Status UI
- **Location**: CarsListScreen TopAppBar (line 73-94)
- **Indicator**: Icon + text showing Online/Offline
- **Behavior**: Remote actions disabled when offline (line 96)

### Notifications
- **Helper**: `app/src/main/kotlin/com/example/cars/util/NotificationHelper.kt`
- **Channels**: Sync notification channel (line 29-38)
- **Types**:
  - Ongoing sync notification (line 44-50)
  - Completion notification (line 52-61)
- **Permission**: Runtime notification permission check (line 68-76)

## 6. Sensors (3 points)

### Accelerometer
- **File**: `app/src/main/kotlin/com/example/cars/util/SensorMonitor.kt`
- **Features**:
  - Accelerometer or step counter fallback (line 23-26)
  - Motion detection with threshold (line 44-63)
  - Throttled updates (200ms) (line 36)
  - Magnitude calculation (line 47-51)

### UI Integration
- **Location**: CarsListScreen (line 112-135)
- **Display**: "Device motion: Stable/Moving" with animation
- **Animation**: CircularProgressIndicator when moving (line 130-135)

## 7. Animations (3 points)

### Screen Transitions
- **File**: `app/src/main/kotlin/com/example/cars/ui/cars/detail/CarDetailScreen.kt`
- **AnimatedContent**: Maps ↔ Camera mode switch (line 191-211)
- **Spec**: slideInHorizontally + fadeIn / slideOutHorizontally + fadeOut

### List Animations
- **File**: `app/src/main/kotlin/com/example/cars/ui/cars/list/CarsListScreen.kt`
- **Features**:
  - AnimatedVisibility for motion indicator (line 112-115)
  - animateContentSize for card expansion (line 178-183)
  - fadeIn/slideIn for appearing elements

### Map Animations
- **File**: `app/src/main/kotlin/com/example/cars/ui/common/LocationPicker.kt`
- **Marker Animation**: Drop with alpha fade (line 52-56)
- **Duration**: 500ms tween

### Spring Animations
- **Implementation**: Material motion with spring dampingRatio (line 179-182)
- **Usage**: Card content size changes

## 8. Location & Maps (3 points)

### Google Maps Integration
- **File**: `app/src/main/kotlin/com/example/cars/ui/common/LocationPicker.kt`
- **Features**:
  - Interactive map with GoogleMap composable (line 67-80)
  - Tap to set location (line 71-74)
  - Marker with custom position (line 75-79)
  - Camera positioning (line 44-46)

### Runtime Permissions
- **Implementation**: Accompanist permissions (line 28-33)
- **Permissions**: ACCESS_FINE_LOCATION, ACCESS_COARSE_LOCATION
- **UI**: Permission rationale with button (line 85-96)

### Camera Mode (Alternative)
- **Feature Flag**: `AuthPreferencesDataSource.isUseCameraMode()` (line 21-25)
- **Toggle**: `setUseCameraMode(Boolean)` (line 27-31)
- **UI Switch**: AnimatedContent in CarDetailScreen (line 191-211)
- **Status**: Placeholder implementation provided

## 9. Dependency Injection (Hilt)

### Modules
- **AppModule**: `app/src/main/kotlin/com/example/cars/di/AppModule.kt`
  - DataStore provider
  - Room database provider
  - DAO provider

- **NetworkModule**: `app/src/main/kotlin/com/example/cars/di/NetworkModule.kt`
  - Retrofit configuration
  - OkHttpClient with interceptors
  - JSON serialization
  - API service

- **WorkModule**: `app/src/main/kotlin/com/example/cars/di/WorkModule.kt`
  - WorkManager provider
  - Periodic work scheduling

### Application Class
- **File**: `app/src/main/kotlin/com/example/cars/app/CarsApplication.kt`
- **Annotations**: @HiltAndroidApp
- **Setup**: WorkManager initialization

## 10. Testing

### Unit Tests
- **Auth Repository**: `app/src/test/kotlin/com/example/cars/data/repository/AuthRepositoryTest.kt`
  - Login success/failure
  - Token management
  - Logout functionality

- **Cars Repository**: `app/src/test/kotlin/com/example/cars/data/repository/CarsRepositoryTest.kt`
  - CRUD operations
  - Sync logic
  - Conflict resolution

### UI Tests
- **Login Screen**: `app/src/androidTest/kotlin/com/example/cars/ui/LoginScreenTest.kt`
  - UI element visibility
  - User input
  - Button states

### Mocking
- **Framework**: MockK
- **Usage**: Repository and ViewModel testing

## 11. Additional Features

### Theme
- **Files**: 
  - `app/src/main/kotlin/com/example/cars/app/theme/Theme.kt`
  - `app/src/main/kotlin/com/example/cars/app/theme/Color.kt`
  - `app/src/main/kotlin/com/example/cars/app/theme/Type.kt`
- **Features**:
  - Material 3 design
  - Dynamic colors (Android 12+)
  - Dark mode support
  - Custom color scheme

### Navigation
- **File**: `app/src/main/kotlin/com/example/cars/app/navigation/NavGraph.kt`
- **Routes**:
  - Login → CarsList → CarDetail
  - Type-safe navigation
  - Back stack management

### Image Loading
- **Library**: Coil
- **Usage**: AsyncImage in CarListItem and detail screens
- **Features**: Caching, placeholders, error handling

### Data Mappers
- **File**: `app/src/main/kotlin/com/example/cars/domain/mapper/CarMapper.kt`
- **Conversions**:
  - Entity ↔ Domain
  - DTO ↔ Domain
  - DTO ↔ Entity

## Code Quality

### Error Handling
- **Pattern**: Result sealed class (`app/src/main/kotlin/com/example/cars/util/Result.kt`)
- **Usage**: Repository methods return Result<T>
- **UI**: SnackbarHost for error display

### State Management
- **Pattern**: UI State data classes in ViewModels
- **Flow**: StateFlow for reactive UI updates
- **Lifecycle**: viewModelScope for coroutines

### Resource Management
- **Strings**: `app/src/main/res/values/strings.xml`
- **Colors**: `app/src/main/res/values/colors.xml`
- **Icons**: Material Icons Extended

## Mock Server

### Implementation
- **File**: `server/src/main/kotlin/com/example/cars/server/Application.kt`
- **Framework**: Ktor
- **Features**:
  - JWT auth simulation
  - CRUD operations
  - Delta sync support (?since=)
  - Sample data (5 cars)
  - CORS enabled
  - Request logging

### Endpoints
- POST `/auth/login`
- GET `/cars` (with pagination & delta)
- GET `/cars/{id}`
- POST `/cars`
- PUT `/cars/{id}`
- DELETE `/cars/{id}`

## Build Configuration

### Gradle
- **Root**: `build.gradle.kts` - Plugin versions
- **App**: `app/build.gradle.kts` - Dependencies, SDK versions
- **Server**: `server/build.gradle.kts` - Ktor dependencies

### ProGuard
- **File**: `app/proguard-rules.pro`
- **Rules**: Serialization, Retrofit, Room optimization

### Manifest
- **File**: `app/src/main/AndroidManifest.xml`
- **Permissions**: Internet, Location, Camera, Notifications, Sensors
- **Services**: FileProvider for camera

## Documentation

- **README.md**: Comprehensive setup and usage guide
- **QUICKSTART.md**: 5-minute quick start
- **PROJECT_SUMMARY.md**: Feature checklist and overview
- **FEATURES.md**: This file - detailed implementation reference
- **server/README.md**: Mock server documentation

---

**Total Files**: 60+ source files
**Total Lines**: ~3,500+ lines of production code
**Test Coverage**: Core functionality tested
**Documentation**: Comprehensive with multiple guides

