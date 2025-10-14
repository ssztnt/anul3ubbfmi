import React, { useEffect, useState } from 'react';
import {
  IonBadge,
  IonChip,
  IonIcon,
  IonLabel,
} from '@ionic/react';
import { cloudOfflineOutline, cloudDoneOutline, hourglassOutline } from 'ionicons/icons';
import { networkStatusService, NetworkStatus } from '../services/network';
import { syncService } from '../services/sync';

const NetworkStatusIndicator: React.FC = () => {
  const [networkStatus, setNetworkStatus] = useState<NetworkStatus>('online');
  const [pendingCount, setPendingCount] = useState(0);

  useEffect(() => {
    // Subscribe to network status
    const unsubscribeNetwork = networkStatusService.subscribe(setNetworkStatus);

    // Subscribe to sync updates
    const unsubscribeSync = syncService.onSyncUpdate(async () => {
      const count = await syncService.getPendingCount();
      setPendingCount(count);
    });

    // Initial pending count
    syncService.getPendingCount().then(setPendingCount);

    return () => {
      unsubscribeNetwork();
      unsubscribeSync();
    };
  }, []);

  return (
    <div style={{ display: 'flex', gap: '8px', alignItems: 'center' }}>
      <IonChip color={networkStatus === 'online' ? 'success' : 'danger'}>
        <IonIcon icon={networkStatus === 'online' ? cloudDoneOutline : cloudOfflineOutline} />
        <IonLabel>{networkStatus === 'online' ? 'Online' : 'Offline'}</IonLabel>
      </IonChip>

      {pendingCount > 0 && (
        <IonChip color="warning">
          <IonIcon icon={hourglassOutline} />
          <IonLabel>{pendingCount} pending</IonLabel>
        </IonChip>
      )}
    </div>
  );
};

export default NetworkStatusIndicator;

