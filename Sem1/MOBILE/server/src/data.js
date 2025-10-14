/**
 * In-memory data store
 */

/** @type {import('./types.js').User[]} */
export const users = [
  {
    id: 'u1',
    email: 'alice@example.com',
    name: 'Alice',
    password: 'alice123'
  },
  {
    id: 'u2',
    email: 'bob@example.com',
    name: 'Bob',
    password: 'bob123'
  }
];

/** @type {import('./types.js').Flower[]} */
export const flowers = [];

let flowerIdCounter = 1;

/**
 * Seed initial data
 */
export function seedData() {
  const catalog = [
    { 
      name: 'Crimson Queen', 
      species: 'Rosa rubiginosa', 
      category: 'Rose', 
      colors: ['red'], 
      price: 19.99,
      imageUrl: 'https://images.unsplash.com/photo-1518895949257-7621c3c786d7?w=400&h=300&fit=crop'
    },
    { 
      name: 'Snow Petal', 
      species: 'Rosa alba', 
      category: 'Rose', 
      colors: ['white'], 
      price: 17.99,
      imageUrl: 'https://images.unsplash.com/photo-1490750967868-88aa4486c946?w=400&h=300&fit=crop'
    },
    { 
      name: 'Sunflare', 
      species: 'Helianthus annuus', 
      category: 'Sunflower', 
      colors: ['yellow'], 
      price: 9.99,
      imageUrl: 'https://images.unsplash.com/photo-1597848212624-e530bb09f921?w=400&h=300&fit=crop'
    },
    { 
      name: 'Violet Dream', 
      species: 'Viola odorata', 
      category: 'Violet', 
      colors: ['purple'], 
      price: 8.49,
      imageUrl: 'https://images.unsplash.com/photo-1615887047235-b6e0fef4f82e?w=400&h=300&fit=crop'
    },
    { 
      name: 'Blushing Bride', 
      species: 'Paeonia lactiflora', 
      category: 'Peony', 
      colors: ['pink'], 
      price: 24.99,
      imageUrl: 'https://images.unsplash.com/photo-1614794010481-aa58d6fe8857?w=400&h=300&fit=crop'
    },
    { 
      name: 'Starlight', 
      species: 'Lilium candidum', 
      category: 'Lily', 
      colors: ['white'], 
      price: 14.99,
      imageUrl: 'https://images.unsplash.com/photo-1617791160505-6f00504e3519?w=400&h=300&fit=crop'
    },
    { 
      name: 'Emerald Tulip', 
      species: 'Tulipa gesneriana', 
      category: 'Tulip', 
      colors: ['green','white'], 
      price: 12.99,
      imageUrl: 'https://images.unsplash.com/photo-1520763185298-1b434c919102?w=400&h=300&fit=crop'
    },
    { 
      name: 'Lavender Mist', 
      species: 'Lavandula angustifolia', 
      category: 'Lavender', 
      colors: ['lavender'], 
      price: 11.99,
      imageUrl: 'https://images.unsplash.com/photo-1499002238440-d264edd596ec?w=400&h=300&fit=crop'
    },
    { 
      name: 'Ocean Breeze', 
      species: 'Hydrangea macrophylla', 
      category: 'Hydrangea', 
      colors: ['blue'], 
      price: 21.49,
      imageUrl: 'https://images.unsplash.com/photo-1589051884289-f25f3e7dec90?w=400&h=300&fit=crop'
    },
    { 
      name: 'Golden Daisy', 
      species: 'Bellis perennis', 
      category: 'Daisy', 
      colors: ['yellow','white'], 
      price: 6.99,
      imageUrl: 'https://images.unsplash.com/photo-1558109550-62c0e52fc8c7?w=400&h=300&fit=crop'
    },
  ];

  users.forEach(user => {
    for (let i = 0; i < 25; i++) {
      const base = catalog[i % catalog.length];
      const status = i % 5 === 0 ? 'archived' : 'active';
      flowers.push({
        id: `flower${flowerIdCounter++}`,
        userId: user.id,
        name: base.name + (i > 9 ? ` ${i}` : ''),
        species: base.species,
        category: base.category,
        colors: base.colors,
        price: base.price + ((i % 3) * 0.5),
        status,
        updatedAt: Date.now() - (i * 1000 * 60 * 45),
        description: `A beautiful ${base.category.toLowerCase()} named ${base.name}.`,
        imageUrl: base.imageUrl,
        photoUrl: undefined,
        location: undefined
      });
    }
  });

  console.log(`Seeded ${flowers.length} flowers for ${users.length} users`);
}

