#include "TRA/netcode/engine/networkIdManager.hpp"

#include "TRA/netcode/engine/networkEngine.hpp"

namespace tra::netcode::engine
{
	NetworkIdManager::NetworkIdManager()
		: m_nextNetworkId(1)
	{

	}

	NetworkId NetworkIdManager::AddEntity(ecs::Entity _entity)
	{
		NetworkId networkId = m_nextNetworkId;
		++m_nextNetworkId;

		m_networkIdEntityLookup.insert({ networkId, _entity });
		
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
}