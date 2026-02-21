#include "TRA/netcode/engine/networkIdManager.hpp"

#include "internal/networkIdMessage.hpp"

namespace tra::netcode::engine
{
	NetworkIdManager::NetworkIdManager()
		: m_nextNetworkId(0)
	{

	}

	NetworkId NetworkIdManager::AddEntity(ecs::Entity _entity)
	{
		m_networkIdEntityLookup.insert({ m_nextNetworkId, _entity });
		++m_nextNetworkId;
	}

	void NetworkIdManager::removeNetworkId(const NetworkId _networkId)
	{
		m_networkIdEntityLookup.erase(_networkId);
	}

	void NetworkIdManager::clear()
	{
		m_networkIdEntityLookup.clear();

		m_newNetworkIds.clear();
		m_removedNetworkIds.clear();
	}

	void NetworkIdManager::sendUpdateMessage()
	{

	}
}