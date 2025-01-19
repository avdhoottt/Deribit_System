#include "market_operations.hpp"
#include "latency_tracker.hpp"
#include <iostream>
using namespace std;

atomic<int> MarketOperations::messageCounter{1};

MarketOperations::MarketOperations(NetworkClient &client)
    : network(client) {}

int MarketOperations::getNextMessageId()
{
    return messageCounter++;
}

json MarketOperations::login(const string &key, const string &secret)
{
    try
    {
        json request = {
            {"jsonrpc", "2.0"},
            {"id", getNextMessageId()},
            {"method", "public/auth"},
            {"params", {{"grant_type", "client_credentials"}, {"client_id", key}, {"client_secret", secret}}}};
        return sendRequest(request);
    }
    catch (const exception &e)
    {
        handleError("Authentication failed: " + string(e.what()));
        throw;
    }
}

json MarketOperations::submitOrder(const string &symbol, double size, double price)
{
    try
    {
        json request = {
            {"jsonrpc", "2.0"},
            {"id", getNextMessageId()},
            {"method", "private/buy"},
            {"params", {{"instrument_name", symbol}, {"amount", size}, {"type", "limit"}, {"price", price}}}};
        return sendRequest(request);
    }
    catch (const exception &e)
    {
        handleError("Order submission failed: " + string(e.what()));
        throw;
    }
}

json MarketOperations::removeOrder(const string &orderId)
{
    try
    {
        json request = {
            {"jsonrpc", "2.0"},
            {"id", getNextMessageId()},
            {"method", "private/cancel"},
            {"params", {{"order_id", orderId}}}};
        return sendRequest(request);
    }
    catch (const exception &e)
    {
        handleError("Order cancellation failed: " + string(e.what()));
        throw;
    }
}

json MarketOperations::adjustOrder(const string &orderId, double newPrice, double newSize)
{
    try
    {
        json request = {
            {"jsonrpc", "2.0"},
            {"id", getNextMessageId()},
            {"method", "private/edit"},
            {"params", {{"order_id", orderId}, {"price", newPrice}, {"amount", newSize}, {"post_only", true}}}};
        return sendRequest(request);
    }
    catch (const exception &e)
    {
        handleError("Order modification failed: " + string(e.what()));
        throw;
    }
}

json MarketOperations::fetchMarketDepth(const string &symbol)
{
    try
    {
        json request = {
            {"jsonrpc", "2.0"},
            {"id", getNextMessageId()},
            {"method", "public/get_order_book"},
            {"params", {{"instrument_name", symbol}, {"depth", 10}}}};
        return sendRequest(request);
    }
    catch (const exception &e)
    {
        handleError("Market depth fetch failed: " + string(e.what()));
        throw;
    }
}

json MarketOperations::getActivePositions()
{
    try
    {
        json request = {
            {"jsonrpc", "2.0"},
            {"id", getNextMessageId()},
            {"method", "private/get_positions"},
            {"params", {{"kind", "future"}}}};
        return sendRequest(request);
    }
    catch (const exception &e)
    {
        handleError("Position fetch failed: " + string(e.what()));
        throw;
    }
}

void MarketOperations::registerMarketDataCallback(const string &symbol,
                                                  function<void(const json &)> callback)
{
    dataHandlers[symbol] = callback;

    try
    {
        json subscription = {
            {"jsonrpc", "2.0"},
            {"id", getNextMessageId()},
            {"method", "public/subscribe"},
            {"params", {{"channels", {"book." + symbol + ".100ms"}}}}};
        sendRequest(subscription);
    }
    catch (const exception &e)
    {
        handleError("Market data subscription failed: " + string(e.what()));
        throw;
    }
}

json MarketOperations::sendRequest(const json &request)
{
    auto latencyStart = LatencyTracker::startMeasurement();

    network.transmitData(request);
    json response = network.receiveData();

    LatencyTracker::endMeasurement(latencyStart, "API Request Latency");

    if (response.contains("error"))
    {
        throw runtime_error("API error: " + response["error"]["message"].get<string>());
    }

    return response;
}

void MarketOperations::handleError(const string &context)
{
    cerr << "Operation error: " << context << endl;
}
