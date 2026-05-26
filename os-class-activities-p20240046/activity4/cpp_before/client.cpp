#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <unistd.h>

const int PORT = 9001;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ./client 192.168.204.95 Song Phengroth" << std::endl;
        return 1;
    }

    std::string server_ip = argv[1];
    std::string name = argv[2];

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, server_ip.c_str(), &server.sin_addr);

    connect(sock, (sockaddr*)&server, sizeof(server));
    send(sock, name.c_str(), name.size(), 0);

    char buffer[1024] = {0};
    read(sock, buffer, sizeof(buffer) - 1);
    std::cout << buffer;

    close(sock);
    return 0;
}