import React, { useEffect, useState, useRef } from 'react';
import {
  IonBadge,
  IonButton,
  IonButtons,
  IonChip,
  IonContent,
  IonHeader,
  IonIcon,
  IonInfiniteScroll,
  IonInfiniteScrollContent,
  IonPage,
  IonRefresher,
  IonRefresherContent,
  IonSearchbar,
  IonSegment,
  IonSegmentButton,
  IonLabel,
  IonTitle,
  IonToolbar,
  IonText,
  IonSpinner,
} from '@ionic/react';
import { addOutline, logOutOutline } from 'ionicons/icons';
import { useHistory } from 'react-router-dom';
import { Flower, PaginatedResponse } from '../types';
import { apiGet } from '../services/api';
import { authService } from '../services/auth';
import { webSocketService } from '../services/websocket';
import { networkStatusService } from '../services/network';
import {
  cacheItemsPage,
  getCachedItemsPage,
  clearItemsCache,
  saveSearchState,
  getSearchState,
} from '../services/storage';
import NetworkStatusIndicator from '../components/NetworkStatusIndicator';
import { staggerListAnimation } from '../animations/pageTransitions';
import '../theme/flowers.css';

const CATEGORIES = ['all','Rose','Tulip','Lily','Lavender','Hydrangea','Daisy','Sunflower','Peony','Violet'];

