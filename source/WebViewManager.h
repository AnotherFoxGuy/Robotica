#pragma once

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/WebSocket.h>

class WebViewManager
{
public:
    void start();
    void stop();
    void sendData(const std::string& payload);
private:
    Poco::Net::HTTPServer *server;

};

