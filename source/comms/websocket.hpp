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
        int audio_buffer[64];


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

        void initAudio(void) {
            wsAudio.setUrl("ws://localhost:8080");
            std::cout << "Connecting to ws://localhost:8080...\n";

            wsAudio.setOnMessageCallback([&](const auto& msg) {
              std::lock_guard lock { mtx };

              if (msg->type == ix::WebSocketMessageType::Message) {
                 sscanf(msg->str.c_str(), "[%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i]", &audio_buffer[0], &audio_buffer[1], &audio_buffer[2], &audio_buffer[3], &audio_buffer[4], &audio_buffer[5], &audio_buffer[6], &audio_buffer[7], &audio_buffer[8], &audio_buffer[9], &audio_buffer[10], &audio_buffer[11], &audio_buffer[12], &audio_buffer[13], &audio_buffer[14], &audio_buffer[15], &audio_buffer[16], &audio_buffer[17], &audio_buffer[18], &audio_buffer[19], &audio_buffer[20], &audio_buffer[21], &audio_buffer[22], &audio_buffer[23], &audio_buffer[24], &audio_buffer[25], &audio_buffer[26], &audio_buffer[27], &audio_buffer[28], &audio_buffer[29], &audio_buffer[30], &audio_buffer[31], &audio_buffer[32], &audio_buffer[33], &audio_buffer[34], &audio_buffer[35], &audio_buffer[36], &audio_buffer[37], &audio_buffer[38], &audio_buffer[39], &audio_buffer[40], &audio_buffer[41], &audio_buffer[42], &audio_buffer[43], &audio_buffer[44], &audio_buffer[45], &audio_buffer[46], &audio_buffer[47], &audio_buffer[48], &audio_buffer[49], &audio_buffer[50], &audio_buffer[51], &audio_buffer[52], &audio_buffer[53], &audio_buffer[54], &audio_buffer[55], &audio_buffer[56], &audio_buffer[57], &audio_buffer[58], &audio_buffer[59], &audio_buffer[60], &audio_buffer[61], &audio_buffer[62], &audio_buffer[63]);
              }

            });

            wsAudio.start();
        }


        void update(void) {
            // Run callbacks on main thread.
            std::lock_guard lock { mtx };

            for (const auto& [key, args] : queue) {
                for (auto& handler : handlers[key]) handler(args);
            }

            queue.clear();
        }

        void sendData(const std::string& kaas, float worst)
        {
            if(ws.getReadyState() != ix::ReadyState::Open)
                return;

            std::stringstream msg;
            msg << "data|" << kaas << "=" << worst;
            ws.sendText(msg.str());
        }

        void sendData(const std::string& stri)
        {
            if(ws.getReadyState() != ix::ReadyState::Open)
                return;

            std::stringstream msg;
            msg << "data|" << stri;
            ws.sendText(msg.str());
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
        ix::WebSocket wsAudio;

        std::unordered_map<std::string, std::vector<message_handler>> handlers;

        struct cb_data { std::string key, args; };
        std::vector<cb_data> queue;
    };
}