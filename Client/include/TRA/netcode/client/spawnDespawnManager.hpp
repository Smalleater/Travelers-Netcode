#ifndef TRA_NETCODE_CLIENT_SPAWN_DESPAWN_MANAGER_HPP
#define TRA_NETCODE_CLIENT_SPAWN_DESPAWN_MANAGER_HPP

#include <vector>
#include <deque>

#include "TRA/netcode/engine/spawnDespawn.hpp"

namespace tra::netcode::client
{
	class SpawnDespawnManager
	{
	public:
		SpawnDespawnManager() = default;
		~SpawnDespawnManager() = default;

		void addSpawn(std::vector<engine::Spawn>& _spawns);
		void addDespawn(std::vector<engine::Despawn>& _despawn);

		bool tryGetSpawn(engine::Spawn& _spawn);
		bool tryGetDespawn(engine::Despawn& _despawn);

		void clear();

	private:
		std::deque<engine::Spawn> m_spawns;
		std::deque<engine::Despawn> m_despawns;
	};
}

#endif
