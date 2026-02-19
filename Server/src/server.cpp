#include "TRA/netcode/server/server.hpp"

#include "TRA/debugUtils.hpp"

#include "TRA/netcode/engine/tags.hpp"
#include "TRA/netcode/engine/message.hpp"

#include "TRA/netcode/server/tags.hpp"
#include "TRA/netcode/server/messages.hpp"

namespace tra::netcode::server
{
	Server* Server::m_singleton = nullptr;

	Server::~Server()
	{
		stop();
	}

	Server* Server::Get()
	{
		if (m_singleton == nullptr)
		{
			m_singleton = new Server();
		}

		return m_singleton;
	}

	ErrorCode Server::start(uint16_t _port, uint8_t _tickRate)
	{
		if (isRunning())
		{
			TRA_DEBUG_LOG("Server: Start called but server is already running.");
			return ErrorCode::ServerAlreadyStarted;
		}

		m_networkEngine = std::make_unique<engine::NetworkEngine>();

		ErrorCode ec;

		ec = m_networkEngine->startTcpListenOnPort(_port, false);
		if (ec != ErrorCode::Success)
		{
			m_networkEngine.reset();
			return ec;
		}

		/*ec = m_networkEngine->startUdpOnPort(_port, false);
		if (ec != ErrorCode::Success)
		{
			m_networkEngine->stopTcpListen();
			return ec;
		}*/

		m_nextClientId = 1;
		m_clientEntityRegistry = ClientEntityRegistry();

		m_networkEngine->setTickRate(_tickRate);
		m_networkEngine->resetElapsedTime();

		TRA_INFO_LOG("Server: Started successfully on port %d.", _port);
		return ErrorCode::Success;
	}

	ErrorCode Server::stop()
	{
		if (!isRunning())
		{
			TRA_DEBUG_LOG("Server: Stop called but server is not running.");
			return ErrorCode::Success;
		}

		m_networkEngine->stopTcpListen();
		//ErrorCode ecUdp = m_networkEngine->stopUdp();

		m_networkEngine.reset();

		TRA_INFO_LOG("Server: Stopped successfully.");
		return ErrorCode::Success;
	}

	bool Server::isRunning() const
	{
		return m_networkEngine && m_networkEngine->getEcsWorld()->hasTag<engine::tags::ListeningTag>(m_networkEngine->getSelfEntity());
	}

	uint32_t Server::getCurrentTick()
	{
		return m_networkEngine->getCurrentTick();
	}

	float Server::getFixedDeltaTime()
	{
		return m_networkEngine->getFixedDeltaTime();
	}

	std::vector<ClientId> Server::getNewClientIds()
	{
		return m_newClientIds;
	}

	std::vector<ClientId> Server::getDisconnectedClientIds()
	{
		return m_disconnectedClientIds;
	}

	bool Server::canUpdateNetcode()
	{
		if (!isRunning())
		{
			return false;
		}

		return m_networkEngine->canUpdateNetcode();
	}

	void Server::updateElapsedTime()
	{
		m_networkEngine->updateElapsedTime();
	}

	void Server::beginUpdate()
	{
		if (!isRunning())
		{
			TRA_ERROR_LOG("Server: Cannot begin update, server is not running.");
			return;
		}

		m_networkEngine->beginUpdate();
	}

	void Server::endUpdate()
	{
		if (!isRunning())
		{
			TRA_ERROR_LOG("Server: Cannot end update, server is not running.");
			return;
		}

		disconnectedClient();

		setClientReady();
		initializeNewClient();

		m_networkEngine->endUpdate();
	}

	ecs::World* Server::getEcsWorld()
	{
		return m_networkEngine->getEcsWorld();
	}

