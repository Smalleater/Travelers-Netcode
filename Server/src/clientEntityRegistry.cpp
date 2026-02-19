#include "TRA/netcode/server/clientEntityRegistry.hpp"

namespace tra::netcode::server
{
	void ClientEntityRegistry::addClientId(const ClientId _clientId, const ecs::Entity _entity)
	{
		m_clientEntityLookup.insert_or_assign(_clientId, _entity);
		m_entityClientLookup.insert_or_assign(_entity, _clientId);
	}

	void ClientEntityRegistry::removeClientId(const ClientId _clientId)
	{
		ecs::Entity entity = getEntity(_clientId);
		if (!entity.isNull())
		{
			m_entityClientLookup.erase(entity);
		}

		m_clientEntityLookup.erase(_clientId);
	}

	ClientId ClientEntityRegistry::getClientId(const ecs::Entity _entity)
	{
		auto it = m_entityClientLookup.find(_entity);
		if (it == m_entityClientLookup.end())
		{
			return NULL_CLIENT_ID;
		}

		return it->second;
	}

	ecs::Entity ClientEntityRegistry::getEntity(const ClientId _clientId)
	{
		auto it = m_clientEntityLookup.find(_clientId);
		if (it == m_clientEntityLookup.end())
		{
			return ecs::Entity();
		}

		return it->second;
	}
}