import React, { useEffect, useState, useRef } from 'react';
import {
  IonBackButton,
  IonButtons,
  IonButton,
  IonContent,
  IonHeader,
  IonInput,
  IonItem,
  IonLabel,
  IonPage,
  IonSelect,
  IonSelectOption,
  IonSpinner,
  IonTextarea,
  IonTitle,
  IonToolbar,
  IonText,
  IonIcon,
  IonImg,
} from '@ionic/react';
import { cloudUpload, map } from 'ionicons/icons';
import { Capacitor } from '@capacitor/core';
import { useParams, useHistory } from 'react-router-dom';
import { Flower } from '../types';
import { apiGet, apiPost, apiPut } from '../services/api';
import { networkStatusService } from '../services/network';
import { syncService } from '../services/sync';
import { getCurrentPosition, checkPermissions } from '../hooks/useGeolocation';
import MapPicker from '../components/MapPicker';

const FlowerEdit: React.FC = () => {
  const { id } = useParams<{ id: string }>();
  const isNew = id === 'new';
  const [name, setName] = useState('');
  const [species, setSpecies] = useState('');
  const [category, setCategory] = useState('Rose');
  const [colors, setColors] = useState<string>('red,white');
  const [price, setPrice] = useState<number>(9.99);
  const [status, setStatus] = useState<'active'|'archived'>('active');
  const [description, setDescription] = useState('');
  const [photoUrl, setPhotoUrl] = useState<string>('');
  const [photoPreview, setPhotoPreview] = useState<string>('');
  const [photoPending, setPhotoPending] = useState(false);
  const [location, setLocation] = useState<{ lat: number; lng: number } | undefined>();
  const [showMapPicker, setShowMapPicker] = useState(false);
  const [loading, setLoading] = useState(!isNew);
  const [saving, setSaving] = useState(false);
  const [uploading, setUploading] = useState(false);
  const [error, setError] = useState('');
  const history = useHistory();
  const fileInputRef = useRef<HTMLInputElement | null>(null);

  useEffect(() => {
    if (!isNew) {
      const fetchData = async () => {
        try {
          const data = await apiGet<Flower>(`/flowers/${id}`);
          setName(data.name);
          setSpecies(data.species);
          setCategory(data.category);
          setColors(data.colors.join(','));
          setPrice(data.price);
          setStatus(data.status);
          setDescription(data.description || '');
          setPhotoUrl(data.photoUrl || '');
          setPhotoPreview(data.photoUrl || '');
          setLocation(data.location);
        } catch (err: any) {
          setError(err.message || 'Failed to load flower');
        } finally {
          setLoading(false);
        }
      };
      fetchData();
    }
  }, [id, isNew]);

  const handleUploadPhoto = async (file: File) => {
    setUploading(true);
    setError('');
    
    try {
      // Convert file to base64
      const toBase64 = (f: File): Promise<string> => {
        return new Promise((resolve, reject) => {
          const reader = new FileReader();
          reader.onload = () => {
            const result = reader.result as string;
            // Remove data URL prefix (e.g., "data:image/jpeg;base64,")
            const base64 = result.split(',')[1];
            resolve(base64);
          };
          reader.onerror = reject;
          reader.readAsDataURL(f);
        });
      };

      const base64 = await toBase64(file);
      
      // Set preview immediately
      setPhotoPreview(URL.createObjectURL(file));
      
      // Try to upload if online
      const isOnline = networkStatusService.getStatus() === 'online';
      
      if (isOnline) {
        try {
          const ext = file.type.includes('png') ? 'png' : file.type.includes('gif') ? 'gif' : 'jpg';
          const response = await apiPost<{ url: string }>('/upload', { base64, ext });
          setPhotoUrl(response.url);
          setPhotoPending(false);
          
          // Save to device on native platforms only
          if (Capacitor.isNativePlatform()) {
            const { Filesystem, Directory } = await import('@capacitor/filesystem');
            try {
              await Filesystem.writeFile({
                path: `flower_${Date.now()}.${ext}`,
                data: base64,
                directory: Directory.Data,
              });
            } catch (fsErr) {
              console.warn('Failed to save to device:', fsErr);
              // Don't throw - upload succeeded which is more important
            }
          }
        } catch (uploadErr: any) {
          console.warn('Upload failed, marking as pending:', uploadErr);
          setPhotoPending(true);
          setError('Photo will be uploaded when online');
        }
      } else {
        setPhotoPending(true);
        setError('Offline: Photo will be uploaded when online');
      }
    } catch (err: any) {
      setError('Failed to process photo: ' + err.message);
    } finally {
      setUploading(false);
    }
  };

  const handleFileInputChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (file) {
      handleUploadPhoto(file);
    }
  };

  const handleOpenMap = async () => {
    // Get current position if we don't have one yet
    if (!location) {
      try {
        const currentPos = await getCurrentPosition();
        if (currentPos) {
          setLocation(currentPos);
        }
      } catch (err) {
        console.warn('Could not get current position:', err);
        // Set default location (Cluj-Napoca) if geolocation fails
        setLocation({ lat: 46.7712, lng: 23.6236 });
      }
    }
    
    setShowMapPicker(true);
  };

  const handleSave = async (e: React.FormEvent) => {
    e.preventDefault();
    setSaving(true);
    setError('');

    try {
      const payload: Partial<Flower> = {
        name,
        species,
        category,
        colors: colors.split(',').map(s => s.trim()).filter(Boolean),
        price,
        status,
        description,
        photoUrl: photoUrl || undefined,
        location: location || undefined,
      };

      if (networkStatusService.getStatus() === 'offline') {
        await syncService.addToQueue(isNew ? 'create' : 'update', {
          ...payload,
          ...(isNew ? {} : { id }),
        });
        alert('Saved offline. Will sync when online.');
        history.goBack();
        return;
      }

      try {
        if (isNew) {
          await apiPost('/flowers', payload);
        } else {
          await apiPut(`/flowers/${id}`, payload);
        }
        // Small delay to ensure WebSocket message is received
        setTimeout(() => {
          history.goBack();
        }, 100);
      } catch (apiErr) {
        await syncService.addToQueue(isNew ? 'create' : 'update', {
          ...payload,
          ...(isNew ? {} : { id }),
        });
        alert('Could not save online. Added to outbox for later sync.');
        history.goBack();
      }
    } catch (err: any) {
      setError(err.message || 'Failed to save flower');
    } finally {
      setSaving(false);
    }
  };

  return (
    <IonPage>
      <IonHeader>
        <IonToolbar>
          <IonButtons slot="start">
            <IonBackButton defaultHref="/flowers" />
          </IonButtons>
          <IonTitle>{isNew ? 'New Flower' : 'Edit Flower'}</IonTitle>
        </IonToolbar>
      </IonHeader>

      <IonContent className="ion-padding">
        {loading ? (
          <div className="ion-text-center ion-padding"><IonSpinner /></div>
        ) : (
          <form onSubmit={handleSave}>
            <IonItem>
              <IonLabel position="stacked">Name *</IonLabel>
              <IonInput value={name} onIonInput={e => setName(e.detail.value!)} required disabled={saving} />
            </IonItem>

            <IonItem>
              <IonLabel position="stacked">Species</IonLabel>
              <IonInput value={species} onIonInput={e => setSpecies(e.detail.value!)} disabled={saving} />
            </IonItem>

            <IonItem>
              <IonLabel>Category</IonLabel>
              <IonSelect value={category} onIonChange={(e) => setCategory(e.detail.value)} disabled={saving}>
                {['Rose','Tulip','Lily','Lavender','Hydrangea','Daisy','Sunflower','Peony','Violet'].map(c => (
                  <IonSelectOption key={c} value={c}>{c}</IonSelectOption>
                ))}
              </IonSelect>
            </IonItem>

            <IonItem>
              <IonLabel position="stacked">Colors (comma-separated)</IonLabel>
              <IonInput value={colors} onIonInput={e => setColors(e.detail.value!)} disabled={saving} />
            </IonItem>

            <IonItem>
              <IonLabel position="stacked">Price</IonLabel>
              <IonInput 
                type="text" 
                value={price} 
                onIonInput={e => {
                  const value = e.detail.value!;
                  // Replace comma with dot for decimal separator
                  const normalized = value.replace(',', '.');
                  const parsed = parseFloat(normalized);
                  if (!isNaN(parsed)) {
                    setPrice(parsed);
                  } else if (value === '' || value === '0') {
                    setPrice(0);
                  }
                }} 
                disabled={saving} 
                placeholder="e.g., 15.99 or 15,99"
              />
            </IonItem>

            <IonItem>
              <IonLabel>Status</IonLabel>
              <IonSelect value={status} onIonChange={(e) => setStatus(e.detail.value)} disabled={saving}>
                <IonSelectOption value="active">Active</IonSelectOption>
                <IonSelectOption value="archived">Archived</IonSelectOption>
              </IonSelect>
            </IonItem>

            <IonItem>
              <IonLabel position="stacked">Description</IonLabel>
              <IonTextarea rows={6} value={description} onIonInput={e => setDescription(e.detail.value!)} disabled={saving} />
            </IonItem>

            <IonItem lines="none">
              <IonLabel>Photo</IonLabel>
              <IonButton onClick={() => fileInputRef.current?.click()} disabled={uploading || saving}>
                <IonIcon slot="start" icon={cloudUpload} />
                {uploading ? 'Uploading...' : 'Upload Photo'}
              </IonButton>
              {photoPending && (
                <IonText color="warning" style={{ marginLeft: '8px' }}>
                  <small>Pending upload</small>
                </IonText>
              )}
            </IonItem>
            
            {/* Hidden file input */}
            <input
              ref={fileInputRef}
              type="file"
              accept="image/*"
              style={{ display: 'none' }}
              onChange={handleFileInputChange}
            />

            {(photoPreview || photoUrl) && (
              <div className="ion-padding">
                <IonImg 
                  src={photoPreview || photoUrl} 
                  style={{ maxHeight: '200px', objectFit: 'cover', borderRadius: '12px' }} 
                />
              </div>
            )}

            <IonItem lines="none">
              <IonLabel>Location</IonLabel>
              <IonButton onClick={handleOpenMap} disabled={saving}>
                <IonIcon slot="start" icon={map} />
                {location ? 'Change Location' : 'Select Location'}
              </IonButton>
            </IonItem>

            {location && (
              <IonItem>
                <IonLabel>
                  <p>Lat: {location.lat.toFixed(6)}, Lng: {location.lng.toFixed(6)}</p>
                </IonLabel>
              </IonItem>
            )}

            {error && (
              <IonText color="danger"><p>{error}</p></IonText>
            )}

            <IonButton expand="block" type="submit" disabled={saving || !name} className="ion-margin-top">
              {saving ? <IonSpinner name="crescent" /> : 'Save'}
            </IonButton>
          </form>
        )}
        
        <MapPicker
          isOpen={showMapPicker}
          onDismiss={() => setShowMapPicker(false)}
          onLocationSelect={setLocation}
          initialLocation={location}
        />
      </IonContent>
    </IonPage>
  );
};

export default FlowerEdit;

