import localforage from 'localforage';
import { Flower, OutboxItem, PaginatedResponse } from '../types';

// Configure localforage
localforage.config({
  name: 'GreenLeafApp',
  storeName: 'greenleaf_store',
  description: 'Storage for GreenLeaf app data'
});

// ============================================================================
// AUTH TOKEN
// ============================================================================

const TOKEN_KEY = 'auth_token';

export async function saveToken(token: string): Promise<void> {
  await localforage.setItem(TOKEN_KEY, token);
}

export async function getToken(): Promise<string | null> {
  return await localforage.getItem<string>(TOKEN_KEY);
}

export async function removeToken(): Promise<void> {
  await localforage.removeItem(TOKEN_KEY);
}

// ============================================================================
// CACHED ITEMS (keyed by query params + page)
// ============================================================================

function getCacheKey(page: number, query: string, status: string, category: string = ''): string {
  return `flowers_page_${page}_q_${query}_status_${status}_cat_${category}`;
}

export async function cacheItemsPage(
  page: number,
  query: string,
  status: string,
  data: PaginatedResponse,
  category: string = ''
): Promise<void> {
  const key = getCacheKey(page, query, status, category);
  await localforage.setItem(key, {
    data,
    timestamp: Date.now()
  });
}

export async function getCachedItemsPage(
  page: number,
  query: string,
  status: string,
  category: string = ''
): Promise<PaginatedResponse | null> {
  const key = getCacheKey(page, query, status, category);
  const cached = await localforage.getItem<{ data: PaginatedResponse; timestamp: number }>(key);
  
  if (!cached) return null;
  
  // Cache expires after 5 minutes
  const CACHE_TTL = 5 * 60 * 1000;
  if (Date.now() - cached.timestamp > CACHE_TTL) {
    await localforage.removeItem(key);
    return null;
  }
  
  return cached.data;
}

export async function clearItemsCache(): Promise<void> {
  const keys = await localforage.keys();
  const itemKeys = keys.filter(key => key.startsWith('flowers_page_'));
  await Promise.all(itemKeys.map(key => localforage.removeItem(key)));
}

// ============================================================================
// SEARCH/FILTER STATE
// ============================================================================

const SEARCH_STATE_KEY = 'search_state';

export interface SearchState {
  query: string;
  status: string;
  category?: string;
}

export async function saveSearchState(state: SearchState): Promise<void> {
  await localforage.setItem(SEARCH_STATE_KEY, state);
}

export async function getSearchState(): Promise<SearchState | null> {
  return await localforage.getItem<SearchState>(SEARCH_STATE_KEY);
}

// ============================================================================
// OUTBOX QUEUE (pending creates/updates)
// ============================================================================

const OUTBOX_KEY = 'outbox';

export async function getOutbox(): Promise<OutboxItem[]> {
  const outbox = await localforage.getItem<OutboxItem[]>(OUTBOX_KEY);
  return outbox || [];
}

export async function addToOutbox(item: OutboxItem): Promise<void> {
  const outbox = await getOutbox();
  outbox.push(item);
  await localforage.setItem(OUTBOX_KEY, outbox);
}

export async function removeFromOutbox(itemId: string): Promise<void> {
  const outbox = await getOutbox();
  const filtered = outbox.filter(i => i.id !== itemId);
  await localforage.setItem(OUTBOX_KEY, filtered);
}

export async function clearOutbox(): Promise<void> {
  await localforage.setItem(OUTBOX_KEY, []);
}

export async function updateOutboxItem(itemId: string, updates: Partial<OutboxItem>): Promise<void> {
  const outbox = await getOutbox();
  const index = outbox.findIndex(i => i.id === itemId);
  if (index !== -1) {
    outbox[index] = { ...outbox[index], ...updates };
    await localforage.setItem(OUTBOX_KEY, outbox);
  }
}

// ============================================================================
// INDIVIDUAL FLOWER CACHE (for offline editing)
// ============================================================================

function getFlowerCacheKey(id: string): string {
  return `flower_detail_${id}`;
}

export async function cacheFlowerDetail(flower: Flower): Promise<void> {
  const key = getFlowerCacheKey(flower.id);
  await localforage.setItem(key, {
    data: flower,
    timestamp: Date.now()
  });
}

export async function getCachedFlowerDetail(id: string): Promise<Flower | null> {
  const key = getFlowerCacheKey(id);
  const cached = await localforage.getItem<{ data: Flower; timestamp: number }>(key);
  
  if (!cached) return null;
  
  // Cache expires after 30 minutes
  const CACHE_TTL = 30 * 60 * 1000;
  if (Date.now() - cached.timestamp > CACHE_TTL) {
    await localforage.removeItem(key);
    return null;
  }
  
  return cached.data;
}

export async function removeCachedFlowerDetail(id: string): Promise<void> {
  const key = getFlowerCacheKey(id);
  await localforage.removeItem(key);
}

export async function clearFlowerDetailsCache(): Promise<void> {
  const keys = await localforage.keys();
  const flowerKeys = keys.filter(key => key.startsWith('flower_detail_'));
  await Promise.all(flowerKeys.map(key => localforage.removeItem(key)));
}

