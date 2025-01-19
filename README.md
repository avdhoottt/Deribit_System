# Deribit Trading System Documentation

## Overview

A high-performance C++ trading system for interacting with the Deribit cryptocurrency exchange's test environment. The system provides real-time market data access, order management, and position tracking capabilities with a focus on low-latency operations.

## Features

- Real-time WebSocket connectivity
- Secure API authentication
- Order management (create/modify/cancel)
- Market depth monitoring
- Position tracking
- Performance metrics and latency tracking
- Asynchronous operations support

## Project Structure

```
Deribit_System/
├── CMakeLists.txt              # Build configuration
├── credentials.hpp             # API credentials
├── market_operations.hpp/cpp   # Trading operations implementation
├── network_client.hpp/cpp      # WebSocket communication layer
├── latency_tracker.hpp/cpp     # Performance monitoring utilities
├── performance_monitor.hpp/cpp # Detailed performance metrics
└── main.cpp                    # Application entry point
```

## Dependencies

- C++17 compatible compiler
- Boost (1.87.0 or newer)
- OpenSSL
- nlohmann/json
- CMake (3.10 or newer)

## Installation Steps

### 1. Install Required Libraries

#### Windows:

```bash
# Install vcpkg if not already installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg integrate install

# Install dependencies
vcpkg install boost:x64-windows
vcpkg install openssl:x64-windows
vcpkg install nlohmann-json:x64-windows
```

#### Linux:

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install libboost-all-dev
sudo apt-get install libssl-dev
sudo apt-get install nlohmann-json3-dev
```

### 2. Build Instructions

```bash
# Clone the repository
git clone https://github.com/avdhoottt/Deribit_System.git
cd Deribit_System

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build . --config Release
```

## Configuration

### API Credentials

Update credentials.hpp with your Deribit test account API keys:

```cpp
const string API_KEY = "api-key";
const string API_SECRET = "api-secret";
```

## Usage

### Running the System

```bash
# From the build/Release directory
./trading_system.exe
```

### Available Commands

1. Create New Order
2. Remove Order
3. Update Order
4. View Market Depth
5. Check Positions
6. Exit System

## Component Details

### NetworkClient

- Manages WebSocket connections
- Handles SSL/TLS encryption
- Provides message transmission/reception

### MarketOperations

- Implements trading functionality
- Manages order operations
- Handles market data subscriptions

### LatencyTracker

- Measures operation latencies
- Maintains performance history
- Provides statistical analysis

### PerformanceMonitor

- Detailed performance profiling
- Operation timing metrics
- System performance analysis

## Performance Considerations

- Asynchronous operations for non-blocking execution
- Smart pointer usage for efficient memory management
- Thread-safe performance monitoring
- Efficient market data handling

## Error Handling

- Comprehensive exception handling
- Detailed error reporting
- Connection state management
- Automatic reconnection capabilities

## Security Features

- SSL/TLS encryption
- Secure API authentication
- Input validation
- Error logging

## Best Practices

- Always check market depth before placing orders
- Use appropriate position sizes
- Monitor latency metrics
- Regularly check system logs
