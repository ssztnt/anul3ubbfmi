import React, { useEffect, useState } from 'react';
import { Redirect, Route } from 'react-router-dom';
import { IonApp, IonRouterOutlet, IonSpinner, setupIonicReact } from '@ionic/react';
import { IonReactRouter } from '@ionic/react-router';
import Login from './pages/Login';
import FlowersList from './pages/FlowersList';
import FlowerDetail from './pages/FlowerDetail';
import FlowerEdit from './pages/FlowerEdit';
import { authService } from './services/auth';
import { customPageTransition } from './animations/pageTransitions';

/* Core CSS required for Ionic components to work properly */
import '@ionic/react/css/core.css';

/* Basic CSS for apps built with Ionic */
import '@ionic/react/css/normalize.css';
import '@ionic/react/css/structure.css';
import '@ionic/react/css/typography.css';

/* Optional CSS utils that can be commented out */
import '@ionic/react/css/padding.css';
import '@ionic/react/css/float-elements.css';
import '@ionic/react/css/text-alignment.css';
import '@ionic/react/css/text-transformation.css';
import '@ionic/react/css/flex-utils.css';
import '@ionic/react/css/display.css';

/* Theme variables */
import './theme/variables.css';
import './theme/flowers.css';

setupIonicReact({
  animated: true,
  navAnimation: customPageTransition, // Custom page transition
});

const App: React.FC = () => {
  const [isAuthenticated, setIsAuthenticated] = useState<boolean | null>(null);

  useEffect(() => {
    // Silent login check
    authService.silentLogin().then((user) => {
      setIsAuthenticated(user !== null);
    });
  }, []);

  if (isAuthenticated === null) {
    // Still checking authentication
    return (
      <IonApp>
        <div style={{ 
          display: 'flex', 
          justifyContent: 'center', 
          alignItems: 'center', 
          height: '100vh' 
        }}>
          <IonSpinner />
        </div>
      </IonApp>
    );
  }

  return (
    <IonApp>
      <IonReactRouter>
        <IonRouterOutlet>
          <Route path="/login" exact>
            {isAuthenticated ? <Redirect to="/flowers" /> : <Login />}
          </Route>
          
          <Route path="/flowers" exact>
            {isAuthenticated ? <FlowersList /> : <Redirect to="/login" />}
          </Route>
          
          <Route path="/flowers/new" exact>
            {isAuthenticated ? <FlowerEdit /> : <Redirect to="/login" />}
          </Route>
          
          <Route path="/flowers/:id" exact>
            {isAuthenticated ? <FlowerDetail /> : <Redirect to="/login" />}
          </Route>
          
          <Route path="/flowers/:id/edit" exact>
            {isAuthenticated ? <FlowerEdit /> : <Redirect to="/login" />}
          </Route>
          
          <Route path="/" exact>
            <Redirect to={isAuthenticated ? "/flowers" : "/login"} />
          </Route>
        </IonRouterOutlet>
      </IonReactRouter>
    </IonApp>
  );
};

export default App;
