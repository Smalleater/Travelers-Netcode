#ifndef TRA_NETCODE_SERVER_CLIENT_ENTITY_REGISTRY_HPP
#define TRA_NETCODE_SERVER_CLIENT_ENTITY_REGISTRY_HPP

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "TRA/ecs/entity.hpp"

#include "TRA/netcode/server/clientId.hpp"

namespace tra::netcode::server
{
	constexpr uint32_t NULL_CLIENT_ID = 0;

	class ClientEntityRegistry
	{
	public:
		ClientEntityRegistry() = default;
		~ClientEntityRegistry() = default;

		void addClientId(const ClientId _clientId, const ecs::Entity _entity);
		void removeClientId(const ClientId _clientId);

		ClientId getClientId(const ecs::Entity _entity);
		ecs::Entity getEntity(const ClientId _clientId);
		const std::vector<ecs::Entity> getAllEntities() const;

	private:
		std::vector<ecs::Entity> m_entities;

		std::unordered_map<ClientId, size_t> m_clientEntityLookup;
		std::unordered_map<ecs::Entity, ClientId> m_entityClientLookup;
	};
}

#endif
