#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 6667

int main()
{
    int server_fd, client_fd;
    sockaddr_in server_addr{};
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);

    /* =========================
       Socket erstellen
       ========================= */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "File descxriptor:"<< server_fd << std::endl;
    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* =========================
       Adresse setzen
       ========================= */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 1) < 0)
    {
        perror("listen");
        return 1;
    }

    std::cout << "Server läuft auf Port " << PORT << std::endl;

    /* =========================
       Client akzeptieren
       ========================= */
    client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
    if (client_fd < 0)
    {
        perror("accept");
        return 1;
    }

    std::cout << "Client verbunden\n";

    /* =========================
       Alle 5 Sekunden senden
       ========================= */
    while (true)
    {
        const char* msg = "Hallo Welt\r\n";
        ssize_t sent = send(client_fd, msg, std::strlen(msg), 0);

        if (sent <= 0)
        {
            std::cout << "Client getrennt\n";
            break;
        }

        std::cout << "Gesendet: Hallo Welt\n";
        sleep(5);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
