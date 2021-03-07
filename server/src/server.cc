#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#include <server/game_room.h>
#include <iso646.h>


#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
#include <unistd.h>
#endif // __linux__

static bool RUN = true;

#ifdef __linux__
void ctrlCHandler(int)
{
    RUN = false;
}
#endif // __linux__

int main() {
#ifdef __linux__
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = ctrlCHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, nullptr);
#endif // __linux__


	unsigned short tcp_port = 53012;
	sf::TcpListener listener;
	listener.listen(tcp_port);

    std::vector<GameRoom> all_rooms;
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;

	sf::SocketSelector selector;
	selector.add(listener);

    while (RUN) {
		// bind the listener to a port

		if (selector.wait()) { // wait inifinitely long for _anything_ to change
			printf("Something is happening\n");
			if (selector.isReady(listener)) { // new pending connection
    			std::unique_ptr<sf::TcpSocket> new_socket = std::make_unique<sf::TcpSocket>();
				if (listener.accept(*new_socket) == sf::Socket::Done) {
					printf("Client connected\n");
					// Add the new client to the selector so that we will
					// be notified when he sends something
					selector.add(*new_socket);


					// Add the new client to the clients list
					clients.push_back(std::move(new_socket));

				}
			} else { // message received ?
				for (auto& client : clients) {
					if (selector.isReady(*client)) {
						// The client has sent some data, we can receive it
						sf::Packet packet;
						auto status = client->receive(packet);
						if (status == sf::Socket::Done) {
							auto* data = packet.getData();
							printf("Message received: %s\n", static_cast<const char*>(data ? data : "NO MESSAGE") );
						} else if (status == sf::Socket::Disconnected) {
							selector.remove(*client);
							client->disconnect();
							clients.erase(std::remove(std::begin(clients), std::end(clients), client), std::end(clients));
							printf("Client disconnected\n");
						}
					}
				}
			}
        }
    }
}
