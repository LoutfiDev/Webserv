#ifndef MANAGER_SERVER_HPP
#define MANAGER_SERVER_HPP

#include <map>
#include <vector>

#include "Worker.hpp"
#include "../Parser/parser.hpp"

class ServerManager {
	public:
		// ServerManager();
		ServerManager(Parser &);
		ServerManager(const ServerManager &);
		ServerManager &operator=(const ServerManager &);


		void start();

		~ServerManager();

	private:
		std::map<int, std::vector<Worker>> nginx;
		Parser config; //Entry point to loop througth all server and create the workers
		
};

#endif //!MANAGER_SERVER_HPP
