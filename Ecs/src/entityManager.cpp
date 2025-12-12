#include "TRA/ecs/entityManager.hpp"

#include <limits>

#include "TRA/debugUtils.hpp"

namespace tra::ecs
{
	Entity EntityManager::createEntity()
	{
		if (m_freeEntity.empty())
		{
			if (m_entities.size() >= std::numeric_limits<EntityId>::max())
			{
				TRA_ERROR_LOG("Ecs: Entity limit reached");
				return Entity::Null;
			}

			return m_entities.emplace_back(Entity{ static_cast<EntityId>(m_entities.size()), 0 });
		}

		EntityId entityId = m_freeEntity.front();
		m_freeEntity.pop();

		return m_entities[entityId];
	}

	void EntityManager::Delete(const Entity& _entity)
	{
		++m_entities[_entity.m_id].m_version;
		m_freeEntity.emplace(_entity.m_id);
	}
}