/**
 * @typedef {Object} User
 * @property {string} id
 * @property {string} email
 * @property {string} name
 * @property {string} password
 */

/**
 * @typedef {Object} Flower
 * @property {string} id
 * @property {string} userId
 * @property {string} name
 * @property {string} species
 * @property {string} category        // e.g., Rose, Tulip, Orchid
 * @property {string[]} colors        // e.g., ['red','white']
 * @property {number} price           // e.g., 19.99
 * @property {'active'|'archived'} status
 * @property {number} updatedAt
 * @property {string} [description]
 * @property {string} [imageUrl]
 * @property {string} [photoUrl]
 * @property {Object} [location]
 * @property {number} [location.lat]
 * @property {number} [location.lng]
 */

/**
 * @typedef {Object} PaginatedResponse
 * @property {Flower[]} items
 * @property {number} page
 * @property {number} pageSize
 * @property {number} total
 * @property {boolean} hasMore
 */

export {};

