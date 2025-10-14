import { Camera, CameraResultType, CameraSource } from '@capacitor/camera';
import { Filesystem, Directory } from '@capacitor/filesystem';
import { Capacitor } from '@capacitor/core';
import { apiPost } from '../services/api';

export interface PhotoResult {
  base64: string;
  webPath?: string;
  serverUrl?: string;
}

export async function takePhoto(): Promise<PhotoResult | null> {
  try {
    const photo = await Camera.getPhoto({
      quality: 80,
      allowEditing: false,
      resultType: CameraResultType.Base64,
      source: CameraSource.Camera,
    });

    if (!photo.base64String) {
      throw new Error('No photo data');
    }

    return {
      base64: photo.base64String,
      webPath: photo.webPath,
    };
  } catch (error) {
    console.error('Camera error:', error);
    return null;
  }
}

export async function uploadPhoto(base64: string): Promise<string> {
  const response = await apiPost<{ url: string }>('/upload', {
    base64,
    ext: 'jpg',
  });
  return response.url;
}

export async function savePhotoToDevice(base64: string, filename: string): Promise<string> {
  if (Capacitor.getPlatform() === 'web') {
    return `data:image/jpeg;base64,${base64}`;
  }

  try {
    const savedFile = await Filesystem.writeFile({
      path: filename,
      data: base64,
      directory: Directory.Data,
    });

    return savedFile.uri;
  } catch (error) {
    console.error('Error saving photo:', error);
    return `data:image/jpeg;base64,${base64}`;
  }
}

