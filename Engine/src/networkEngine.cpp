#include "TRA/netcode/engine/networkEngine.hpp"

#include "TRA/debugUtils.hpp"

#include "TRA/netcode/core/netUtils.hpp"

#include "TRA/netcode/engine/message.hpp"
#include "TRA/netcode/engine/tags.hpp"

#ifdef _WIN32
#include "TRA/netcode/core/wsaInitializer.hpp"
#endif

#include "internal/networkSystemRegistrar.hpp"
#include "internal/socketComponent.hpp"
#include "internal/messageComponent.hpp"

namespace tra::netcode::engine
{
	NetworkEngine::NetworkEngine()
	{
		//m_udpSocket = nullptr;

		m_ecsWorld = std::make_unique<ecs::World>();

		internal::NetworkSystemRegistrar::registerNetworkSystems(m_ecsWorld.get());

		m_selfEntity = m_ecsWorld->createEntity();
		m_ecsWorld->addTag<tags::SelfTag>(m_selfEntity);
	}

	NetworkEngine::~NetworkEngine()
	{
		//delete m_udpSocket;

		m_ecsWorld->destroyEntity(m_selfEntity);

		m_ecsWorld.reset();
	}

	ErrorCode NetworkEngine::startTcpListenOnPort(uint16_t _port, bool _blocking)
	{
		if (m_ecsWorld->hasComponent<internal::components::TcpListenSocketComponent>(m_selfEntity))
		{
			TRA_ERROR_LOG("NetworkEngine: Start TCP listen on port called but TCP listen socket is already open.");
			return ErrorCode::SocketAlreadyOpen;
		}

		if (!core::NetUtils::isValidPort(_port))
		{
			TRA_ERROR_LOG("NetworkEngine: Invalid port number %d for TCP listen socket.", _port);
			return ErrorCode::InvalidPortNumber;
		}

#ifdef _WIN32
		ErrorCode errorCode = core::WSAInitializer::Get()->Init();
		if (errorCode != ErrorCode::Success)
		{
			return errorCode;
		}

		TRA_DEBUG_LOG("NetworkEngine: WSA initialized successfully.");
#endif

		internal::components::TcpListenSocketComponent socketComponent;
		socketComponent.m_tcpSocket = std::make_unique<core::TcpSocket>();

		std::pair<ErrorCode, int> intPairResult;

		intPairResult = socketComponent.m_tcpSocket->bindSocket(_port);
		if (intPairResult.first != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to bind TCP listen socket on port %d. ErrorCode: %d", _port, static_cast<int>(intPairResult.first));
			stopTcpListen();
			return intPairResult.first;
		}

		intPairResult = socketComponent.m_tcpSocket->listenSocket();
		if (intPairResult.first != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to listen on TCP socket. ErrorCode: %d", static_cast<int>(intPairResult.first));
			stopTcpListen();
			return intPairResult.first;
		}

		intPairResult = socketComponent.m_tcpSocket->setBlocking(_blocking);
		if (intPairResult.first != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to set TCP listen socket blocking mode. ErrorCode: %d", static_cast<int>(intPairResult.first));
			stopTcpListen();
			return intPairResult.first;
		}

		m_ecsWorld->addComponent(m_selfEntity, std::move(socketComponent));

		TRA_DEBUG_LOG("NetworkEngine: TCP listen socket started on port %d.", _port);
		return ErrorCode::Success;
	}

	ErrorCode NetworkEngine::startTcpConnectToAddress(const std::string& _address, uint16_t _port, bool _blocking)
	{
		TRA_ASSERT_REF_PTR_OR_COPIABLE(_address);

		if (m_ecsWorld->hasComponent<internal::components::TcpConnectSocketComponent>(m_selfEntity))
		{
			TRA_ERROR_LOG("NetworkEngine: start TCP connect to address called but TCP connect socket is already open.");
			return ErrorCode::SocketAlreadyOpen;
		}

		if (!core::NetUtils::isValidIpV4Address(_address))
		{
			TRA_ERROR_LOG("NetworkEngine: Invalid IP address %s for TCP connect socket.", _address.c_str());
			return ErrorCode::InvalidIpAddress;
		}

		if (!core::NetUtils::isValidPort(_port))
		{
			TRA_ERROR_LOG("NetworkEngine: Invalid port number %d for TCP listen socket.", _port);
			return ErrorCode::InvalidPortNumber;
		}

#ifdef _WIN32
		ErrorCode errorCode = core::WSAInitializer::Get()->Init();
		if (errorCode != ErrorCode::Success)
		{
			return errorCode;
		}

		TRA_DEBUG_LOG("NetworkEngine: WSA initialized successfully.");
#endif

		internal::components::TcpConnectSocketComponent socketComponent;
		socketComponent.m_tcpSocket = std::make_unique<core::TcpSocket>();

		std::pair<ErrorCode, int> intPairResult;

		intPairResult = socketComponent.m_tcpSocket->connectTo(_address, _port);
		if (intPairResult.first != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to connect TCP socket to %s:%d. ErrorCode: %d", _address.c_str(), _port, static_cast<int>(intPairResult.first));
			TRA_INFO_LOG("NetworkEngine: TCP connect socket was not connected on port %d.", _port);
			return intPairResult.first;
		}

		intPairResult = socketComponent.m_tcpSocket->setBlocking(_blocking);
		if (intPairResult.first != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to set TCP connect socket blocking mode. ErrorCode: %d", static_cast<int>(intPairResult.first));
			TRA_INFO_LOG("NetworkEngine: TCP connect socket was not connected on port %d.", _port);
			return intPairResult.first;
		}

		m_ecsWorld->addComponent(m_selfEntity, std::move(socketComponent));

		m_ecsWorld->addComponent(m_selfEntity, internal::components::SendTcpMessageComponent{});
		m_ecsWorld->addComponent(m_selfEntity, internal::components::ReceiveTcpMessageComponent{});

		m_ecsWorld->addTag<tags::ConnectedTag>(m_selfEntity);

		TRA_DEBUG_LOG("NetworkEngine: TCP connect socket connected to %s:%d.", _address.c_str(), _port);

		return ErrorCode::Success;
	}

