#include "credentials.hpp"
#include "network_client.hpp"
#include "market_operations.hpp"
#include "latency_tracker.hpp"
#include <iostream>
#include <memory>
#include <future>
#include <unordered_map>
using namespace std;

void displayMenu()
{
    cout << "\n=== Deribit Trading System ===\n"
         << "1. Create New Order\n"
         << "2. Remove Order\n"
         << "3. Update Order\n"
         << "4. View Market Depth\n"
         << "5. Check Positions\n"
         << "6. Exit System\n"
         << "Select option: ";
}

void runTradingSystem()
{
    try
    {
        NetworkClient websocket("test.deribit.com", "443", "/ws/api/v2");
        websocket.establishConnection();

        auto trading = make_unique<MarketOperations>(websocket);
        auto auth_result = trading->login(API_KEY, API_SECRET);
        cout << "Authentication Status: " << auth_result.dump(4) << endl;

        unordered_map<string, json> order_cache;

        while (true)
        {
            displayMenu();
            int choice;
            cin >> choice;

            auto operation_start = LatencyTracker::startMeasurement();

            if (choice == 6)
            {
                cout << "Shutting down trading system...\n";
                break;
            }

            string instrument_id, order_id;
            double quantity, price;

            switch (choice)
            {
            case 1:
            {
                cout << "Enter details for new order:\n"
                     << "Instrument (e.g., BTC-PERPETUAL): ";
                cin >> instrument_id;
                cout << "Quantity: ";
                cin >> quantity;
                cout << "Price: ";
                cin >> price;

                try
                {
                    auto order_task = async(launch::async, [&]()
                                            {
                            auto placement_start = LatencyTracker::startMeasurement();
                            json result = trading->submitOrder(instrument_id, quantity, price);
                            LatencyTracker::endMeasurement(placement_start, "Order Creation");
                            return result; });

                    json order_result = order_task.get();
                    cout << "Order Details: " << order_result.dump(4) << endl;

                    if (order_result.contains("order_id"))
                    {
                        order_cache[order_result["order_id"]] = order_result;
                    }
                }
                catch (const exception &e)
                {
                    cerr << "Order creation failed: " << e.what() << endl;
                }
                break;
            }

            case 2:
            {
                cout << "Enter order ID to cancel: ";
                cin >> order_id;

                try
                {
                    auto cancel_task = async(launch::async, [&]()
                                             {
                            auto cancel_start = LatencyTracker::startMeasurement();
                            json result = trading->removeOrder(order_id);
                            LatencyTracker::endMeasurement(cancel_start, "Order Cancellation");
                            return result; });

                    json cancel_result = cancel_task.get();
                    cout << "Cancellation Result: " << cancel_result.dump(4) << endl;

                    if (cancel_result.contains("status") && cancel_result["status"] == "cancelled")
                    {
                        order_cache.erase(order_id);
                    }
                }
                catch (const exception &e)
                {
                    cerr << "Order cancellation failed: " << e.what() << endl;
                }
                break;
            }

            case 3:
            { // Update Order
                cout << "Enter order details to modify:\n"
                     << "Order ID: ";
                cin >> order_id;
                cout << "New price: ";
                cin >> price;
                cout << "New quantity: ";
                cin >> quantity;

                try
                {
                    auto modify_task = async(launch::async, [&]()
                                             {
                            auto modify_start = LatencyTracker::startMeasurement();
                            json result = trading->adjustOrder(order_id, price, quantity);
                            LatencyTracker::endMeasurement(modify_start, "Order Modification");
                            return result; });

                    json modify_result = modify_task.get();
                    cout << "Modification Result: " << modify_result.dump(4) << endl;

                    if (modify_result.contains("order_id"))
                    {
                        order_cache[modify_result["order_id"]] = modify_result;
                    }
                }
                catch (const exception &e)
                {
                    cerr << "Order modification failed: " << e.what() << endl;
                }
                break;
            }

            case 4:
            {
                cout << "Enter instrument for market depth (e.g., BTC-PERPETUAL): ";
                cin >> instrument_id;

                try
                {
                    auto depth_start = LatencyTracker::startMeasurement();
                    json market_depth = trading->fetchMarketDepth(instrument_id);
                    LatencyTracker::endMeasurement(depth_start, "Market Depth Fetch");

                    cout << "Market Depth:\n";
                    if (market_depth.contains("result"))
                    {
                        auto &result = market_depth["result"];
                        cout << "Asks (first 5):\n";
                        for (size_t i = 0; i < min(size_t(5), result["asks"].size()); ++i)
                        {
                            cout << "Price: " << result["asks"][i][0]
                                 << ", Size: " << result["asks"][i][1] << endl;
                        }
                        cout << "\nBids (first 5):\n";
                        for (size_t i = 0; i < min(size_t(5), result["bids"].size()); ++i)
                        {
                            cout << "Price: " << result["bids"][i][0]
                                 << ", Size: " << result["bids"][i][1] << endl;
                        }
                    }
                }
                catch (const exception &e)
                {
                    cerr << "Market depth fetch failed: " << e.what() << endl;
                }
                break;
            }

            case 5:
            {
                try
                {
                    auto positions_start = LatencyTracker::startMeasurement();
                    json positions = trading->getActivePositions();
                    LatencyTracker::endMeasurement(positions_start, "Positions Fetch");

                    cout << "Current Positions:\n";
                    if (positions.contains("result"))
                    {
                        for (const auto &position : positions["result"])
                        {
                            cout << "Instrument: " << position["instrument_name"]
                                 << "\nSize: " << position["size"]
                                 << "\nAverage Price: " << position["average_price"]
                                 << "\nUnrealized PNL: " << position["unrealized_pnl"]
                                 << "\n-------------------\n";
                        }
                    }
                }
                catch (const exception &e)
                {
                    cerr << "Positions fetch failed: " << e.what() << endl;
                }
                break;
            }

            default:
                cout << "Invalid option selected. Please try again.\n";
                break;
            }

            LatencyTracker::endMeasurement(operation_start, "Total Operation Time");
        }

        websocket.disconnect();
    }
    catch (const exception &e)
    {
        cerr << "Fatal system error: " << e.what() << endl;
    }
}

int main()
{
    try
    {
        cout << "Starting Deribit Trading System...\n";
        runTradingSystem();
        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
}
