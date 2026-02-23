#include "TRA/netcode/server/clientEntityRegistry.hpp"

namespace tra::netcode::server
{
	void ClientEntityRegistry::addClientId(const ClientId _clientId, const ecs::Entity _entity)
	{
		m_entities.push_back(_entity);
		m_clientEntityLookup.insert_or_assign(_clientId, m_entities.size() - 1);

		m_entityClientLookup.insert_or_assign(_entity, _clientId);
	}

	void ClientEntityRegistry::removeClientId(const ClientId _clientId)
	{
		auto it = m_clientEntityLookup.find(_clientId);
		if (it == m_clientEntityLookup.end()) return;

		m_entityClientLookup.erase(m_entities[it->second]);

		if (it->second != m_clientEntityLookup.size() - 1)
		{
			m_entities[it->second] = std::move(m_entities.back());
			m_clientEntityLookup[m_entityClientLookup[m_entities[it->second]]] = it->second;
		}
		m_entities.pop_back();
		m_clientEntityLookup.erase(it);
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

		return m_entities.at(it->second);
	}

	const std::vector<ecs::Entity> ClientEntityRegistry::getAllEntities() const
	{
		return m_entities;
	}
}