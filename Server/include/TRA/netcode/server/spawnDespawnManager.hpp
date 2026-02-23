#ifndef TRA_NETCODE_SERVER_SPAWN_DESPAWN_MANAGER_HPP
#define TRA_NETCODE_SERVER_SPAWN_DESPAWN_MANAGER_HPP

#include <vector>
#include <unordered_map>

#include "TRA/netcode/engine/networkId.hpp"
#include "TRA/netcode/engine/spawnDespawn.hpp"

#include "TRA/netcode/server/clientId.hpp"

namespace tra::netcode::server
{
	class SpawnDespawnManager
	{
	public:
		SpawnDespawnManager() = default;
		~SpawnDespawnManager() = default;

		const std::vector<engine::Spawn>& getSpawns() const;
		const std::vector<engine::Spawn>& getNewSpawns() const;
		const std::vector<engine::Despawn>& getNewDespawns() const;

		void addSpawnMessage(engine::NetworkId _networkId, ClientId _owner, uint16_t _tag);
		void addDespawn(engine::NetworkId _networkId);

		void clearNewSpawnDespawn();

	private:
		std::vector<engine::Spawn> m_spawns;

		std::vector<engine::Spawn> m_newSpawns;
		std::vector<engine::Despawn> m_newDespawn;

		std::unordered_map<engine::NetworkId, size_t> m_spawnLookup;
	};
}

#endif
