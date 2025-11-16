# Cars Mock Server

A lightweight Ktor-based REST API server for development and testing of the Cars Android app.

## Features

- RESTful API with JWT authentication
- In-memory data storage
- Pre-populated sample data (5 cars)
- Delta sync support (`?since=` parameter)
- CORS enabled
- Request logging

## Running the Server

### Option 1: Gradle

```bash
./gradlew :server:run
```

### Option 2: IDE

Run `com.example.cars.server.ApplicationKt` from IntelliJ IDEA or Android Studio.

## Server Details

- **Host**: `0.0.0.0`
- **Port**: `8080`
- **Base URL**: `http://localhost:8080/`

From Android emulator, use: `http://10.0.2.2:8080/`

## API Endpoints

### Authentication

#### Login
```http
POST /auth/login
Content-Type: application/json

{
  "email": "demo@demo.com",
  "password": "demo1234"
}

Response:
{
  "token": "mock_jwt_token_..."
}
```

**Note**: Server simulates occasional 401 errors (10% chance) for testing.

### Cars (Require Authorization header)

#### List All Cars
```http
GET /cars?page=1&pageSize=20&since=2025-01-01T12:00:00Z
Authorization: Bearer <token>

Response:
{
  "cars": [...],
  "page": 1,
  "pageSize": 20,
  "total": 5
}
```

Parameters:
- `page` (optional): Page number, default 1
- `pageSize` (optional): Items per page, default 20
- `since` (optional): ISO 8601 timestamp for delta sync

#### Get Single Car
```http
GET /cars/{id}
Authorization: Bearer <token>

Response:
{
  "id": "1",
  "make": "Tesla",
  "model": "Model 3 Long Range AWD",
  ...
}
```

#### Create Car
```http
POST /cars
Authorization: Bearer <token>
Content-Type: application/json

{
  "id": "",
  "make": "Ford",
  "model": "Mustang",
  "year": 2023,
  "price": 55000.0,
  "mileage": 0,
  "fuelType": "Gasoline",
  "transmission": "Manual",
  "description": "Brand new",
  "updatedAt": "2025-01-01T12:00:00Z",
  "lat": 44.4268,
  "lng": 26.1025
}

Response: Created car with generated ID
```

#### Update Car
```http
PUT /cars/{id}
Authorization: Bearer <token>
Content-Type: application/json

{
  "id": "1",
  "make": "Tesla",
  ...
}

Response: Updated car
```

#### Delete Car
```http
DELETE /cars/{id}
Authorization: Bearer <token>

Response: 204 No Content
```

## Sample Data

The server includes 5 pre-populated cars:

1. **Tesla Model 3 Long Range AWD** - $38,999
2. **BMW M5 Competition** - $105,000
3. **Mercedes-Benz AMG GT 63 S 4MATIC+** - $135,000
4. **Porsche 911 Carrera S** - $125,000
5. **Audi RS6 Avant** - $115,000

All with images from Unsplash and Bucharest coordinates.

## Error Responses

- `401 Unauthorized`: Missing or invalid token
- `404 Not Found`: Car doesn't exist
- `500 Internal Server Error`: Server error

## Development

### Add More Sample Data

Edit `initializeSampleData()` in `Application.kt`:

```kotlin
val newCar = CarDto(
    id = "6",
    make = "Your Make",
    model = "Your Model",
    // ... other fields
)
carsDb[newCar.id] = newCar
```

### Change Port

Edit `main()` function:

```kotlin
embeddedServer(Netty, port = 9090, host = "0.0.0.0") {
    // ...
}.start(wait = true)
```

### Enable HTTPS

Add SSL configuration in Netty settings (requires certificate).

## Testing

Use curl to test endpoints:

```bash
# Login
curl -X POST http://localhost:8080/auth/login \
  -H "Content-Type: application/json" \
  -d '{"email":"demo@demo.com","password":"demo1234"}'

# Get cars (replace TOKEN)
curl http://localhost:8080/cars \
  -H "Authorization: Bearer TOKEN"
```

Or use Postman, Insomnia, or any HTTP client.

## Logging

The server logs all requests to console. Output includes:
- HTTP method and path
- Status code
- Response time

Example:
```
12:00:00.123 [main] INFO  ktor.application - Responding at http://0.0.0.0:8080
12:00:05.456 [eventLoopGroupProxy-1-1] INFO  ktor.application - POST /auth/login - 200 OK - 45ms
```

## Troubleshooting

**Port already in use?**
- Kill process: `lsof -ti:8080 | xargs kill`
- Or change port in Application.kt

**Module not found?**
- Run `./gradlew :server:build` first

**CORS errors?**
- CORS is already configured for all origins
- Check browser console for details

## Dependencies

- Ktor Server 2.3.7
- kotlinx.serialization
- kotlinx.datetime
- Logback

## License

Educational purposes.

