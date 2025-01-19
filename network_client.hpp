#ifndef NETWORK_CLIENT_HPP
#define NETWORK_CLIENT_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class NetworkClient
{
public:
    NetworkClient(const string &server, const string &port, const string &path);
    void establishConnection();
    void transmitData(const json &data);
    json receiveData();
    void disconnect();
    bool isConnected() const { return connected; }

private:
    boost::asio::io_context io_context;
    ssl::context ssl_context;
    tcp::resolver resolver;
    websocket::stream<beast::ssl_stream<tcp::socket>> ws;
    string host;
    string endpoint;
    bool connected;

    void setupSSL();
    void logError(const string &operation, const string &message);
};

#endif