	/*ErrorCode NetworkEngine::startUdpOnPort(uint16_t _port, bool _blocking)
	{
		if (m_udpSocket)
		{
			TRA_ERROR_LOG("NetworkEngine: start UDP on port called but UDP socket is already open.");
			return ErrorCode::SocketAlreadyOpen;
		}

		if (!core::NetUtils::isValidPort(_port))
		{
			TRA_ERROR_LOG("NetworkEngine: Invalid port number %d for UDP socket.", _port);
			return ErrorCode::InvalidPortNumber;
		}

#ifdef _WIN32
		ErrorCode errorCode = core::WSAInitializer::Get()->Init();
		if (errorCode != ErrorCode::Success)
		{
			return errorCode;
		}

		TRA_DEBUG_LOG("NetworkEngine: WSA initialized successfully.");
#endif

		m_udpSocket = new core::UdpSocket();

		std::pair<ErrorCode, int> intPairResult;

		intPairResult = m_udpSocket->bindSocket(_port);
		if (intPairResult.first != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to bind UDP socket on port %d. ErrorCode: %d", _port, static_cast<int>(intPairResult.first));
			stopUdp();
			return intPairResult.first;
		}

		intPairResult = m_udpSocket->setBlocking(_blocking);
		if (intPairResult.first != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to set UDP socket blocking mode. ErrorCode: %d", static_cast<int>(intPairResult.first));
			stopUdp();
			return intPairResult.first;
		}

		if (_port == 0)
		{
			std::pair<ErrorCode, uint16_t> portResult = m_udpSocket->getPort();

			if (portResult.first != ErrorCode::Success)
			{
				TRA_ERROR_LOG("NetworkEngine: Failed to get assigned UDP port number. ErrorCode: %d", static_cast<int>(portResult.first));
				stopUdp();
				return portResult.first;
			}

			_port = portResult.second;
		}

		TRA_DEBUG_LOG("NetworkEngine: UDP socket started on port %d.", _port);
		return ErrorCode::Success;
	}*/

	ErrorCode NetworkEngine::stopTcpListen()
	{
		if (!m_ecsWorld->hasComponent<internal::components::TcpListenSocketComponent>(m_selfEntity))
		{
			TRA_DEBUG_LOG("NetworkEngine: Stop TCP listen called but TCP listen socket is not open.");
			return ErrorCode::Success;
		}

		auto socketComponentPtr = m_ecsWorld->getComponent<internal::components::TcpListenSocketComponent>(m_selfEntity);
		socketComponentPtr->m_tcpSocket->shutdownSocket();
		socketComponentPtr->m_tcpSocket->closeSocket();

		m_ecsWorld->removeComponent<internal::components::TcpListenSocketComponent>(m_selfEntity);

#ifdef _WIN32
		core::WSAInitializer::Get()->CleanUp();
		TRA_DEBUG_LOG("NetworkEngine: WSA cleaned up successfully.");
#endif

		TRA_DEBUG_LOG("NetworkEngine: TCP listen socket stopped.");
		return ErrorCode::Success;
	}

