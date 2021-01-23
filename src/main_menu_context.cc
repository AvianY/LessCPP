#include "main_menu_context.h"
#include "join_dialog_context.h"
#include "game_context.h"
#include "wall.h"

#include "ai.h"
#include "helpers.h"

#include <sstream>
#include <fstream>
#include "osdialog.h"
#include "nlohmann/json.hpp"
#include <iostream>

Context* MainMenuContext::processBackgroundTask() { return nullptr; }
Context* MainMenuContext::processEvent(const sf::Event & event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (quitButton.contains(m_mousepos)) quit = true;
		else if (SPGameButton.contains(m_mousepos)) return new GameContext(wall::generateNwallconfigs<9>(), GameMode::SINGLEPLAYER);
		else if (LPVPGameButton.contains(m_mousepos)) return new GameContext(wall::generateNwallconfigs<9>(), GameMode::LOCAL_PVP);
		else if (loadGameButton.contains(m_mousepos)) return load_game();
		else if (MPHGameButton.contains(m_mousepos)) return host_game();
		else if (MPCGameButton.contains(m_mousepos)) return join_game();
	}
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::J) return new JoinDialogContext();
	}

	return nullptr;
}

Context* MainMenuContext::host_game() {
	if (false) {
		std::string port_str;
		std::cout << "Please, enter port of player 2: (default " << tcp_port << ")\n";
		std::getline(std::cin, port_str);
		if (port_str.length() > 0)
			tcp_port = std::stoi(port_str);
	}
	return new GameContext(wall::generateNwallconfigs<9>(), tcp_port /*53012*/);
}

Context* MainMenuContext::join_game() {
	std::string json_string = get_game_tcp_packets(ip_player2, tcp_port);
	std::stringstream ss;
	ss << json_string;
	nlohmann::json game_json{};
	ss >> game_json;
	return new GameContext(game_json, ip_player2, tcp_port);
}

Context* MainMenuContext::load_game() {
	osdialog_filters* filters = osdialog_filters_parse("json:json");
	char* filename = osdialog_file(OSDIALOG_OPEN, ".json", "Select a json file", filters);

	if (filename != nullptr) {
		std::ifstream game_file(filename);
		free(filename);
		nlohmann::json game_json{};
		game_file >> game_json;
		return new GameContext(game_json, GameMode::SINGLEPLAYER);
	}
}

sf::Texture MainMenuContext::render() {
	text.setString("Less Game");

	rentex.clear();

	rentex.draw(text);
	rentex.draw(quitButton);
	rentex.draw(SPGameButton);
	rentex.draw(LPVPGameButton);
	rentex.draw(MPHGameButton);
	rentex.draw(MPCGameButton);
	rentex.draw(loadGameButton);
	
	rentex.display();

	return rentex.getTexture();
}

