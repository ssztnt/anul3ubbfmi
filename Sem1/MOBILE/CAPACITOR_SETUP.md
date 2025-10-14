# Capacitor Setup Instructions

## Initial Setup

After installing dependencies, you need to sync Capacitor:

```bash
cd myApp
npx cap sync
```

This command:
- Copies web assets to native projects
- Updates native dependencies
- Installs Capacitor plugins

## Opening Native IDEs

### iOS (macOS only)
```bash
npx cap open ios
```

This opens Xcode. Before building:

1. Add camera and location permissions to `ios/App/App/Info.plist`:

```xml
<key>NSCameraUsageDescription</key>
<string>This app uses the camera to take photos of flowers.</string>

<key>NSLocationWhenInUseUsageDescription</key>
<string>This app uses location to tag flowers with their location.</string>

<key>NSPhotoLibraryUsageDescription</key>
<string>This app needs access to your photo library.</string>
```

2. Select a development team in Xcode (Signing & Capabilities)
3. Build and run on simulator or device

### Android

```bash
npx cap open android
```

This opens Android Studio. Permissions are automatically handled via `AndroidManifest.xml`.

Build and run on emulator or device.

## Web Development

For web development, the native plugins have browser fallbacks:

- **Camera**: Uses file input picker
- **Filesystem**: Uses base64 data URLs
- **Geolocation**: Uses browser geolocation API
- **Network**: Uses navigator.onLine

Simply run:
```bash
npm run dev
```

## Live Reload on Device

For live reload during development:

```bash
# 1. Find your local IP
ipconfig getifaddr en0  # macOS
# or
ip addr show  # Linux

# 2. Update capacitor.config.ts with your IP:
server: {
  url: 'http://YOUR_IP:5173',
  cleartext: true
}

# 3. Sync and run
npx cap sync
npx cap run ios  # or android
```

## Troubleshooting

### Camera not working on iOS
- Check Info.plist has NSCameraUsageDescription
- Make sure to allow camera permission when prompted
- Test on real device (camera doesn't work on simulator)

### Location not working
- Check Info.plist has NSLocationWhenInUseUsageDescription (iOS)
- Allow location permission when prompted
- Test on real device for best results

### Build errors after installing new plugins
```bash
npx cap sync
```

Run this command whenever you install new Capacitor plugins.

### "Plugin not implemented" error
This usually means:
1. You forgot to run `npx cap sync` after installing a plugin
2. You're trying to use a plugin that doesn't have a web implementation (camera on web uses fallback)

## Updating Capacitor

```bash
npm install @capacitor/cli@latest @capacitor/core@latest
npm install @capacitor/ios@latest @capacitor/android@latest
npx cap sync
```

## Platform-Specific Code

The app automatically detects platform:

```typescript
import { Capacitor } from '@capacitor/core';

if (Capacitor.getPlatform() === 'ios') {
  // iOS-specific code
} else if (Capacitor.getPlatform() === 'android') {
  // Android-specific code
} else {
  // Web fallback
}
```

## Production Build

### Web
```bash
npm run build
# Output in dist/
```

### iOS
```bash
npm run build
npx cap sync
npx cap open ios
# Archive in Xcode
```

### Android
```bash
npm run build
npx cap sync
npx cap open android
# Build > Generate Signed Bundle / APK
```

