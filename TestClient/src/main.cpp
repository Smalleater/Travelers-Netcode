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

TRA_NETCODE_DECLARE_NETWORK_COMPONENT_BEGIN(TestNetworkComponent)
TRA_NETCODE_DECLARE_NETWORK_COMPONENT_FIELD(float, m_x)
TRA_NETCODE_DECLARE_NETWORK_COMPONENT_FIELD(float, m_y)
TRA_NETCODE_DECLARE_NETWORK_COMPONENT_END()

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

	std::vector<NetworkId> networkIds;

	while (Client::Get()->isConnected())
	{
		Client::Get()->updateElapsedTime();
		while (Client::Get()->canUpdateNetcode())
		{
			Client::Get()->beginUpdate();

			if (Client::Get()->isReady())
			{
				//std::cout << "ClientId: " << Client::Get()->getClientId() << std::endl;

				//auto getMessageResult = Client::Get()->getTcpMessages("HelloWorld");
				//for (auto message : getMessageResult)
				//{
				//	message::HelloWorld* helloMessage = static_cast<message::HelloWorld*>(message.get());
				//	//std::cout << "Received from server: " << helloMessage->string << std::endl;
				//	std::cout << "Value: ";
				//	for (const auto& element : helloMessage->m_vector)
				//	{
				//		std::cout << element << ", ";
				//	}
				//	std::cout << std::endl;
				//}

				//Client::Get()->sendTcpMessage(message);

				Spawn spawn;
				while (Client::Get()->tryGetSpawn(spawn))
				{
					std::cout << "Spawn -"; 
					std::cout << " NetworkId: " << spawn.m_networkId;
					std::cout << " OwnerId: " << spawn.m_ownerId;
					std::cout << " Tag: " << spawn.m_tag << std::endl;

					networkIds.push_back(spawn.m_networkId);
				}

				Despawn despawn;
				while (Client::Get()->tryGetDespawn(despawn))
				{
					std::cout << "Despawn -";
					std::cout << " NetworkId: " << despawn.m_networkId << std::endl;

					auto it = std::find(networkIds.begin(), networkIds.end(), despawn.m_networkId);
					if (it != networkIds.end())
					{
						networkIds.erase(it);
					}
					else
					{
						std::cout << "Warning: Received despawn for unknown NetworkId: " << despawn.m_networkId << std::endl;
					}
				}

				for (auto& networkId : networkIds)
				{
					std::shared_ptr<NetworkComponent> componentPtr = Client::Get()->getNetworkComponentFromCurrentState(networkId, "TestNetworkComponent");
					if (componentPtr != nullptr)
					{
						auto testComponentPtr = dynamic_cast<networkComponent::TestNetworkComponent*>(componentPtr.get());

						if (testComponentPtr)
						{
							std::cout << "NetworkId: " << networkId << " - ";
							std::cout << "TestComponent - x: " << testComponentPtr->m_x << ", y: " << testComponentPtr->m_y << std::endl;
						}
						else
						{
							std::cout << "Failed to get TestComponent for NetworkId: " << networkId << std::endl;
						}
					}

					componentPtr = Client::Get()->getNetworkComponentFromBuffer(networkId, "TestNetworkComponent");
					if (componentPtr != nullptr)
					{
						auto testComponentPtr = dynamic_cast<networkComponent::TestNetworkComponent*>(componentPtr.get());

						if (testComponentPtr)
						{
							std::cout << "NetworkId: " << networkId << " - ";
							std::cout << "TestComponent from buffer - x: " << testComponentPtr->m_x << ", y: " << testComponentPtr->m_y << std::endl;
						}
						else
						{
							std::cout << "Failed to get TestComponent from buffer for NetworkId: " << networkId << std::endl;
						}
					}
				}
			}

			Client::Get()->endUpdate();
		}
	}

	return 0;
}