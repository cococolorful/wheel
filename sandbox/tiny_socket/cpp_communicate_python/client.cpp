#include <iostream>
#include <json.hpp>
#include <tiny_socket/tiny_socket.hpp>

int main() {
    wk::tiny_socket::client client(6009, "127.0.0.1");
    client.make_connect(true);

    using json = nlohmann::json;

    constexpr char* k_obs = "observation";
    constexpr char* k_action = "action";
    json sendData;
    sendData[k_obs] = std::vector{1.1f, 2.1f, 3.1f};
    client.send_message(sendData.dump());

    auto action_json = json::parse(client.receive_message().c_str());
    std::vector<float> action = action_json[k_action].get<std::vector<float>>();

    for (auto a : action) {
        std::cout << a;
    }
    std::cout << "\n";
    int tmp;
    std::cin >> tmp;
    return 0;
}