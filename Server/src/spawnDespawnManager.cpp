#include "TRA/netcode/server/spawnDespawnManager.hpp"

namespace tra::netcode::server
{
	const std::vector<engine::Spawn>& SpawnDespawnManager::getSpawns() const
	{
		return m_spawns;
	}

	const std::vector<engine::Spawn>& SpawnDespawnManager::getNewSpawns() const
	{
		return m_newSpawns;
	}

	const std::vector<engine::Despawn>& SpawnDespawnManager::getNewDespawns() const
	{
		return m_newDespawn;
	}

	void SpawnDespawnManager::addSpawnMessage(engine::NetworkId _networkId, ClientId _owner, uint16_t _tag)
	{
		engine::Spawn spawn;

		spawn._networkId = _networkId;
		spawn._ownerId = _owner;
		spawn._tag = _tag;

		m_newSpawns.push_back(spawn);
		m_spawns.push_back(spawn);
		m_spawnLookup.insert({ _networkId, m_spawns.size() - 1});
	}

	void SpawnDespawnManager::addDespawn(engine::NetworkId _networkId)
	{
		auto it = m_spawnLookup.find(_networkId);
		if (it == m_spawnLookup.end() || m_spawns.empty())
		{
			return;
		}

		if (it->second != m_spawns.size() - 1)
		{
			m_spawns[it->second] = std::move(m_spawns.back());
			m_spawnLookup[m_spawns[it->second]._networkId] = it->second;
		}

		m_spawns.pop_back();
		m_spawnLookup.erase(it);

		engine::Despawn despawn;
		despawn._networkId = _networkId;

		m_newDespawn.push_back(despawn);
	}

	void SpawnDespawnManager::clearNewSpawnDespawn()
	{
		m_newSpawns.clear();
		m_newDespawn.clear();
	}
}