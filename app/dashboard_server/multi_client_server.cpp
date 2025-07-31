#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PORT 9090
#define LOG_FILE "/var/log/weather_cpp_server.log"
#define MAX_CLIENTS 10

void handle_client(int client_sock) {
    char buffer[1024] = {0};
    int read_bytes = read(client_sock, buffer, sizeof(buffer));
    if (read_bytes > 0) {
        std::ofstream log(LOG_FILE, std::ios::app);
        log << "Received: " << buffer << std::endl;
        log.close();

        std::string ack = "ACK\n";
        send(client_sock, ack.c_str(), ack.length(), 0);
    }
    close(client_sock);
}

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    setsid();
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main() {
    daemonize();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) exit(EXIT_FAILURE);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, MAX_CLIENTS);

    while (true) {
        socklen_t addrlen = sizeof(address);
        int client_sock = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_sock >= 0) {
            std::thread(handle_client, client_sock).detach();
        }
    }

    close(server_fd);
    return 0;
}
