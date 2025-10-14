import { Geolocation } from '@capacitor/geolocation';

export interface Location {
  lat: number;
  lng: number;
}

export async function getCurrentPosition(): Promise<Location | null> {
  try {
    const position = await Geolocation.getCurrentPosition({
      enableHighAccuracy: true,
      timeout: 10000,
    });

    return {
      lat: position.coords.latitude,
      lng: position.coords.longitude,
    };
  } catch (error) {
    console.error('Geolocation error:', error);
    return null;
  }
}

export async function checkPermissions(): Promise<boolean> {
  try {
    const status = await Geolocation.checkPermissions();
    if (status.location === 'granted') {
      return true;
    }

    const requested = await Geolocation.requestPermissions();
    return requested.location === 'granted';
  } catch (error) {
    console.error('Permission error:', error);
    return false;
  }
}

