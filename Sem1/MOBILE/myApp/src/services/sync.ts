import { getOutbox, removeFromOutbox, updateOutboxItem, addToOutbox } from './storage';
import { apiPost, apiPut, ApiError } from './api';
import { networkStatusService } from './network';
import { Flower, OutboxItem } from '../types';

class SyncService {
  private isSyncing = false;
  private syncListeners: Array<() => void> = [];

  constructor() {
    // Listen for network status changes
    networkStatusService.subscribe((status) => {
      if (status === 'online') {
        this.sync();
      }
    });
  }

  /**
   * Add item to outbox for later syncing
   */
  async addToQueue(type: 'create' | 'update', item: Partial<Flower>): Promise<void> {
    const outboxItem: OutboxItem = {
      id: `outbox_${Date.now()}_${Math.random()}`,
      type,
      item,
      timestamp: Date.now(),
      retries: 0
    };

    await addToOutbox(outboxItem);
    this.notifySyncListeners();

    // Try to sync immediately if online
    if (networkStatusService.getStatus() === 'online') {
      this.sync();
    }
  }

  /**
   * Sync all items in the outbox
   */
  async sync(): Promise<void> {
    if (this.isSyncing) {
      return;
    }

    if (networkStatusService.getStatus() === 'offline') {
      return;
    }

    this.isSyncing = true;

    try {
      const outbox = await getOutbox();

      for (const outboxItem of outbox) {
        try {
          if (outboxItem.type === 'create') {
            await apiPost('/flowers', outboxItem.item);
          } else if (outboxItem.type === 'update' && outboxItem.item.id) {
            await apiPut(`/flowers/${outboxItem.item.id}`, outboxItem.item);
          }

          // Success - remove from outbox
          await removeFromOutbox(outboxItem.id);
          this.notifySyncListeners();
        } catch (error) {
          // Update retry count
          await updateOutboxItem(outboxItem.id, {
            retries: outboxItem.retries + 1
          });

          // If max retries reached, you might want to handle it differently
          if (outboxItem.retries >= 3) {
            console.error('Max retries reached for outbox item:', outboxItem);
            // Optionally remove it or mark as failed
          }
        }
      }
    } finally {
      this.isSyncing = false;
    }
  }

  /**
   * Get pending items count
   */
  async getPendingCount(): Promise<number> {
    const outbox = await getOutbox();
    return outbox.length;
  }

  /**
   * Subscribe to sync updates
   */
  onSyncUpdate(callback: () => void): () => void {
    this.syncListeners.push(callback);
    
    return () => {
      this.syncListeners = this.syncListeners.filter(cb => cb !== callback);
    };
  }

  private notifySyncListeners(): void {
    this.syncListeners.forEach(cb => cb());
  }
}

export const syncService = new SyncService();

