import React, { useEffect, useState } from 'react';
import {
  IonBackButton,
  IonBadge,
  IonButtons,
  IonContent,
  IonHeader,
  IonIcon,
  IonLabel,
  IonPage,
  IonText,
  IonTitle,
  IonToolbar,
  IonButton,
  IonSpinner,
} from '@ionic/react';
import { createOutline, map as mapIcon } from 'ionicons/icons';
import { useParams, useHistory } from 'react-router-dom';
import { Flower } from '../types';
import { apiGet } from '../services/api';
import MapPicker from '../components/MapPicker';
import '../theme/flowers.css';

const FlowerDetail: React.FC = () => {
  const { id } = useParams<{ id: string }>();
  const [flower, setFlower] = useState<Flower | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');
  const [showMap, setShowMap] = useState(false);
  const history = useHistory();

  useEffect(() => {
    const fetchFlower = async () => {
      try {
        const data = await apiGet<Flower>(`/flowers/${id}`);
        setFlower(data);
      } catch (err: any) {
        setError(err.message || 'Failed to load flower');
      } finally {
        setLoading(false);
      }
    };
    fetchFlower();
  }, [id]);

  return (
    <IonPage>
      <IonHeader>
        <IonToolbar>
          <IonButtons slot="start">
            <IonBackButton defaultHref="/flowers" />
          </IonButtons>
          <IonTitle>Flower</IonTitle>
          <IonButtons slot="end">
            {flower && (
              <IonButton onClick={() => history.push(`/flowers/${id}/edit`)}>
                <IonIcon icon={createOutline} />
              </IonButton>
            )}
          </IonButtons>
        </IonToolbar>
      </IonHeader>

      <IonContent className="ion-padding">
        {loading ? (
          <div className="ion-text-center ion-padding"><IonSpinner /></div>
        ) : error ? (
          <IonText color="danger"><p>{error}</p></IonText>
        ) : flower ? (
          <>
            <div className="bubble-card">
              {flower.photoUrl ? (
                <img src={flower.photoUrl} alt={flower.name} className="bubble-image" />
              ) : flower.imageUrl ? (
                <img src={flower.imageUrl} alt={flower.name} className="bubble-image" />
              ) : (
                <div className="bubble-image" style={{background: 'linear-gradient(135deg, var(--flr-leaf), var(--flr-tint))'}} />
              )}
              <div className="bubble-body">
                <div className="bubble-title">{flower.name}</div>
                <div className="bubble-sub">{flower.species} • {flower.category}</div>
                <div className="bubble-row">
                  {flower.colors.map((c, idx) => (
                    <span key={idx} className="color-chip" style={{ background: c }} />
                  ))}
                  <IonBadge className="price" color="success">${flower.price.toFixed(2)}</IonBadge>
                </div>
                {flower.description && (
                  <div className="ion-padding-top">
                    <IonText color="medium"><p>{flower.description}</p></IonText>
                  </div>
                )}
                {flower.location && (
                  <div className="ion-padding-top">
                    <IonButton size="small" onClick={() => setShowMap(true)}>
                      <IonIcon slot="start" icon={mapIcon} />
                      View on Map
                    </IonButton>
                  </div>
                )}
                <div className="ion-padding-top">
                  <IonText color="medium"><small>Last updated: {new Date(flower.updatedAt).toLocaleString()}</small></IonText>
                </div>
              </div>
            </div>
          </>
        ) : (
          <IonText><p>Flower not found</p></IonText>
        )}
        
        {flower?.location && (
          <MapPicker
            isOpen={showMap}
            onDismiss={() => setShowMap(false)}
            onLocationSelect={() => {}}
            initialLocation={flower.location}
            viewOnly={true}
          />
        )}
      </IonContent>
    </IonPage>
  );
};

export default FlowerDetail;

