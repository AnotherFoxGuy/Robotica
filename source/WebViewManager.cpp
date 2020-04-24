#include "WebViewManager.h"
#include <iostream>

#include "Poco/Format.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/WebSocket.h"

using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;


using namespace std;

std::vector<Poco::Net::WebSocket> Kaas;

class PageRequestHandler : public HTTPRequestHandler
/// Return a HTML document with some JavaScript creating
/// a WebSocket connection.
{
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
    {
        response.setChunkedTransferEncoding(true);
        response.setContentType("text/html");
        std::ostream& ostr = response.send();
        ostr << "<html>";
        ostr << "<head>";
        ostr << "<title>WebSocketServer</title>";
        ostr << "<script type=\"text/javascript\">";
        ostr << "function WebSocketTest()";
        ostr << "{";
        ostr << "  if (\"WebSocket\" in window)";
        ostr << "  {";
        ostr << "    var ws = new WebSocket(\"ws://" << request.serverAddress().toString() << "/ws\");";
        ostr << "    ws.onopen = function()";
        ostr << "      {";
        ostr << "        ws.send(\"Hello, world!\");";
        ostr << "      };";
        ostr << "    ws.onmessage = function(evt)";
        ostr << "      { ";
        ostr << "        var msg = evt.data;";
        ostr << "        alert(\"Message received: \" + msg);";
        ostr << "        ws.close();";
        ostr << "      };";
        ostr << "    ws.onclose = function()";
        ostr << "      { ";
        ostr << "        alert(\"WebSocket closed.\");";
        ostr << "      };";
        ostr << "  }";
        ostr << "  else";
        ostr << "  {";
        ostr << "     alert(\"This browser does not support WebSockets.\");";
        ostr << "  }";
        ostr << "}";
        ostr << "</script>";
        ostr << "</head>";
        ostr << "<body>";
        ostr << "  <h1>WebSocket Server</h1>";
        ostr << "  <p><a href=\"javascript:WebSocketTest()\">Run WebSocket Script</a></p>";
        ostr << "</body>";
        ostr << "</html>";
    }
};

class WebSocketRequestHandler : public HTTPRequestHandler
/// Handle a WebSocket connection.
{

public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
    {
        try
        {
            WebSocket ws(request, response);
            cout << "WebSocket connection established." << endl;
            Kaas.push_back(ws);

            char buffer[1024];
            int flags;
            int n;
            do
            {
                n = ws.receiveFrame(buffer, sizeof(buffer), flags);
                cout << Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)) << endl;
            } while (n > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
            cout << "WebSocket connection closed." << endl;
            Kaas.erase(std::remove(Kaas.begin(), Kaas.end(), ws), Kaas.end());
        }
        catch (WebSocketException& exc)
        {
            cout << exc.message();
            switch (exc.code())
            {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            // fallthrough
            case WebSocket::WS_ERR_NO_HANDSHAKE:
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                response.setContentLength(0);
                response.send();
                break;
            }
        }
    }
};

class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override
    {
        if (request.find("Upgrade") != request.end() && Poco::icompare(request["Upgrade"], "websocket") == 0)
            return new WebSocketRequestHandler;
        else
            return new PageRequestHandler;
    }
};

void WebViewManager::start()
{
    // set-up a server socket
    ServerSocket svs(8080);
    // set-up a HTTPServer instance
    server = new HTTPServer(new RequestHandlerFactory, svs, new HTTPServerParams);
    // start the HTTPServer
    server->start();
}
void WebViewManager::stop()
{
    server->stopAll(true);
}
void WebViewManager::sendData(const string& payload)
{
    for (WebSocket & ws : Kaas)
    {
        try
        {
            ws.sendFrame(payload.c_str(), payload.length(), 0x81);
        }
        catch (...)
        {

        }
    }
}
