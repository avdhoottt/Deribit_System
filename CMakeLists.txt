cmake_minimum_required(VERSION 3.10)
project(Deribit_System)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

set(BOOST_ROOT "C:/boost_1_87_0")
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

set(SOURCE_FILES
    main.cpp
    network_client.cpp
    market_operations.cpp
    latency_tracker.cpp
    performance_monitor.cpp
)

add_executable(trading_system ${SOURCE_FILES})

target_include_directories(trading_system PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(trading_system PRIVATE
    ${Boost_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
