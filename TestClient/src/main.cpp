#include <thread>
#include <chrono>

#include "TRA/netcode/client/client.hpp"

#include "TRA/Netcode/engine/message.hpp"

using namespace tra;
using namespace tra::netcode::engine;
using namespace tra::netcode::client;

DECLARE_MESSAGE_BEGIN(HelloWorld)
FIELD(std::string, string)
DECLARE_MESSAGE_END()

int main() {
	
	ErrorCode ec;

	ec = Client::Get()->connectTo("127.0.0.1", 2025);
	if (ec != ErrorCode::Success) return -1;

	std::shared_ptr<message::HelloWorld> message = std::make_shared<message::HelloWorld>();
	message->string = "Hello World from client!";

	while (Client::Get()->isConnected())
	{
		Client::Get()->updateElapsedTime();
		while (Client::Get()->canUpdateNetcode())
		{
			Client::Get()->beginUpdate();

			if (Client::Get()->isReady())
			{
				auto getMessageResult = Client::Get()->getTcpMessages("HelloWorld");
				for (auto message : getMessageResult)
				{
					message::HelloWorld* helloMessage = static_cast<message::HelloWorld*>(message.get());
					std::cout << "Received from server: " << helloMessage->string << std::endl;
				}

				Client::Get()->sendTcpMessage(message);
			}

			Client::Get()->endUpdate();
		}
	}

	return 0;
}