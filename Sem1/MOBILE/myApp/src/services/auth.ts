import { apiPost } from './api';
import { saveToken, getToken, removeToken } from './storage';
import { AuthResponse, User } from '../types';

class AuthService {
  private currentUser: User | null = null;

  /**
   * Login with email and password
   */
  async login(email: string, password: string): Promise<User> {
    const response = await apiPost<AuthResponse>('/auth/login', { email, password });
    
    await saveToken(response.token);
    this.currentUser = response.user;
    
    return response.user;
  }

  /**
   * Silent login - check if we have a valid token
   */
  async silentLogin(): Promise<User | null> {
    const token = await getToken();
    
    if (!token) {
      return null;
    }

    // Try to verify the token by making a test request
    try {
      // We'll just try to fetch items to verify the token is valid
      const { apiGet } = await import('./api');
      await apiGet('/flowers?page=1&pageSize=1');
      
      // Token is valid, decode it to get user info
      // In a real app, you might want to store user info separately
      // For now, we'll just return a placeholder
      // The token is valid, so we can proceed
      return { id: 'user', email: '', name: 'User' };
    } catch (error) {
      // Token is invalid
      await removeToken();
      return null;
    }
  }

  /**
   * Logout
   */
  async logout(): Promise<void> {
    await removeToken();
    this.currentUser = null;
  }

  /**
   * Get current user
   */
  getCurrentUser(): User | null {
    return this.currentUser;
  }

  /**
   * Check if user is authenticated
   */
  async isAuthenticated(): Promise<boolean> {
    const token = await getToken();
    return token !== null;
  }
}

export const authService = new AuthService();