const FlowersList: React.FC = () => {
  const [flowers, setFlowers] = useState<Flower[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');
  const [page, setPage] = useState(1);
  const [hasMore, setHasMore] = useState(true);
  const [query, setQuery] = useState('');
  const [status, setStatus] = useState<'all'|'active'|'archived'>('all');
  const [category, setCategory] = useState<string>('all');
  const history = useHistory();
  const cardsRef = useRef<HTMLDivElement[]>([]);

  useEffect(() => {
    getSearchState().then((state) => {
      if (state) {
        setQuery(state.query);
        setStatus((state.status as any) || 'all');
        if (state.category) setCategory(state.category);
      }
    });
  }, []);

  useEffect(() => {
    saveSearchState({ query, status, category });
  }, [query, status, category]);

  const fetchFlowers = async (pageNum: number, append = false) => {
    try {
      setError('');

      if (networkStatusService.getStatus() === 'offline') {
        const cached = await getCachedItemsPage(pageNum, query, status, category);
        if (cached) {
          if (append) setFlowers((prev) => [...prev, ...cached.items]);
          else setFlowers(cached.items);
          setHasMore(cached.hasMore);
          setLoading(false);
          return;
        }
      }

      const params = new URLSearchParams({
        page: pageNum.toString(),
        pageSize: '10',
        q: query,
        status: status === 'all' ? '' : status,
        category: category === 'all' ? '' : category,
      });

      const response = await apiGet<PaginatedResponse>(`/flowers?${params}`);
      await cacheItemsPage(pageNum, query, status, response, category);

      if (append) setFlowers((prev) => [...prev, ...response.items]);
      else setFlowers(response.items);
      setHasMore(response.hasMore);
      
      // Animate cards when not appending
      if (!append) {
        setTimeout(() => {
          if (cardsRef.current.length > 0) {
            staggerListAnimation(cardsRef.current.filter(Boolean));
          }
        }, 50);
      }
    } catch (err: any) {
      setError(err.message || 'Failed to load flowers');
      const cached = await getCachedItemsPage(pageNum, query, status, category);
      if (cached && !append) {
        setFlowers(cached.items);
        setHasMore(cached.hasMore);
      }
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    setLoading(true);
    setPage(1);
    fetchFlowers(1);
  }, [query, status, category]);

  useEffect(() => {
    webSocketService.connect();

    const unsubCreated = webSocketService.on('flower:created', ({ flower }: { flower: Flower }) => {
      setFlowers((prev) => [flower, ...prev]);
      clearItemsCache();
    });

    const unsubUpdated = webSocketService.on('flower:updated', ({ flower }: { flower: Flower }) => {
      setFlowers((prev) => prev.map(f => f.id === flower.id ? flower : f));
      clearItemsCache();
    });

    return () => { unsubCreated(); unsubUpdated(); };
  }, []);

  const handleRefresh = async (event: CustomEvent) => {
    await clearItemsCache();
    setPage(1);
    await fetchFlowers(1);
    event.detail.complete();
  };

  const loadMore = async (event: CustomEvent) => {
    const next = page + 1;
    setPage(next);
    await fetchFlowers(next, true);
    (event.target as HTMLIonInfiniteScrollElement).complete();
  };

  const handleLogout = async () => {
    await authService.logout();
    webSocketService.disconnect();
    history.replace('/login');
  };

  return (
    <IonPage>
      <IonHeader>
        <IonToolbar>
          <IonButtons slot="start">
            <NetworkStatusIndicator />
          </IonButtons>
          <IonTitle className="large-title">Flowers</IonTitle>
          <IonButtons slot="end">
            <IonButton onClick={() => history.push('/flowers/new')}>
              <IonIcon icon={addOutline} />
            </IonButton>
            <IonButton onClick={handleLogout}>
              <IonIcon icon={logOutOutline} />
            </IonButton>
          </IonButtons>
        </IonToolbar>
      </IonHeader>

      <IonContent>
        <IonRefresher slot="fixed" onIonRefresh={handleRefresh}>
          <IonRefresherContent />
        </IonRefresher>

        <div className="ion-padding-horizontal">
          <IonSearchbar
            value={query}
            onIonInput={(e) => setQuery(e.detail.value!)}
            debounce={400}
            placeholder="Search flowers..."
          />
        </div>

        <div className="filters-row">
          {CATEGORIES.map(cat => (
            <IonChip key={cat} outline color={cat === category ? 'primary' : 'medium'} onClick={() => setCategory(cat)}>
              <IonLabel>{cat}</IonLabel>
            </IonChip>
          ))}
        </div>

        <div className="ion-padding-horizontal">
          <IonSegment value={status} onIonChange={(e) => setStatus(e.detail.value as any)}>
            <IonSegmentButton value="all">
              <IonLabel>All</IonLabel>
            </IonSegmentButton>
            <IonSegmentButton value="active">
              <IonLabel>Active</IonLabel>
            </IonSegmentButton>
            <IonSegmentButton value="archived">
              <IonLabel>Archived</IonLabel>
            </IonSegmentButton>
          </IonSegment>
        </div>

        {loading && flowers.length === 0 ? (
          <div className="ion-text-center ion-padding">
            <IonSpinner />
          </div>
        ) : error && flowers.length === 0 ? (
          <div className="ion-padding">
            <IonText color="danger">
              <p>{error}</p>
            </IonText>
          </div>
        ) : (
          <>
            <div className="ion-padding-horizontal ion-padding-top">
              <div className="ion-grid">
                <div className="ion-row">
                  {flowers.map((f, idx) => (
                    <div 
                      key={f.id} 
                      className="ion-col-12 ion-col-md-6"
                      ref={(el) => {
                        if (el) cardsRef.current[idx] = el;
                      }}
                    >
                      <div className="bubble-card" onClick={() => history.push(`/flowers/${f.id}`)}>
                        {f.photoUrl ? (
                          <img src={f.photoUrl} alt={f.name} className="bubble-image" />
                        ) : f.imageUrl ? (
                          <img src={f.imageUrl} alt={f.name} className="bubble-image" />
                        ) : (
                          <div className="bubble-image" style={{background: 'linear-gradient(135deg, var(--flr-rose), var(--flr-lav))'}} />
                        )}
                        <div className="bubble-body">
                          <div className="ion-text-wrap bubble-title">{f.name}</div>
                          <div className="bubble-sub">{f.species}</div>
                          <div className="bubble-row">
                            {f.colors.map((c, idx) => (
                              <span key={idx} className="color-chip" style={{ background: c }} />
                            ))}
                            <IonBadge className="price" color="success">${f.price.toFixed(2)}</IonBadge>
                          </div>
                        </div>
                      </div>
                    </div>
                  ))}
                </div>
              </div>
            </div>

            <IonInfiniteScroll onIonInfinite={loadMore} threshold="100px" disabled={!hasMore}>
              <IonInfiniteScrollContent loadingSpinner="bubbles" loadingText="Loading more..." />
            </IonInfiniteScroll>
          </>
        )}
      </IonContent>
    </IonPage>
  );
};

export default FlowersList;

