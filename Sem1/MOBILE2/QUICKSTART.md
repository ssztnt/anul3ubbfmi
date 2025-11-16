# Quick Start Guide

Get the Cars app running in 5 minutes!

## Prerequisites

- Android Studio installed
- JDK 17+
- Google Maps API key

## Steps

### 1. Get a Maps API Key (2 minutes)

1. Visit [Google Cloud Console](https://console.cloud.google.com/)
2. Create/select project
3. Enable "Maps SDK for Android"
4. Create API Key

### 2. Configure the Project (1 minute)

Create `local.properties` in project root:

```properties
MAPS_API_KEY=YOUR_ACTUAL_KEY_HERE
```

### 3. Start the Mock Server (1 minute)

```bash
./gradlew :server:run
```

Keep this terminal open. Server runs on `http://localhost:8080`

### 4. Run the App (1 minute)

1. Open project in Android Studio
2. Wait for Gradle sync
3. Click Run ▶️
4. Select emulator/device

### 5. Login

- Email: `demo@demo.com`
- Password: `demo1234`

## That's It! 🎉

You should now see:
- ✅ Login screen
- ✅ Cars list with sample data
- ✅ Online indicator (top-left)
- ✅ Motion sensor indicator
- ✅ Add/Edit/Delete cars functionality
- ✅ Google Maps location picker

## Troubleshooting

**Maps not showing?**
- Double-check your API key in `local.properties`
- Ensure Maps SDK is enabled in Google Cloud

**Can't connect to server?**
- Make sure `./gradlew :server:run` is running
- Check console for "Server running" message

**Build errors?**
- Run `./gradlew clean`
- File → Invalidate Caches and Restart

## Next Steps

- Add more cars
- Try offline mode (disable network in emulator)
- Test background sync (wait 15 minutes or tap sync icon)
- Explore the code structure

Enjoy! 🚗

