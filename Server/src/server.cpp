#include "TRA/netcode/server/server.hpp"

#include "TRA/debugUtils.hpp"

#include "TRA/netcode/engine/tags.hpp"
#include "TRA/netcode/engine/message.hpp"

namespace tra::netcode::server
{
	Server* Server::m_singleton = nullptr;

	Server::Server()
	{
		m_networkEngine = new engine::NetworkEngine();
	}

	Server::~Server()
	{
		Stop();
	}

	Server* Server::Get()
	{
		if (m_singleton == nullptr)
		{
			m_singleton = new Server();
		}

		return m_singleton;
	}

	ErrorCode Server::Start(uint16_t _port, uint8_t _tickRate)
	{
		if (isRunning())
		{
			TRA_DEBUG_LOG("Server: Start called but server is already running.");
			return ErrorCode::ServerAlreadyStarted;
		}

		if (!m_networkEngine)
		{
			TRA_ERROR_LOG("Server: Network engine is not initialized.");
			return ErrorCode::NetworkEngineNotInitialized;
		}

		ErrorCode ec;

		ec = m_networkEngine->startTcpListenOnPort(_port, false);
		if (ec != ErrorCode::Success)
		{
			return ec;
		}

		/*ec = m_networkEngine->startUdpOnPort(_port, false);
		if (ec != ErrorCode::Success)
		{
			m_networkEngine->stopTcpListen();
			return ec;
		}*/

		m_networkEngine->resetTickSystem();
		m_networkEngine->setTickRate(_tickRate);

		TRA_INFO_LOG("Server: Started successfully on port %d.", _port);
		return ErrorCode::Success;
	}

	ErrorCode Server::Stop()
	{
		if (!isRunning())
		{
			TRA_DEBUG_LOG("Server: Stop called but server is not running.");
			return ErrorCode::Success;
		}

		if (!m_networkEngine)
		{
			TRA_ERROR_LOG("Server: Network engine is not initialized.");
			return ErrorCode::NetworkEngineNotInitialized;
		}

		m_networkEngine->stopTcpListen();
		//ErrorCode ecUdp = m_networkEngine->stopUdp();

		TRA_INFO_LOG("Server: Stopped successfully.");
		return ErrorCode::Success;
	}

	bool Server::isRunning() const
	{
		return m_networkEngine->getEcsWorld()->hasTag<engine::tags::ListeningTag>(m_networkEngine->getSelfEntity());
	}

	TRA_API bool Server::canUpdateNetcode()
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

		m_networkEngine->endUpdate();
	}

	ecs::World* Server::getEcsWorld()
	{
		return m_networkEngine->getEcsWorld();
	}

	ErrorCode Server::sendTcpMessage(ecs::Entity _entity, std::shared_ptr<engine::Message> _message)
	{
		if (!isRunning())
		{
			TRA_ERROR_LOG("Server: Cannot send TCP message, server is not running.");
			return ErrorCode::ServerNotRunning;
		}

		return m_networkEngine->sendTcpMessage(_entity, _message);
	}

	std::vector<std::shared_ptr<engine::Message>> Server::getTcpMessages(ecs::Entity _entity, const std::string& _messageType)
	{
		if (!isRunning())
		{
			return {};
		}

		return m_networkEngine->getTcpMessages(_entity, _messageType);
	}
}