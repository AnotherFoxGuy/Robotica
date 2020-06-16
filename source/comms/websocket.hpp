#pragma once

#include <utility/utility.hpp>

#include <ixwebsocket/IXWebSocket.h>

#include <string_view>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>
#include <cstdio>
#include <regex>
#include <mutex>
#include <utility>


namespace robotica {
    class websocket {
    public:
        constexpr static std::string_view url = "ws://vps.anotherfoxguy.com:5000/ws/Robot";
        using message_handler = std::function<void(std::string_view)>;


        static websocket& instance(void) {
            static websocket i {};
            return i;
        }


        void init(void) {
            ws.setUrl(std::string(url));
            std::cout << "Connecting to " << url << "...\n";

            ws.setOnMessageCallback([&](const auto& msg) {
                std::lock_guard lock { mtx };

                if (msg->type == ix::WebSocketMessageType::Open) {
                    std::cout << "Connection with server established!\n";

#ifdef _MSC_VER
                    std::string name = std::getenv("USERNAME");
#else
                    std::string name = std::getenv("USER");
#endif
                    name = concat("UwUBot (", name, ")");
                    ws.sendText(std::string("register|") + name);
                }
                else if (msg->type == ix::WebSocketMessageType::Message) {
                    const static std::regex rgx{ R"REGEX((\w+)\((.+)\))REGEX" };

                    auto split = regex_groups(msg->str, rgx);
                    if (split.size() >= 3)
                        queue.push_back({ split[1], split[2] });
                    else
                        std::cout << "Message: " << msg->str << std::endl;
                }

            });

            ws.start();
        }


        void update(void) {
            // Run callbacks on main thread.
            std::lock_guard lock { mtx };

            for (const auto& [key, args] : queue) {
                for (auto& handler : handlers[key]) handler(args);
            }

            queue.clear();
        }


        void add_callback(const std::string& key, message_handler&& fn) {
            std::lock_guard lock { mtx };

            auto it = handlers.find(key);

            if (it == handlers.end()) handlers.insert({ key, { std::move(fn) } });
            else it->second.push_back(std::move(fn));
        }
    private:
        std::mutex mtx;
        ix::WebSocket ws;

        std::unordered_map<std::string, std::vector<message_handler>> handlers;

        struct cb_data { std::string key, args; };
        std::vector<cb_data> queue;
    };
}