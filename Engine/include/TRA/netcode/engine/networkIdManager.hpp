#ifndef TRA_NETCODE_ENGINE_NETWORK_ID_MANAGER_HPP
#define TRA_NETCODE_ENGINE_NETWORK_ID_MANAGER_HPP

#include "TRA/export.hpp"

#include <unordered_map>
#include <vector>

#include "TRA/ecs/entity.hpp"

#include "TRA/netcode/engine/networkId.hpp"

namespace tra::netcode::engine
{
	class NetworkEngine;

	class NetworkIdManager
	{
	public:
		NetworkIdManager();
		~NetworkIdManager() = default;

		NetworkId AddEntity(ecs::Entity _entity);
		void removeNetworkId(const NetworkId _networkId);

		ecs::Entity getEntity(const NetworkId _networkId);

		void clear();

		void sendUpdateMessage(NetworkEngine* _networkEngine);

	private:
		std::unordered_map<NetworkId, ecs::Entity> m_networkIdEntityLookup;

		std::vector<NetworkId> m_newNetworkIds;
		std::vector<NetworkId> m_removedNetworkIds;

		NetworkId m_nextNetworkId;
	};
}

#endif
