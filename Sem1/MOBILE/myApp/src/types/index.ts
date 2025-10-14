export interface User {
  id: string;
  email: string;
  name: string;
}

export interface AuthResponse {
  token: string;
  user: User;
}

export interface Flower {
  id: string;
  userId: string;
  name: string;
  species: string;
  category: string;
  colors: string[];
  price: number;
  status: 'active' | 'archived';
  updatedAt: number;
  description?: string;
  imageUrl?: string;
  photoUrl?: string;
  location?: {
    lat: number;
    lng: number;
  };
}

export interface PaginatedResponse {
  items: Flower[];
  page: number;
  pageSize: number;
  total: number;
  hasMore: boolean;
}

export interface OutboxItem {
  id: string;
  type: 'create' | 'update';
  item: Partial<Flower>;
  timestamp: number;
  retries: number;
}