	bool Server::sendTcpMessage(const ClientId _clientId, std::shared_ptr<engine::Message> _message)
	{
		if (!isRunning())
		{
			TRA_ERROR_LOG("Server: Cannot send TCP message, server is not running.");
			return false;
		}

		const ecs::Entity entity = m_clientEntityRegistry.getEntity(_clientId);
		if (entity.isNull())
		{
			TRA_ERROR_LOG("Server: Cannot send TCP message, client with ID %u does not exist.", _clientId);
			return false;
		}

		if (!m_networkEngine->getEcsWorld()->hasTag<tags::ClientIsReadyTag>(entity))
		{
			TRA_ERROR_LOG("Server: Cannot send TCP message to entity: %ull, client is not ready.", entity.id());
			return false;
		}

		return m_networkEngine->sendTcpMessage(entity, _message) == ErrorCode::Success ? true : false;
	}

	std::vector<std::shared_ptr<engine::Message>> Server::getTcpMessages(const ClientId _clientId, const std::string& _messageType)
	{
		if (!isRunning())
		{
			TRA_ERROR_LOG("Server: Cannot get TCP message, server is not running.");
			return {};
		}

		const ecs::Entity entity = m_clientEntityRegistry.getEntity(_clientId);
		if (entity.isNull())
		{
			TRA_ERROR_LOG("Server: Cannot get TCP message, client with ID %u does not exist.", _clientId);
			return {};
		}

		if (!m_networkEngine->getEcsWorld()->hasTag<tags::ClientIsReadyTag>(entity))
		{
			TRA_ERROR_LOG("Server: Cannot get TCP message to entity: %ull, client is not ready.", entity.id());
			return {};
		}

		return m_networkEngine->getTcpMessages(entity, _messageType);
	}

	void Server::disconnectedClient()
	{
		m_disconnectedClientIds.clear();

		for (auto& [entity] : m_networkEngine->getEcsWorld()->queryEntities(
			ecs::WithComponent<>{},
			ecs::WithoutComponent<>{},
			ecs::WithTag<engine::tags::DisconnectedTag>{}))
		{
			ClientId clientId = m_clientEntityRegistry.getClientId(entity);
			if (clientId != NULL_CLIENT_ID)
			{
				m_disconnectedClientIds.push_back(clientId);
				m_clientEntityRegistry.removeClientId(clientId);
			}
		}
	}

	void Server::initializeNewClient()
	{
		ecs::World* world = m_networkEngine->getEcsWorld();

		auto& queryResult = world->queryEntities(
			ecs::WithComponent<>{},
			ecs::WithoutComponent<>{},
			ecs::WithTag<engine::tags::NewConnectionTag>{},
			ecs::WithoutTag<tags::WaitingClientIsReadyTag>{});

		if (queryResult.size() == 0)
		{
			return;
		}

		auto initializeClientMessage = std::make_shared<message::InitializeClientMessage>();
		initializeClientMessage->m_tickRate = m_networkEngine->getTickRate();

		for (auto& [entity] : queryResult)
		{
			m_networkEngine->sendTcpMessage(entity, initializeClientMessage);

			world->addTag<tags::WaitingClientIsReadyTag>(entity);
		}
	}

	void Server::setClientReady()
	{
		m_newClientIds.clear();

		ecs::World* world = m_networkEngine->getEcsWorld();

		auto& queryResult = world->queryEntities(
			ecs::WithComponent<>{},
			ecs::WithoutComponent<>{},
			ecs::WithTag<tags::WaitingClientIsReadyTag>{});

		if (queryResult.size() == 0)
		{
			return;
		}

		for (auto& [entity] : queryResult)
		{
			auto& clientIsReadyMessages = m_networkEngine->getTcpMessages(entity, "ClientIsReadyMessage");
			if (clientIsReadyMessages.size() == 0)
			{
				continue;
			}

			m_clientEntityRegistry.addClientId(m_nextClientId, entity);
			m_newClientIds.push_back(m_nextClientId);
			++m_nextClientId;

			world->removeTag<tags::WaitingClientIsReadyTag>(entity);
			world->addTag<tags::ClientIsReadyTag>(entity);
		}
	}
}