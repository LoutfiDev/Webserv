#include "ServerManager.hpp"
#include <cstddef>
#include <vector>


ServerManager::ServerManager(Parser &parser) {
	config = parser;
}

ServerManager::ServerManager(const ServerManager& obj) {
	(void)obj;
}

ServerManager &ServerManager::operator=(const ServerManager& obj)
{
	if (this == &obj)
		return (*this);
	return (*this);
}

void ServerManager::start()
{
	std::vector<Server *> servers;

	servers = config.get_servers();
	std::cout << servers.size() << "\n";
	
	for (size_t i = 0; i < servers.size(); i++) {
		std::cout << i << "\n";
		std::cout << servers[i]->port << "\n";
	
	}
}

ServerManager::~ServerManager() {
}
