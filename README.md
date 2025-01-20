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

## Component Overview

### 1. Network Client (network_client.hpp/cpp)

The NetworkClient class manages all WebSocket communications with Deribit.

Key features:

```cpp
class NetworkClient {
    // Core networking capabilities
    void establishConnection();     // Establishes secure WebSocket connection
    void transmitData(json &data);  // Sends data to exchange
    json receiveData();            // Receives exchange responses
    void disconnect();             // Closes connection safely
}
```

Key implementations:

- SSL/TLS encryption using OpenSSL
- Asynchronous I/O with Boost.ASIO
- Comprehensive error handling
- Connection state management

### 2. Market Operations (market_operations.hpp/cpp)

Handles all trading-related operations and API interactions.

Core functionalities:

```cpp
class MarketOperations {
    // Trading operations
    json submitOrder(string &symbol, double size, double price);
    json removeOrder(string &orderId);
    json adjustOrder(string &orderId, double newPrice, double newSize);
    json fetchMarketDepth(string &symbol);
    json getActivePositions();
}
```

Features:

- Atomic message ID generation
- Thread-safe operations
- Comprehensive error handling
- Performance monitoring integration

### 3. Latency Tracker (latency_tracker.hpp/cpp)

Monitors and analyzes system performance.

Structure:

```cpp
class LatencyTracker {
    struct LatencyRecord {
        string operation;
        double duration;
        chrono::system_clock::time_point timestamp;
    };

    struct OperationStats {
        double min;
        double max;
        double total;
        int count;
    };
}
```

Capabilities:

- Microsecond precision timing
- Statistical analysis of operations
- Thread-safe metric collection
- Historical data maintenance

### 4. Performance Monitor (performance_monitor.hpp/cpp)

Provides system-wide performance tracking.

Implementation:

```cpp
class PerformanceMonitor {
    static chrono::high_resolution_clock::time_point startTimer();
    static void stopTimer(const time_point &startTime, const string &operationType);
    static map<string, double> getAverageTimings();
}
```

Features:

- High-precision operation timing
- Statistical aggregation
- Thread-safe operation
- Performance profiling capabilities

## System Menu Options

The system provides six main operations:

1. Create New Order
2. Remove Order
3. Update Order
4. View Market Depth
5. Check Positions
6. Exit System

Implementation in main.cpp:

```cpp
void displayMenu() {
    cout << "\n=== Deribit Trading System ===\n"
         << "1. Create New Order\n"
         << "2. Remove Order\n"
         << "3. Update Order\n"
         << "4. View Market Depth\n"
         << "5. Check Positions\n"
         << "6. Exit System\n"
         << "Select option: ";
}
```

## Thread Safety Implementation

The system ensures thread safety through multiple mechanisms:

1. Mutex Protection:

```cpp
mutex historyMutex;  // Protects shared resources
lock_guard<mutex> lock(historyMutex);  // RAII lock management
```

2. Atomic Operations:

```cpp
static atomic<int> messageCounter{1};  // Thread-safe counter
```

3. Lock-free Algorithms where possible

## Error Handling Strategy

Comprehensive error handling throughout the system:

1. Network Errors:

```cpp
try {
    ws.write(boost::asio::buffer(message));
} catch (const exception &e) {
    logError("transmission", e.what());
    throw;
}
```

2. API Errors:

```cpp
if (response.contains("error")) {
    throw runtime_error("API error: " + response["error"]["message"]);
}
```

3. System Errors:

```cpp
catch (const exception &e) {
    cerr << "Fatal system error: " << e.what() << endl;
}
```

## Performance Features

### 1. Latency Monitoring

- Operation-specific timing
- Network round-trip measurement
- API response time tracking

### 2. Performance Metrics

Example output:

```
=== Latency Metrics ===
Order Creation:
  Current: 15.23 ms
  Average: 12.45 ms
  Min: 8.12 ms
  Max: 22.34 ms
  Sample Count: 100
```

## Security Implementation

### 1. SSL/TLS Setup

```cpp
void NetworkClient::setupSSL() {
    ssl_context.set_default_verify_paths();
    ssl_context.set_verify_mode(ssl::verify_peer);
}
```

### 2. Secure Data Handling

- Encrypted WebSocket communication
- Secure credential management
- Protected API interactions

## Best Practices

The system implements several C++ best practices:

1. RAII (Resource Acquisition Is Initialization):

```cpp
auto trading = make_unique<MarketOperations>(websocket);
```

2. Smart Pointers:

- Use of unique_ptr for resource management
- Automatic cleanup of resources

3. Exception Safety:

- Strong exception guarantee where possible
- Proper resource cleanup in error paths

4. Modern C++ Features:

- Structured bindings
- Smart pointers
- std::optional where appropriate
