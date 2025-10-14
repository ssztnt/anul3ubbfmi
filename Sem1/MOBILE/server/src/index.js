import express from 'express';
import http from 'http';
import { Server } from 'socket.io';
import cors from 'cors';
import fs from 'fs';
import path from 'path';
import crypto from 'crypto';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

import { generateToken, verifyToken, authenticateRequest } from './auth.js';
import {
  seedData,
  findUserByEmail,
  getFlowers,
  getFlower,
  createFlower,
  updateFlower
} from './data.js';

const app = express();
app.use(cors());
app.use(express.json({ limit: '50mb' }));

// Setup uploads directory
const UPLOAD_DIR = path.join(__dirname, '..', 'uploads');
if (!fs.existsSync(UPLOAD_DIR)) {
  fs.mkdirSync(UPLOAD_DIR, { recursive: true });
}

// Serve static uploads
app.use('/uploads', express.static(UPLOAD_DIR));

const server = http.createServer(app);
const io = new Server(server, {
  cors: {
    origin: '*',
    methods: ['GET', 'POST']
  }
});

// Seed initial data
seedData();

// ============================================================================
// UPLOAD ROUTE
// ============================================================================

/**
 * POST /upload
 * Body: { base64: string, ext: string }
 * Returns: { url: string }
 */
app.post('/upload', (req, res) => {
  try {
    const { base64, ext } = req.body || {};
    if (!base64 || !ext) {
      return res.status(400).json({ error: 'Missing base64 or ext' });
    }
    
    const allowedExts = ['jpg', 'jpeg', 'png', 'gif'];
    const extension = ext.toLowerCase();
    if (!allowedExts.includes(extension)) {
      return res.status(400).json({ error: 'Invalid extension' });
    }

    const buf = Buffer.from(base64, 'base64');
    const filename = crypto.randomBytes(8).toString('hex') + '.' + extension;
    const filepath = path.join(UPLOAD_DIR, filename);
    
    fs.writeFileSync(filepath, buf);
    
    const url = `${req.protocol}://${req.get('host')}/uploads/${filename}`;
    return res.json({ url });
  } catch (error) {
    console.error('Upload error:', error);
    return res.status(500).json({ error: 'Upload failed' });
  }
});

// ============================================================================
// AUTH ROUTES
// ============================================================================

/**
 * POST /auth/login
 * Body: { email, password }
 * Returns: { token, user: { id, email, name } }
 */
app.post('/auth/login', (req, res) => {
  const { email, password } = req.body;

  if (!email || !password) {
    return res.status(400).json({ error: 'Email and password are required' });
  }

  const user = findUserByEmail(email);

  if (!user || user.password !== password) {
    return res.status(401).json({ error: 'Invalid credentials' });
  }

  const token = generateToken(user.id, user.email);

  res.json({
    token,
    user: {
      id: user.id,
      email: user.email,
      name: user.name
    }
  });
});

// ============================================================================
// FLOWERS ROUTES (all require authentication)
// ============================================================================

/**
 * GET /flowers?page=1&pageSize=10&q=&status=active&category=
 * Returns paginated flowers for the authenticated user
 */
app.get('/flowers', authenticateRequest, (req, res) => {
  const userId = req.user.sub;
  const page = parseInt(req.query.page) || 1;
  const pageSize = parseInt(req.query.pageSize) || 10;
  const query = req.query.q || '';
  const status = req.query.status || '';

  const category = req.query.category || '';
  const result = getFlowers(userId, page, pageSize, query, status, category);
  res.json(result);
});

/**
 * GET /flowers/:id
 * Returns a single flower (only if owned by user)
 */
app.get('/flowers/:id', authenticateRequest, (req, res) => {
  const userId = req.user.sub;
  const flowerId = req.params.id;

  const flower = getFlower(flowerId, userId);

  if (!flower) {
    return res.status(404).json({ error: 'Flower not found' });
  }

  res.json(flower);
});

/**
 * POST /flowers
 * Body: { name, species?, category?, colors?, price?, status?, description?, imageUrl? }
 * Creates a new flower owned by the authenticated user
 */
app.post('/flowers', authenticateRequest, (req, res) => {
  const userId = req.user.sub;
  const { name } = req.body;

  if (!name) {
    return res.status(400).json({ error: 'Name is required' });
  }

  const newFlower = createFlower(userId, req.body || {});

  // Notify via WebSocket
  io.to(`user:${userId}`).emit('flower:created', { flower: newFlower });

  res.status(201).json(newFlower);
});

/**
 * PUT /flowers/:id
 * Body: { name?, species?, category?, colors?, price?, status?, description?, imageUrl? }
 * Updates a flower (only if owned by user)
 */
app.put('/flowers/:id', authenticateRequest, (req, res) => {
  const userId = req.user.sub;
  const flowerId = req.params.id;

  const updatedFlower = updateFlower(flowerId, userId, req.body || {});

  if (!updatedFlower) {
    return res.status(404).json({ error: 'Flower not found' });
  }

  // Notify via WebSocket
  io.to(`user:${userId}`).emit('flower:updated', { flower: updatedFlower });

  res.json(updatedFlower);
});

// ============================================================================
// WEBSOCKET (Socket.IO with authentication)
// ============================================================================

io.on('connection', (socket) => {
  const token = socket.handshake.auth.token;

  if (!token) {
    console.log('Connection rejected: missing token');
    socket.disconnect();
    return;
  }

  const decoded = verifyToken(token);

  if (!decoded) {
    console.log('Connection rejected: invalid token');
    socket.disconnect();
    return;
  }

  const userId = decoded.sub;
  const userRoom = `user:${userId}`;

  socket.join(userRoom);
  console.log(`User ${userId} connected and joined room ${userRoom}`);

  socket.on('disconnect', () => {
    console.log(`User ${userId} disconnected`);
  });
});

// ============================================================================
// START SERVER
// ============================================================================

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
  console.log(`✅ Server running on http://localhost:${PORT}`);
  console.log('📝 Test credentials:');
  console.log('   - alice@example.com / alice123');
  console.log('   - bob@example.com / bob123');
});
