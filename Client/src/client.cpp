#include "TRA/netcode/client/client.hpp"

#include "TRA/debugUtils.hpp"

#include "TRA/netcode/engine/tags.hpp"
#include "TRA/netcode/engine/message.hpp"

#include "TRA/netcode/client/tags.hpp"
#include "TRA/netcode/client/messages.hpp"

namespace tra::netcode::client
{
	Client* Client::m_singleton = nullptr;

	Client::~Client()
	{
		disconnect();
	}

	Client* Client::Get()
	{
		if (m_singleton == nullptr)
		{
			m_singleton = new Client();
		}
		return m_singleton;
	}

	ErrorCode Client::connectTo(const std::string& _address, uint16_t _port)
	{
		TRA_ASSERT_REF_PTR_OR_COPIABLE(_address);

		if (isConnected())
		{
			TRA_DEBUG_LOG("Client: ConnectTo called but client is already connected.");
			return ErrorCode::ClientAlreadyConnected;
		}

		m_networkEngine = std::make_unique<engine::NetworkEngine>();

		ErrorCode ec;

		ec = m_networkEngine->startTcpConnectToAddress(_address, _port, false);
		if (ec != ErrorCode::Success)
		{
			m_networkEngine.reset();
			return ec;
		}

		/*ec = m_networkEngine->startUdpOnPort(0, false);
		if (ec != ErrorCode::Success)
		{
			m_networkEngine->stopTcpConnect();
			return ec;
		}*/

		TRA_INFO_LOG("Client: Successfully connected to server at %s:%d.", _address.c_str(), _port);
		return ErrorCode::Success;
	}

	ErrorCode Client::disconnect()
	{
		if (!isConnected())
		{
			TRA_DEBUG_LOG("Client: Disconnect called but client is not connected.");
			return ErrorCode::Success;
		}

		m_networkEngine->stopTcpConnect();
		//ErrorCode ecUdp = m_networkEngine->stopUdp();

		m_networkEngine.reset();

		TRA_INFO_LOG("Client: Disconnected successfully.");
		return ErrorCode::Success;
	}

	bool Client::isConnected() const
	{
		return m_networkEngine && m_networkEngine->getEcsWorld()->hasTag<engine::tags::ConnectedTag>(m_networkEngine->getSelfEntity());
	}

	bool Client::isReady() const
	{
		return m_networkEngine && m_networkEngine->getEcsWorld()->hasTag<tags::ClientIsReadyTag>(m_networkEngine->getSelfEntity());
	}

	uint32_t Client::getCurrentTick()
	{
		return m_networkEngine->getCurrentTick();
	}

	float Client::getFixedDeltaTime()
	{
		return m_networkEngine->getFixedDeltaTime();
	}

	bool Client::canUpdateNetcode()
	{
		if (!isConnected())
		{
			return false;
		}

		return m_networkEngine->canUpdateNetcode();
	}

	void Client::updateElapsedTime()
	{
		m_networkEngine->updateElapsedTime();
	}

	void Client::beginUpdate()
	{
		if (!isConnected())
		{
			TRA_ERROR_LOG("Client: Cannot begin update, client is not connected.");
			return;
		}

		m_networkEngine->beginUpdate();
	}

	void Client::endUpdate()
	{
		if (!isConnected())
		{
			TRA_ERROR_LOG("Client: Cannot end update, client is not connected.");
			return;
		}

		receiveInitializeClient();

		m_networkEngine->endUpdate();
	}

	ecs::World* Client::getEcsWorld()
	{
		return m_networkEngine->getEcsWorld();
	}

	ErrorCode Client::sendTcpMessage(std::shared_ptr<engine::Message> _message)
	{
		if (!isConnected())
		{
			TRA_ERROR_LOG("Client: Cannot send TCP message, client is not connected.");
			return ErrorCode::ClientNotConnected;
		}

		ecs::Entity selfEntity = m_networkEngine->getSelfEntity();

		if (!m_networkEngine->getEcsWorld()->hasTag<tags::ClientIsReadyTag>(selfEntity))
		{
			TRA_ERROR_LOG("Client: Cannot send TCP message to server, client is not ready.");
			return ErrorCode::ClientIsNotReady;
		}

		return m_networkEngine->sendTcpMessage(selfEntity, _message);
	}

	std::vector<std::shared_ptr<engine::Message>> Client::getTcpMessages(const std::string& _messageType)
	{
		if (!isConnected())
		{
			TRA_ERROR_LOG("Client: Cannot get TCP message, client is not connected.");
			return {};
		}

		ecs::Entity selfEntity = m_networkEngine->getSelfEntity();

		if (!m_networkEngine->getEcsWorld()->hasTag<tags::ClientIsReadyTag>(selfEntity))
		{
			TRA_ERROR_LOG("Client: Cannot get TCP message to server, client is not ready.");
			return {};
		}

		return m_networkEngine->getTcpMessages(selfEntity, _messageType);
	}

	void Client::receiveInitializeClient()
	{
		ecs::Entity selfEntity = m_networkEngine->getSelfEntity();

		auto& initializeClientMessages = m_networkEngine->getTcpMessages(selfEntity, "InitializeClientMessage");
		if (initializeClientMessages.size() > 0)
		{
			auto initializeClientMessage = static_cast<message::InitializeClientMessage*>(initializeClientMessages.at(0).get());

			m_networkEngine->setTickRate(initializeClientMessage->m_tickRate);
			m_networkEngine->resetElapsedTime();

			m_networkEngine->getEcsWorld()->addTag<tags::ClientIsReadyTag>(selfEntity);

			auto clientIsReadyMessage = std::make_shared<message::ClientIsReadyMessage>();
			m_networkEngine->sendTcpMessage(selfEntity, clientIsReadyMessage);
		}
	}
}