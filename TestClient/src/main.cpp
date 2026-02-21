#include <thread>
#include <chrono>

#include "TRA/netcode/client/client.hpp"

#include "TRA/Netcode/engine/message.hpp"

using namespace tra;
using namespace tra::netcode::engine;
using namespace tra::netcode::client;

DECLARE_MESSAGE_BEGIN(HelloWorld)
FIELD(std::vector<uint32_t>, m_vector)
DECLARE_MESSAGE_END()

int main() {
	
	ErrorCode ec;

	ec = Client::Get()->connectTo("127.0.0.1", 2025);
	if (ec != ErrorCode::Success) return -1;

	std::shared_ptr<message::HelloWorld> message = std::make_shared<message::HelloWorld>();
	//message->string = "Hello World from client!";
	message->m_vector.push_back(1);
	message->m_vector.push_back(2);
	message->m_vector.push_back(3);
	message->m_vector.push_back(4);
	message->m_vector.push_back(5);

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
					//std::cout << "Received from server: " << helloMessage->string << std::endl;
					std::cout << "Value: ";
					for (const auto& element : helloMessage->m_vector)
					{
						std::cout << element << ", ";
					}
					std::cout << std::endl;
				}

				Client::Get()->sendTcpMessage(message);
			}

			Client::Get()->endUpdate();
		}
	}

	return 0;
}