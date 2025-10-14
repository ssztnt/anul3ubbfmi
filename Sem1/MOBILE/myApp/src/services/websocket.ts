import { io, Socket } from 'socket.io-client';
import { getToken } from './storage';
import { Flower } from '../types';

const WS_BASE_URL = 'http://localhost:3000';

class WebSocketService {
  private socket: Socket | null = null;
  private listeners: Map<string, Array<(data: any) => void>> = new Map();

  /**
   * Connect to WebSocket with authentication
   */
  async connect(): Promise<void> {
    if (this.socket?.connected) {
      return;
    }

    const token = await getToken();
    
    if (!token) {
      console.warn('Cannot connect to WebSocket: no auth token');
      return;
    }

    this.socket = io(WS_BASE_URL, {
      auth: {
        token
      }
    });

    this.socket.on('connect', () => {
      console.log('✅ WebSocket connected');
    });

    this.socket.on('disconnect', () => {
      console.log('❌ WebSocket disconnected');
    });

    this.socket.on('flower:created', (data: { flower: Flower }) => {
      this.notifyListeners('flower:created', data);
    });

    this.socket.on('flower:updated', (data: { flower: Flower }) => {
      this.notifyListeners('flower:updated', data);
    });
  }

  /**
   * Disconnect from WebSocket
   */
  disconnect(): void {
    if (this.socket) {
      this.socket.disconnect();
      this.socket = null;
    }
  }

  /**
   * Subscribe to WebSocket events
   */
  on(event: string, callback: (data: any) => void): () => void {
    if (!this.listeners.has(event)) {
      this.listeners.set(event, []);
    }
    
    this.listeners.get(event)!.push(callback);

    // Return unsubscribe function
    return () => {
      const callbacks = this.listeners.get(event);
      if (callbacks) {
        const index = callbacks.indexOf(callback);
        if (index !== -1) {
          callbacks.splice(index, 1);
        }
      }
    };
  }

  private notifyListeners(event: string, data: any): void {
    const callbacks = this.listeners.get(event);
    if (callbacks) {
      callbacks.forEach(callback => callback(data));
    }
  }

  /**
   * Check if connected
   */
  isConnected(): boolean {
    return this.socket?.connected || false;
  }
}

export const webSocketService = new WebSocketService();