	ErrorCode NetworkEngine::stopTcpConnect()
	{
		if (!m_networkEcs->hasComponent<TcpConnectSocketComponent>(m_selfEntityId))
		{
			TRA_DEBUG_LOG("NetworkEngine: Stop TCP connect called but TCP connect socket is not open.");
			return ErrorCode::Success;
		}

		if (m_networkEcs->hasComponent<ConnectedComponentTag>(m_selfEntityId))
		{
			m_networkEcs->removeComponentFromEntity<ConnectedComponentTag>(m_selfEntityId);
		}
		else
		{
			TRA_DEBUG_LOG("NetworkEngine: Stop TCP listen called but ConnectedComponentTag is not present on self entity.");
		}

		ErrorCode removeResult;

		removeResult = m_networkEcs->removeComponentFromEntity<TcpConnectSocketComponent>(m_selfEntityId);
		if (removeResult != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to remove TcpConnectSocketComponent from self entity. ErrorCode: %d", static_cast<int>(removeResult));
		}

		removeResult = m_networkEcs->removeComponentFromEntity<NetworkRootComponentTag>(m_selfEntityId);
		if (removeResult != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to remove NetworkRootComponentTag from self entity. ErrorCode: %d", static_cast<int>(removeResult));
		}

		removeResult = m_networkEcs->removeComponentFromEntity<SendTcpMessageComponent>(m_selfEntityId);
		if (removeResult != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to remove SendTcpMessageComponent from self entity. ErrorCode: %d", static_cast<int>(removeResult));
		}

		removeResult = m_networkEcs->removeComponentFromEntity<ReceiveTcpMessageComponent>(m_selfEntityId);
		if (removeResult != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to remove ReceiveTcpMessageComponent from self entity. ErrorCode: %d", static_cast<int>(removeResult));
		}

#ifdef _WIN32
		core::WSAInitializer::Get()->CleanUp();
		TRA_DEBUG_LOG("NetworkEngine: WSA cleaned up successfully.");
#endif

		TRA_DEBUG_LOG("NetworkEngine: TCP connect socket stopped.");
		return ErrorCode::Success;
	}

	ErrorCode NetworkEngine::stopUdp()
	{
		if (!m_udpSocket)
		{
			TRA_DEBUG_LOG("NetworkEngine: Stop UDP called but UDP socket is not open.");
			return ErrorCode::Success;
		}

		m_udpSocket->closeSocket();
		delete m_udpSocket;
		m_udpSocket = nullptr;

#ifdef _WIN32
		core::WSAInitializer::Get()->CleanUp();
		TRA_DEBUG_LOG("NetworkEngine: WSA cleaned up successfully.");
#endif

		TRA_DEBUG_LOG("NetworkEngine: UDP socket stopped.");
		return ErrorCode::Success;
	}

	void NetworkEngine::beginUpdate()
	{
		m_ecsWorld ->beginUpdate();
	}

	void NetworkEngine::endUpdate()
	{
		m_networkEcs->endUpdate();
	}

	ErrorCode NetworkEngine::sendTcpMessage(ecs::Entity _entity, std::shared_ptr<Message> _message)
	{
		auto getSendTcpMessageComponentResult = m_networkEcs->getComponentOfEntity<SendTcpMessageComponent>(_entityId);
		if (getSendTcpMessageComponentResult.first != ErrorCode::Success)
		{
			TRA_ERROR_LOG("NetworkEngine: Failed to get SendTcpMessageComponent for entity %I32u. ErrorCode: %d", _entityId, static_cast<int>(getSendTcpMessageComponentResult.first));
			return getSendTcpMessageComponentResult.first;
		}

		std::shared_ptr<SendTcpMessageComponent> sendTcpMessageComponent = getSendTcpMessageComponentResult.second.lock();
		if (!sendTcpMessageComponent)
		{
			TRA_ERROR_LOG("NetworkEngine: SendTcpMessageComponent for entity %I32u is no longer valid.", _entityId);
			return ErrorCode::InvalidComponent;
		}

		sendTcpMessageComponent->m_messagesToSend.push_back(_message);
		return ErrorCode::Success;
	}

	std::vector<std::shared_ptr<Message>> NetworkEngine::getTcpMessages(EntityId _entityId, const std::string& _messageType)
	{
		if (!m_networkEcs->hasComponent<ReceiveTcpMessageComponent>(_entityId))
		{
			return {};
		}

		auto getComponentResult = m_networkEcs->getComponentOfEntity<ReceiveTcpMessageComponent>(_entityId);
		if (getComponentResult.first != ErrorCode::Success)
		{
			return {};
		}

		auto receiveTcpMessageComponent = getComponentResult.second.lock();
		if (!receiveTcpMessageComponent)
		{
			TRA_ERROR_LOG("NetworkEngine: ReceiveTcpMessageComponent for entity %I32u is no longer valid.", _entityId);
			return {};
		}

		auto it = receiveTcpMessageComponent->m_receivedMessages.find(_messageType);
		if (it == receiveTcpMessageComponent->m_receivedMessages.end())
		{
			return {};
		}

		std::vector<std::shared_ptr<Message>> messages;
		for (const auto& messagePtr : it->second)
		{
			messages.push_back(messagePtr);
		}

		return messages;
	}

	EntityId NetworkEngine::getSelfEntityId()
	{
		return m_selfEntityId;
	}
}