/**
 * Find user by email
 * @param {string} email
 * @returns {import('./types.js').User|undefined}
 */
export function findUserByEmail(email) {
  return users.find(u => u.email === email);
}

/**
 * Find user by ID
 * @param {string} userId
 * @returns {import('./types.js').User|undefined}
 */
export function findUserById(userId) {
  return users.find(u => u.id === userId);
}

/**
 * Get paginated items for a user
 * @param {string} userId
 * @param {number} page
 * @param {number} pageSize
 * @param {string} [query]
 * @param {string} [status]
 * @returns {import('./types.js').PaginatedResponse}
 */
export function getFlowers(userId, page = 1, pageSize = 10, query = '', status = '', category = '') {
  let filtered = flowers.filter(f => f.userId === userId);

  // Apply status filter
  if (status && status !== 'all') {
    filtered = filtered.filter(f => f.status === status);
  }

  if (category && category !== 'all') {
    filtered = filtered.filter(f => f.category.toLowerCase() === category.toLowerCase());
  }

  // Apply search query
  if (query) {
    const lowerQuery = query.toLowerCase();
    filtered = filtered.filter(f =>
      f.name.toLowerCase().includes(lowerQuery) ||
      f.species.toLowerCase().includes(lowerQuery) ||
      f.category.toLowerCase().includes(lowerQuery) ||
      (f.description && f.description.toLowerCase().includes(lowerQuery))
    );
  }

  // Sort by updatedAt desc
  filtered.sort((a, b) => b.updatedAt - a.updatedAt);

  const total = filtered.length;
  const start = (page - 1) * pageSize;
  const end = start + pageSize;
  const paginatedItems = filtered.slice(start, end);

  return {
    items: paginatedItems,
    page,
    pageSize,
    total,
    hasMore: end < total
  };
}

/**
 * Get a single item by ID
 * @param {string} itemId
 * @param {string} userId
 * @returns {import('./types.js').Item|undefined}
 */
export function getFlower(flowerId, userId) {
  return flowers.find(f => f.id === flowerId && f.userId === userId);
}

/**
 * Create a new item
 * @param {string} userId
 * @param {Partial<import('./types.js').Item>} itemData
 * @returns {import('./types.js').Item}
 */
export function createFlower(userId, flowerData) {
  const newFlower = {
    id: `flower${flowerIdCounter++}`,
    userId,
    name: flowerData.name || 'Unnamed',
    species: flowerData.species || '',
    category: flowerData.category || 'Rose',
    colors: flowerData.colors || ['green'],
    price: flowerData.price ?? 9.99,
    status: flowerData.status || 'active',
    updatedAt: Date.now(),
    description: flowerData.description,
    imageUrl: flowerData.imageUrl,
    photoUrl: flowerData.photoUrl,
    location: flowerData.location
  };
  
  flowers.push(newFlower);
  return newFlower;
}

/**
 * Update an existing item
 * @param {string} itemId
 * @param {string} userId
 * @param {Partial<import('./types.js').Item>} updates
 * @returns {import('./types.js').Item|null}
 */
export function updateFlower(flowerId, userId, updates) {
  const flower = flowers.find(f => f.id === flowerId && f.userId === userId);
  
  if (!flower) {
    return null;
  }

  if (updates.name !== undefined) flower.name = updates.name;
  if (updates.species !== undefined) flower.species = updates.species;
  if (updates.category !== undefined) flower.category = updates.category;
  if (updates.colors !== undefined) flower.colors = updates.colors;
  if (updates.price !== undefined) flower.price = updates.price;
  if (updates.status !== undefined) flower.status = updates.status;
  if (updates.description !== undefined) flower.description = updates.description;
  if (updates.imageUrl !== undefined) flower.imageUrl = updates.imageUrl;
  if (updates.photoUrl !== undefined) flower.photoUrl = updates.photoUrl;
  if (updates.location !== undefined) flower.location = updates.location;
  flower.updatedAt = Date.now();

  return flower;
}

