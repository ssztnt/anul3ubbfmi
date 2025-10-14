import { Network } from '@capacitor/network';
import { Capacitor } from '@capacitor/core';

export type NetworkStatus = 'online' | 'offline';

class NetworkStatusService {
  private listeners: Array<(status: NetworkStatus) => void> = [];
  private currentStatus: NetworkStatus = 'online';
  private isCapacitor = Capacitor.isNativePlatform();

  constructor() {
    this.init();
  }

  private async init() {
    // Initial status
    this.currentStatus = await this.checkStatus();

    if (this.isCapacitor) {
      // Use Capacitor Network plugin
      Network.addListener('networkStatusChange', (status) => {
        const newStatus: NetworkStatus = status.connected ? 'online' : 'offline';
        this.updateStatus(newStatus);
      });
    } else {
      // Use browser APIs
      window.addEventListener('online', () => this.updateStatus('online'));
      window.addEventListener('offline', () => this.updateStatus('offline'));
    }
  }

  private async checkStatus(): Promise<NetworkStatus> {
    if (this.isCapacitor) {
      const status = await Network.getStatus();
      return status.connected ? 'online' : 'offline';
    } else {
      return navigator.onLine ? 'online' : 'offline';
    }
  }

  private updateStatus(status: NetworkStatus) {
    if (this.currentStatus !== status) {
      this.currentStatus = status;
      this.notifyListeners(status);
    }
  }

  private notifyListeners(status: NetworkStatus) {
    this.listeners.forEach(listener => listener(status));
  }

  public getStatus(): NetworkStatus {
    return this.currentStatus;
  }

  public subscribe(listener: (status: NetworkStatus) => void): () => void {
    this.listeners.push(listener);
    
    // Return unsubscribe function
    return () => {
      this.listeners = this.listeners.filter(l => l !== listener);
    };
  }
}

export const networkStatusService = new NetworkStatusService();

