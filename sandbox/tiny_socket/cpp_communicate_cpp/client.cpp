#include <iostream>
#include <tiny_socket/tiny_socket.hpp>

int main() {
    wk::tiny_socket::client client(6009, "127.0.0.1");
    client.make_connect(true);
    while (true) {
        std::string s;
        std::cout << "To server:";
        std::cin >> s;
        client.send_message(s);
        std::cout << "From server:" << client.receive_message() << std::endl;
    }

    return 0;
}