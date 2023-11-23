#pragma once

#include <assert.h>

#include <array>
#include <iostream>
#include <string>
#include <vector>
// Windows
#if defined(_WIN32)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

namespace wk {

namespace tiny_socket {
namespace detail {

constexpr inline int k_length_buf_size = 4;

inline int convert_bytes_to_int_little(const std::array<char, k_length_buf_size> &bytes) {
    int result = 0;
    for (int i = 0; i < k_length_buf_size; i++) {
        result |= static_cast<int>(bytes[i]) << (8 * i);
    }
    return result;
}

inline std::array<char, k_length_buf_size> int_to_bytes_little(std::int32_t value) {
    std::array<char, k_length_buf_size> bytes;
    for (int i = 0; i < k_length_buf_size; i++) {
        bytes[i] = static_cast<unsigned char>((value >> (8 * i)) & 0xFF);
    }
    return bytes;
}

}  // namespace detail

#ifdef _WIN32
inline int g_count{0};
#endif

class socket {
protected:
    int set_address(const std::string &ip_address);
    void set_port(u_short port);

public:
    ~socket();

    enum class socket_type { stream = SOCK_STREAM, datagram = SOCK_DGRAM };

    explicit socket(socket_type type);
    explicit socket(SOCKET other) : m_socket(other), m_flag(send_flag::remote_network){};

    enum class send_flag { local_network = MSG_DONTROUTE, remote_network = 0 };
    void send_message(const std::string &message) const;
    std::string receive_message() const;

    void set_send_flag(send_flag flag) { m_flag = flag; }

protected:
    SOCKET m_socket;
    sockaddr_in m_addr{};
    send_flag m_flag;
};

#pragma region Implement

inline socket::socket(socket::socket_type type) {
#ifdef _WIN32
    if (!g_count) {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            throw std::runtime_error("Error initializing Win_sock " + WSAGetLastError());
        }
    }
#else
    static_assert(false, "To be implemented!");
#endif

    m_socket = ::socket(AF_INET, static_cast<int>(type), 0);
    if (m_socket == INVALID_SOCKET) {
        throw std::runtime_error("Fail to create socket");
    }

    m_addr.sin_family = AF_INET;
    m_flag = send_flag::remote_network;
}

inline socket::~socket() {
#ifdef _WIN32
    if (!--g_count) {
        WSACleanup();
    }
#endif
}

inline int socket::set_address(const std::string &ip_address) {
    return inet_pton(AF_INET, ip_address.c_str(), &m_addr.sin_addr);
}

inline void socket::set_port(u_short port) { m_addr.sin_port = htons(port); }

inline void socket::send_message(const std::string &message) const {
    std::size_t length = message.length();

    if (send(m_socket, detail::int_to_bytes_little(length).data(), detail::k_length_buf_size,
             static_cast<int>(m_flag)) < 0) {
        throw std::runtime_error("Fail to send the length");
    }

    if (send(m_socket, message.c_str(), length, static_cast<int>(m_flag)) < 0) {
        throw std::runtime_error("Fail to send the message");
    }
}

inline std::string socket::receive_message() const {
    std::array<char, 4> reply_length_buf;
    if (recv(m_socket, reply_length_buf.data(), detail::k_length_buf_size, 0) == SOCKET_ERROR) {
        throw std::runtime_error("Fail to receive the length");
    }

    std::size_t length = detail::convert_bytes_to_int_little(reply_length_buf);
    std::vector<char> reply(length);

    if (recv(m_socket, reply.data(), length, 0) == SOCKET_ERROR) {
        throw std::runtime_error("Fail to receive the message");
    }
    return std::string(reply.begin(), reply.end());
}

#pragma endregion

}  // namespace tiny_socket

}  // namespace wk