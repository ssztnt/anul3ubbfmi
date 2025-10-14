#!/bin/bash

echo "🚀 Starting GreenLeaf Application..."
echo ""

# Function to cleanup on exit
cleanup() {
    echo ""
    echo "🛑 Shutting down servers..."
    kill $SERVER_PID $CLIENT_PID 2>/dev/null
    exit 0
}

trap cleanup INT TERM

# Start the backend server
echo "📡 Starting backend server..."
cd server
npm run dev &
SERVER_PID=$!

# Wait a bit for server to start
sleep 2

# Start the frontend
echo "🎨 Starting frontend app..."
cd ../myApp
npm run dev &
CLIENT_PID=$!

echo ""
echo "✅ Both servers are running!"
echo ""
echo "📝 Backend API: http://localhost:3000"
echo "🌐 Frontend App: http://localhost:5173"
echo ""
echo "Test credentials:"
echo "  - alice@example.com / alice123"
echo "  - bob@example.com / bob123"
echo ""
echo "Press Ctrl+C to stop both servers"
echo ""

# Wait for both processes
wait

