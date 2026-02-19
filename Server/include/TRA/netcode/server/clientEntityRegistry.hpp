#ifndef TRA_NETCODE_SERVER_CLIENT_ENTITY_REGISTRY_HPP
#define TRA_NETCODE_SERVER_CLIENT_ENTITY_REGISTRY_HPP

#include <cstdint>
#include <unordered_map>

#include "TRA/ecs/entity.hpp"

namespace tra::netcode::server
{
	using ClientId = uint32_t;

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

	private:
		std::unordered_map<ClientId, ecs::Entity> m_clientEntityLookup;
		std::unordered_map<ecs::Entity, ClientId> m_entityClientLookup;
	};
}

#endif
