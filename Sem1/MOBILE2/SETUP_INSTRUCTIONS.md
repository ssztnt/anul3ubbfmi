# Android App Setup Instructions

Follow these steps to get the Cars Android app running on your Mac.

## ✅ Prerequisites Checklist

- [ ] Android Studio installed
- [ ] Google Maps API key obtained
- [ ] Mock server running (already done! ✅)

---

## Step 1: Install Android Studio (if not already done)

1. **Download**: [Android Studio](https://developer.android.com/studio)
   - Choose: **Mac (Apple Silicon)** if you have M1/M2/M3 Mac
   - Choose: **Mac (Intel)** for older Intel Macs

2. **Install**:
   - Open the downloaded `.dmg` file
   - Drag `Android Studio` to `Applications` folder
   - Open Android Studio from Applications

3. **First Run Setup**:
   - Choose **Standard** installation
   - Wait for SDK components to download (~5-10 minutes)
   - Accept licenses when prompted

---

## Step 2: Get Google Maps API Key

### Quick Steps:

1. Go to [Google Cloud Console](https://console.cloud.google.com/)
2. Create a project (or select existing one)
3. Go to **APIs & Services** → **Library**
4. Search for "Maps SDK for Android"
5. Click **Enable**
6. Go to **APIs & Services** → **Credentials**
7. Click **Create Credentials** → **API Key**
8. Copy your API key (looks like: `AIzaSyD...`)

### Configure API Key:

Edit the file: `local.properties` in the project root and replace:

```properties
MAPS_API_KEY=YOUR_MAPS_API_KEY_HERE
```

with your actual key:

```properties
MAPS_API_KEY=AIzaSyD_your_actual_key_here
```

---

## Step 3: Open the Project in Android Studio

1. **Open Android Studio**

2. **Open Project**:
   - Click **Open** on the welcome screen
   - Navigate to: `/Users/plaiurares/Documents/Facultate/anul3ubbfmi/Sem1/MOBILE2`
   - Click **Open**

3. **Wait for Gradle Sync**:
   - Android Studio will automatically sync Gradle
   - This may take 2-5 minutes the first time
   - You'll see progress at the bottom of the window

4. **If prompted about missing SDK**:
   - Click "Install missing SDK components"
   - Accept licenses

---

## Step 4: Run the App

### Option A: Using Emulator (Recommended for Testing)

1. **Create an Emulator**:
   - Click **Device Manager** (phone icon in toolbar)
   - Click **Create Device**
   - Select **Pixel 5** or any recent phone
   - Download a system image: **API 34** or **API 35** (choose one with Google APIs)
   - Click **Finish**

2. **Run the App**:
   - Click the green **Run** button ▶️ (or press `Ctrl+R`)
   - Select your emulator
   - Wait for the emulator to boot (~1-2 minutes first time)
   - The app will install and launch automatically

### Option B: Using Physical Device

1. **Enable Developer Options** on your Android phone:
   - Go to Settings → About Phone
   - Tap "Build Number" 7 times
   - Go back → Developer Options
   - Enable "USB Debugging"

2. **Connect via USB**:
   - Connect your phone to Mac with USB cable
   - Accept "Allow USB debugging" on phone
   - Select your device in Android Studio
   - Click Run ▶️

---

## Step 5: Login and Test

1. **App opens to Login screen**

2. **Enter credentials**:
   ```
   Email: demo@demo.com
   Password: demo1234
   ```

3. **Explore the app**:
   - Browse 5 sample cars (Tesla, BMW, Mercedes, Porsche, Audi)
   - Tap a car to view details
   - Edit car information
   - Add a new car (tap + button)
   - Try searching for cars
   - Check the online indicator (top-left)
   - Watch the motion sensor indicator

---

## Troubleshooting

### "Gradle sync failed"
- Go to **File** → **Invalidate Caches** → **Invalidate and Restart**
- Make sure you have internet connection

### "Maps not showing / blank screen"
- Check that you entered the correct API key in `local.properties`
- Verify Maps SDK for Android is enabled in Google Cloud Console
- Restart the app

### "Cannot connect to server"
- Make sure the mock server is running: `./gradlew :server:run`
- You should see: "Responding at http://0.0.0.0:8080" in terminal
- For emulator, the app uses `http://10.0.2.2:8080` (already configured)

### "App crashes on startup"
- Check Android Studio Logcat (bottom panel) for error messages
- Make sure emulator is using API level 26 or higher
- Try: **Build** → **Clean Project**, then run again

### Build errors
```bash
# Clean and rebuild
cd /Users/plaiurares/Documents/Facultate/anul3ubbfmi/Sem1/MOBILE2
./gradlew clean
./gradlew :app:assembleDebug
```

---

## Quick Commands Reference

### Start Mock Server:
```bash
cd /Users/plaiurares/Documents/Facultate/anul3ubbfmi/Sem1/MOBILE2
./gradlew :server:run
```

### Test Server:
```bash
curl http://localhost:8080/
# Should show: Cars Mock Server is running!
```

### Build App from Terminal:
```bash
./gradlew :app:assembleDebug
```

### Install App to Connected Device:
```bash
./gradlew :app:installDebug
```

---

## What You Should See

### Login Screen:
- Email and password fields
- "Demo: demo@demo.com / demo1234" hint
- Login button

### Cars List:
- Online indicator (top-left) 🌐
- 5 sample cars with images
- Search bar
- Motion sensor indicator
- Add car button (floating action button)
- Sync button (top-right)

### Car Detail:
- All car information (make, model, year, price, etc.)
- Google Map showing car location
- Save and Delete buttons
- Tap map to change location

### Features to Test:
- ✅ Add a new car
- ✅ Edit existing car
- ✅ Delete a car (with confirmation)
- ✅ Search for cars by make/model
- ✅ Manual sync (tap sync icon)
- ✅ Offline mode (disable network in emulator)
- ✅ Motion sensor (move device/emulator)

---

## Project Structure Overview

```
MOBILE2/
├── app/                    # Android application
│   ├── src/main/kotlin/    # Kotlin source code
│   │   └── com/example/cars/
│   │       ├── app/        # Application, theme, navigation
│   │       ├── data/       # Database, API, repositories
│   │       ├── domain/     # Domain models
│   │       ├── ui/         # UI screens (Compose)
│   │       ├── workers/    # Background sync
│   │       └── util/       # Utilities
│   └── build.gradle.kts    # App dependencies
├── server/                 # Mock API server (Ktor)
│   └── src/main/kotlin/    # Server code
├── local.properties        # Your Maps API key (created)
└── README.md              # Full documentation
```

---

## Next Steps After Setup

1. **Explore the Code**:
   - Open `app/src/main/kotlin/com/example/cars/`
   - Check out the UI screens in `ui/` folder
   - Look at the database in `data/local/`

2. **Read Documentation**:
   - `README.md` - Comprehensive guide
   - `FEATURES.md` - Feature implementation details
   - `QUICKSTART.md` - Quick reference

3. **Test Features**:
   - Try offline mode
   - Add multiple cars
   - Test background sync (wait 15 minutes)
   - Test location picker on map

4. **Customize**:
   - Change app theme in `app/theme/`
   - Add more sample data in server code
   - Modify sync interval in `WorkModule.kt`

---

## Need Help?

- Check `README.md` for detailed information
- Look at `FEATURES.md` for code references
- Review Android Studio Logcat for errors
- Ensure mock server is running before testing

---

**You're all set! 🚗💨**

The app is ready to run. Just open it in Android Studio, add your Maps API key, and click Run!


