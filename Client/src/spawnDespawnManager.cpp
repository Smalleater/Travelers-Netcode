#include "TRA/netcode/client/spawnDespawnManager.hpp"

namespace tra::netcode::client
{
	void SpawnDespawnManager::addSpawn(std::vector<engine::Spawn>& _spawns)
	{
		m_spawns.insert(m_spawns.end(), _spawns.begin(), _spawns.end());
	}

	void SpawnDespawnManager::addDespawn(std::vector<engine::Despawn>& _despawn)
	{
		m_despawns.insert(m_despawns.end(), _despawn.begin(), _despawn.end());
	}

	bool SpawnDespawnManager::tryGetSpawn(engine::Spawn& _spawn)
	{
		if (!m_spawns.empty())
		{
			_spawn = m_spawns.front();
			m_spawns.pop_front();

			return true;
		}

		return false;
	}

	bool SpawnDespawnManager::tryGetDespawn(engine::Despawn& _despawn)
	{
		if (!m_despawns.empty())
		{
			_despawn = m_despawns.front();
			m_despawns.pop_front();

			return true;
		}

		return false;
	}

	void SpawnDespawnManager::clear()
	{
		m_spawns.clear();
		m_despawns.clear();
	}
}