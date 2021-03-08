#include <server/game_room.h>

#include <iso646.h>

void GameRoom::exchange_packets() { // TODO: need to improve the logic when one is missing
    if (!m_p2_connected) return; /* no second player */

    sf::Packet packet;
    if (m_tcp_socket_p1->receive(packet) == sf::Socket::Status::Done) {
        m_tcp_socket_p2->send(packet);
    }
    packet.clear();
    if (m_tcp_socket_p2->receive(packet) == sf::Socket::Status::Done) {
        m_tcp_socket_p1->send(packet);
    }
    packet.clear();
}


void GameRoom::connect_p2(std::unique_ptr<sf::TcpSocket> tcp_socket_p2) {
    sf::Packet packet;
    m_tcp_socket_p2 = std::move(tcp_socket_p2);
    m_p2_connected = true;
    packet << m_game_json;
    m_tcp_socket_p2->send(packet);
}
