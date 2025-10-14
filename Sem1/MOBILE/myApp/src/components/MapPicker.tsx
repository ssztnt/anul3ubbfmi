import React, { useEffect, useRef, useState } from 'react';
import { IonButton, IonButtons, IonContent, IonHeader, IonModal, IonTitle, IonToolbar } from '@ionic/react';
import L from 'leaflet';
import 'leaflet/dist/leaflet.css';

// Fix Leaflet default marker icons
delete (L.Icon.Default.prototype as any)._getIconUrl;
L.Icon.Default.mergeOptions({
  iconRetinaUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon-2x.png',
  iconUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon.png',
  shadowUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-shadow.png',
});

interface MapPickerProps {
  isOpen: boolean;
  onDismiss: () => void;
  onLocationSelect: (location: { lat: number; lng: number }) => void;
  initialLocation?: { lat: number; lng: number };
  viewOnly?: boolean;
}

const MapPicker: React.FC<MapPickerProps> = ({ isOpen, onDismiss, onLocationSelect, initialLocation, viewOnly = false }) => {
  const mapRef = useRef<L.Map | null>(null);
  const markerRef = useRef<L.Marker | null>(null);
  const [selectedLocation, setSelectedLocation] = useState<{ lat: number; lng: number } | null>(initialLocation || null);

  useEffect(() => {
    if (isOpen && !mapRef.current) {
      setTimeout(() => {
        const mapElement = document.getElementById('map');
        if (mapElement && !mapRef.current) {
          const defaultLocation = initialLocation || { lat: 46.7712, lng: 23.6236 }; // Cluj-Napoca
          
          const map = L.map('map').setView([defaultLocation.lat, defaultLocation.lng], 13);
          
          L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            attribution: '© OpenStreetMap contributors'
          }).addTo(map);

          if (initialLocation) {
            markerRef.current = L.marker([initialLocation.lat, initialLocation.lng]).addTo(map);
            setSelectedLocation(initialLocation);
          }

          if (!viewOnly) {
            map.on('click', (e: L.LeafletMouseEvent) => {
              const { lat, lng } = e.latlng;
              
              if (markerRef.current) {
                markerRef.current.setLatLng([lat, lng]);
              } else {
                markerRef.current = L.marker([lat, lng]).addTo(map);
              }
              
              setSelectedLocation({ lat, lng });
            });
          }

          mapRef.current = map;
        }
      }, 100);
    }

    return () => {
      if (mapRef.current) {
        mapRef.current.remove();
        mapRef.current = null;
        markerRef.current = null;
      }
    };
  }, [isOpen, initialLocation, viewOnly]);

  const handleConfirm = () => {
    if (selectedLocation) {
      onLocationSelect(selectedLocation);
    }
    onDismiss();
  };

  return (
    <IonModal isOpen={isOpen} onDidDismiss={onDismiss}>
      <IonHeader>
        <IonToolbar>
          <IonTitle>{viewOnly ? 'Flower Location' : 'Select Location'}</IonTitle>
          <IonButtons slot="end">
            <IonButton onClick={onDismiss}>Close</IonButton>
            {!viewOnly && (
              <IonButton strong onClick={handleConfirm} disabled={!selectedLocation}>
                Confirm
              </IonButton>
            )}
          </IonButtons>
        </IonToolbar>
      </IonHeader>
      <IonContent>
        <div id="map" style={{ height: '100%', width: '100%' }}></div>
      </IonContent>
    </IonModal>
  );
};

export default MapPicker;

