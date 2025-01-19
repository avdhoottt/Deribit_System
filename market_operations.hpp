#ifndef MARKET_OPERATIONS_HPP
#define MARKET_OPERATIONS_HPP

#include "network_client.hpp"
#include <nlohmann/json.hpp>
#include <functional>
#include <map>
#include <atomic>

using namespace std;
using json = nlohmann::json;

class MarketOperations
{
public:
    explicit MarketOperations(NetworkClient &client);

    json login(const string &key, const string &secret);
    json submitOrder(const string &symbol, double size, double price);
    json removeOrder(const string &orderId);
    json adjustOrder(const string &orderId, double newPrice, double newSize);
    json fetchMarketDepth(const string &symbol);
    json getActivePositions();

    void registerMarketDataCallback(const string &symbol, function<void(const json &)> callback);

private:
    NetworkClient &network;
    map<string, function<void(const json &)>> dataHandlers;
    static atomic<int> messageCounter;

    int getNextMessageId();
    json sendRequest(const json &request);
    void handleError(const string &context);
};

#endif
