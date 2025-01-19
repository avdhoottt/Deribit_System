#include "network_client.hpp"
#include "performance_monitor.hpp"
#include <iostream>
using namespace std;

NetworkClient::NetworkClient(const string &server, const string &port, const string &path)
    : ssl_context(ssl::context::tlsv12_client),
      resolver(io_context),
      ws(io_context, ssl_context),
      host(server),
      endpoint(path),
      connected(false)
{
    setupSSL();
}

void NetworkClient::setupSSL()
{
    ssl_context.set_default_verify_paths();
    ssl_context.set_verify_mode(ssl::verify_peer);
}

void NetworkClient::establishConnection()
{
    try
    {
        auto perfStart = PerformanceMonitor::startTimer();

        auto endpoints = resolver.resolve(host, "443");
        boost::asio::connect(ws.next_layer().next_layer(), endpoints);

        ws.next_layer().handshake(ssl::stream_base::client);
        ws.handshake(host, endpoint);

        connected = true;
        PerformanceMonitor::stopTimer(perfStart, "Connection Setup");
        cout << "Connected to exchange" << endl;
    }
    catch (const exception &e)
    {
        logError("connection", e.what());
        throw;
    }
}

void NetworkClient::transmitData(const json &data)
{
    try
    {
        if (!connected)
        {
            throw runtime_error("Not connected to server");
        }

        string message = data.dump();
        ws.write(boost::asio::buffer(message));
        cout << "Transmitted: " << message << endl;
    }
    catch (const exception &e)
    {
        logError("transmission", e.what());
        throw;
    }
}

json NetworkClient::receiveData()
{
    try
    {
        if (!connected)
        {
            throw runtime_error("Not connected to server");
        }

        beast::flat_buffer buffer;
        ws.read(buffer);

        string data = beast::buffers_to_string(buffer.data());
        cout << "Received: " << data << endl;

        return json::parse(data);
    }
    catch (const exception &e)
    {
        logError("reception", e.what());
        throw;
    }
}

void NetworkClient::disconnect()
{
    if (connected)
    {
        ws.close(websocket::close_code::normal);
        connected = false;
        cout << "Disconnected from exchange" << endl;
    }
}

void NetworkClient::logError(const string &operation, const string &message)
{
    cerr << "Network error during " << operation << ": " << message << endl;
}
