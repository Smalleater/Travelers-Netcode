#include "TRA/netcode/server/server.hpp"

#include <thread>
#include <chrono>

#include "TRA/debugUtils.hpp"

#include "TRA/netcode/engine/message.hpp"
#include "TRA/netcode/engine/tags.hpp"

using namespace tra;
using namespace tra::netcode::engine;
using namespace tra::netcode::server;

DECLARE_MESSAGE_BEGIN(HelloWorld)
FIELD(std::string, string)
DECLARE_MESSAGE_END()

int main() {
	ErrorCode ec;

	ec = Server::Get()->Start(2025, 30);
	if (ec != ErrorCode::Success) return -1;

	std::shared_ptr<message::HelloWorld> message = std::make_shared<message::HelloWorld>();
	message->string = "Hello World from server!";

	while (Server::Get()->isRunning())
	{
		Server::Get()->updateElapsedTime();
		while (Server::Get()->canUpdateNetcode())
		{
			std::cout << "Update" << std::endl;

			Server::Get()->beginUpdate();

			for (auto& [entity] : Server::Get()->getEcsWorld()->queryEntities(
				ecs::WithComponent<>{},
				ecs::WithoutComponent<>{},
				ecs::WithTag<tags::ConnectedTag>{},
				ecs::WithoutTag<tags::SelfTag>{}))
			{
				auto getMessageResult = Server::Get()->getTcpMessages(entity, "HelloWorld");
				for (auto message : getMessageResult)
				{
					message::HelloWorld* helloMessage = static_cast<message::HelloWorld*>(message.get());
					std::cout << "Received from client " << entity.id() << ": " << helloMessage->string << std::endl;
				}

				Server::Get()->sendTcpMessage(entity, message);
			}

			Server::Get()->endUpdate();
		}
	}

	return 0;
}