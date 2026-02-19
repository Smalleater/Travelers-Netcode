#include <thread>
#include <chrono>

#include "TRA/debugUtils.hpp"

#include "TRA/netcode/server/server.hpp"
#include "TRA/netcode/server/tags.hpp"

#include "TRA/netcode/engine/message.hpp"
#include "TRA/netcode/engine/networkComponent.hpp"
#include "TRA/netcode/engine/tags.hpp"

using namespace tra;
using namespace tra::netcode::engine;
using namespace tra::netcode::server;

TRA_ECS_REGISTER_TAG(IsInitializedTag);

DECLARE_MESSAGE_BEGIN(HelloWorld)
FIELD(std::string, string)
DECLARE_MESSAGE_END()

TRA_NETCODE_DECLARE_NETWORK_COMPONENT_BEGIN(TestNetworkComponent)
TRA_NETCODE_DECLARE__NETWORK_COMPONENT_FIELD(int, m_int)
TRA_NETCODE_DECLARE_NETWORK_COMPONENT_END()

class Client
{
public:
	Client(ClientId _clientId) 
		: m_clientId(_clientId)
	{

	}

	~Client() = default;

	ClientId getClientId() const
	{
		return m_clientId;
	}

	void update()
	{
		auto getMessages = Server::Get()->getTcpMessages(m_clientId, "HelloWorld");

		for (auto message : getMessages)
		{
			message::HelloWorld* helloMessage = static_cast<message::HelloWorld*>(message.get());
			std::cout << "Received from client: " << m_clientId << ": " << helloMessage->string << std::endl;
		}

		std::shared_ptr<message::HelloWorld> message = std::make_shared<message::HelloWorld>();
		message->string = "Hello client: " + std::to_string(m_clientId) + ", from server!";

		Server::Get()->sendTcpMessage(m_clientId, message);
	}

private:
	ClientId m_clientId = NULL_CLIENT_ID;
};

int main() {
	ErrorCode ec;

	ec = Server::Get()->start(2025, 5);
	if (ec != ErrorCode::Success) return -1;

	std::cout << "Fixed delta time value: " << Server::Get()->getFixedDeltaTime() << std::endl;

	std::shared_ptr<message::HelloWorld> message = std::make_shared<message::HelloWorld>();
	message->string = "Hello World from server!";

	std::vector<Client> clients;

	while (Server::Get()->isRunning())
	{
		Server::Get()->updateElapsedTime();
		while (Server::Get()->canUpdateNetcode())
		{
			Server::Get()->beginUpdate();

			auto& disconnectedClientsid = Server::Get()->getDisconnectedClientIds();
			for (auto& clientId : disconnectedClientsid)
			{
				auto& it = std::find_if(clients.begin(), clients.end(),
					[clientId](const Client& _client) {
						return _client.getClientId() == clientId;
					});

				if (it != clients.end())
				{
					clients.erase(it);
				}
			}

			auto& newClientsId = Server::Get()->getNewClientIds();
			for (auto& clientId : newClientsId)
			{
				auto& it = std::find_if(clients.begin(), clients.end(),
					[clientId](const Client& _client) {
						return _client.getClientId() == clientId;
					});

				if (it == clients.end())
				{
					clients.emplace_back(clientId);
				}
			}

			for (auto& client : clients)
			{
				client.update();
			}

			Server::Get()->endUpdate();
		}
	}

	return 0;
}