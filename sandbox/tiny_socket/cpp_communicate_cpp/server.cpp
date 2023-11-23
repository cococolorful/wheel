#include <iostream>
#include <tiny_socket/tiny_socket.hpp>

int main() {
    wk::tiny_socket::server server;
    auto client = server.accept();
    std::cout << "Connected! \n";

    while (true) {
        auto s = client.receive_message();
        std::cout << "From client:" << s << std::endl;
        std::cout << "To client:";
        std::cin >> s;
        client.send_message(s);
    }

    return 0;
}