#include "TRA/netcode/server/server.hpp"

#include <thread>
#include <chrono>

#include "TRA/debugUtils.hpp"

#include "TRA/netcode/engine/networkRootComponentTag.hpp"
#include "TRA/netcode/engine/message.hpp"
#include "TRA/netcode/engine/newConnectionComponent.hpp"
#include "TRA/netcode/engine/connectionStatusComponent.hpp"

using namespace tra;
using namespace tra::netcode::engine;
using namespace tra::netcode::server;

DECLARE_MESSAGE_BEGIN(HelloWorld)
FIELD(std::string, string)
DECLARE_MESSAGE_END()

int main() {
	ErrorCode ec;

	ec = Server::Get()->Start(2025);
	if (ec != ErrorCode::Success) return -1;

	EntityId selfEntityId = Server::Get()->getSelfEntityId();

	while (Server::Get()->isRunning())
	{
		Server::Get()->beginUpdate();

		std::vector<EntityId> queryIds = Server::Get()->queryEntityIds<NetworkRootComponentTag, ConnectedComponentTag>();
		for (size_t i = 0; i < queryIds.size(); i++)
		{
			if (queryIds[i] == selfEntityId) continue;

			auto getMessageResult = Server::Get()->getTcpMessages(queryIds[i], "HelloWorld");
			for (auto message : getMessageResult)
			{
				message::HelloWorld* helloMessage = static_cast<message::HelloWorld*>(message.get());
				std::cout << "Received from client " << queryIds[i] << ": " << helloMessage->string << std::endl;
			}

			std::shared_ptr<message::HelloWorld> message = std::make_shared<message::HelloWorld>();
			message->string = "Hello World from server!";
			Server::Get()->sendTcpMessage(queryIds[i], message);
		}

		Server::Get()->endUpdate();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	/*ec = Server::Get()->Stop();
	if (ec != ErrorCode::Success) return -1;*/

	return 0;
}