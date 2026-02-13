#include <thread>
#include <chrono>

#include "TRA/debugUtils.hpp"

#include "TRA/netcode/server/server.hpp"
#include "TRA/netcode/server/tags.hpp"

#include "TRA/netcode/engine/message.hpp"
#include "TRA/netcode/engine/tags.hpp"

using namespace tra;
using namespace tra::netcode;
using namespace tra::netcode::engine;
using namespace tra::netcode::server;

DECLARE_MESSAGE_BEGIN(HelloWorld)
FIELD(std::string, string)
DECLARE_MESSAGE_END()

int main() {
	ErrorCode ec;

	ec = Server::Get()->start(2025, 60);
	if (ec != ErrorCode::Success) return -1;

	std::cout << "Fixed delta time value: " << Server::Get()->getFixedDeltaTime() << std::endl; 

	std::shared_ptr<message::HelloWorld> message = std::make_shared<message::HelloWorld>();
	message->string = "Hello World from server!";

	while (Server::Get()->isRunning())
	{
		Server::Get()->updateElapsedTime();
		while (Server::Get()->canUpdateNetcode())
		{
			Server::Get()->beginUpdate();

			for (auto& [entity] : Server::Get()->getEcsWorld()->queryEntities(
				ecs::WithComponent<>{},
				ecs::WithoutComponent<>{},
				ecs::WithTag<engine::tags::ConnectedTag, server::tags::ClientIsReadyTag>{},
				ecs::WithoutTag<engine::tags::SelfTag>{}))
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