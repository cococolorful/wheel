#pragma once
#include "tiny_socket/socket.hpp"

namespace wk {
namespace tiny_socket {

class server : public socket {
public:
    server(u_short port = 6009, const std::string &ip_address = "127.0.0.1");
    socket accept();
};

inline server::server(u_short port, const std::string &ip_address) : socket(socket_type::stream) {
    set_address(ip_address);
    set_port(port);
    std::cout << "server created." << std::endl;  // TODO: replace with tiny-logger

    if (::bind(m_socket, reinterpret_cast<sockaddr *>(&m_addr), sizeof(m_addr)) == SOCKET_ERROR) {
        std::cout << "TCP Socket Bind error." << std::endl;
#ifdef WIN32
        std::cout << WSAGetLastError() << std::endl;
#endif
    }

    std::cout << "TCP Socket Bound." << std::endl;
    listen(m_socket, 3);
    std::cout << "TCP Socket waiting for incoming connections..." << std::endl;
}
inline socket server::accept() {
    socklen_t client_size = sizeof(sockaddr_in);
    sockaddr_in client;

    return socket(::accept(m_socket, reinterpret_cast<sockaddr *>(&client), &client_size));
}

}  // namespace tiny_socket

}  // namespace wk
