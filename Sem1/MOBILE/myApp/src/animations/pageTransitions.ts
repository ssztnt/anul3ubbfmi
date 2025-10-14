import { createAnimation, type AnimationBuilder } from '@ionic/react';

/**
 * Custom page transition animation
 * Slides in from right with fade for entering page
 * Slides out to left with fade for leaving page
 */
export const customPageTransition: AnimationBuilder = (baseEl: HTMLElement, opts?: any) => {
  const DURATION = 300;
  
  const rootAnimation = createAnimation()
    .duration(DURATION)
    .easing('cubic-bezier(0.4, 0.0, 0.2, 1)');

  const enteringAnimation = createAnimation()
    .addElement(opts.enteringEl)
    .fromTo('opacity', '0', '1')
    .fromTo('transform', 'translateX(100%)', 'translateX(0)');

  const leavingAnimation = createAnimation()
    .addElement(opts.leavingEl)
    .fromTo('opacity', '1', '0.8')
    .fromTo('transform', 'translateX(0)', 'translateX(-20%)');

  rootAnimation.addAnimation([enteringAnimation, leavingAnimation]);

  return rootAnimation;
};

/**
 * Fade transition (used for modals, alerts, etc.)
 */
export const fadeTransition: AnimationBuilder = (baseEl: HTMLElement, opts?: any) => {
  const DURATION = 250;
  
  const rootAnimation = createAnimation()
    .duration(DURATION)
    .easing('ease-in-out');

  const enteringAnimation = createAnimation()
    .addElement(opts.enteringEl)
    .fromTo('opacity', '0', '1');

  const leavingAnimation = createAnimation()
    .addElement(opts.leavingEl)
    .fromTo('opacity', '1', '0');

  rootAnimation.addAnimation([enteringAnimation, leavingAnimation]);

  return rootAnimation;
};

/**
 * Scale and fade animation for cards
 */
export const scaleInAnimation = (element: HTMLElement) => {
  const animation = createAnimation()
    .addElement(element)
    .duration(400)
    .easing('cubic-bezier(0.34, 1.56, 0.64, 1)')
    .fromTo('transform', 'scale(0.8)', 'scale(1)')
    .fromTo('opacity', '0', '1');

  animation.play();
};

/**
 * Stagger animation for list items
 */
export const staggerListAnimation = (elements: HTMLElement[]) => {
  elements.forEach((el, index) => {
    const animation = createAnimation()
      .addElement(el)
      .duration(400)
      .delay(index * 50)
      .easing('cubic-bezier(0.25, 0.46, 0.45, 0.94)')
      .fromTo('transform', 'translateY(20px)', 'translateY(0)')
      .fromTo('opacity', '0', '1');

    animation.play();
  });
};

