#include "TRA/netcode/engine/networkIdManager.hpp"

#include "TRA/netcode/engine/networkEngine.hpp"

#include "internal/networkIdMessage.hpp"

namespace tra::netcode::engine
{
	NetworkIdManager::NetworkIdManager()
		: m_nextNetworkId(0)
	{

	}

	NetworkId NetworkIdManager::AddEntity(ecs::Entity _entity)
	{
		NetworkId networkId = m_nextNetworkId;
		++m_nextNetworkId;

		m_networkIdEntityLookup.insert({ m_nextNetworkId, _entity });
		
		return networkId;
	}

	void NetworkIdManager::removeNetworkId(const NetworkId _networkId)
	{
		m_networkIdEntityLookup.erase(_networkId);
	}

	ecs::Entity NetworkIdManager::getEntity(const NetworkId _networkId)
	{
		auto it = m_networkIdEntityLookup.find(_networkId);
		if (it == m_networkIdEntityLookup.end())
		{
			return ecs::NULL_ENTITY;
		}

		return it->second;
	}

	void NetworkIdManager::clear()
	{
		m_networkIdEntityLookup.clear();

		m_newNetworkIds.clear();
		m_removedNetworkIds.clear();
	}

	void NetworkIdManager::sendUpdateMessage(NetworkEngine* _networkEngine)
	{
		if (m_newNetworkIds.size() >= 0 || m_removedNetworkIds.size() >= 0)
		{
			auto updateNetworkIdMessage = std::make_shared<tra::message::UpdateNetworkIdMessage>();

			updateNetworkIdMessage->m_newNetworkIds = m_newNetworkIds;
			updateNetworkIdMessage->m_removedNetworkIds = m_removedNetworkIds;

			_networkEngine->sendTcpMessage(_networkEngine->getSelfEntity(), updateNetworkIdMessage);

			m_newNetworkIds.clear();
			m_removedNetworkIds.clear();
		}
	}
}