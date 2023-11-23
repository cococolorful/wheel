#pragma once
#include "tiny_socket/socket.hpp"

namespace wk {
namespace tiny_socket {

class client : public socket {
public:
    client() = delete;
    client(u_short port = 6009, const std::string &ip_address = "127.0.0.1");
    void make_connect(bool wait_until_conneted = false);
};

inline client::client(u_short port, const std::string &ip_address) : socket(socket_type::stream) {
    set_address(ip_address);
    set_port(port);
    std::cout << "client created." << std::endl;  // TODO: replace with tiny-logger
}
inline void client::make_connect(bool wait_until_conneted) {
    std::cout << "Connecting" << std::endl;  // TODO: replace with tiny-logger

    while (connect(m_socket, reinterpret_cast<sockaddr *>(&m_addr), sizeof(m_addr)) < 0 && wait_until_conneted) {
        std::cout << "Try conneting again" << std::endl;  // TODO: replace with tiny-logger
    }
    std::cout << "connected" << std::endl;
}

}  // namespace tiny_socket

}  // namespace wk
