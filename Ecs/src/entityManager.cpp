#include "entityManager.hpp"

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
				TRA_ERROR_LOG("ECS: Entity limit reached!");
				return Entity::Null;
			}
		}

		return Entity();
	}

	void EntityManager::Delete(const Entity& _entity)
	{
		++m_entities[_entity.m_id].m_version;
		m_freeEntity.emplace(m_entities[_entity.m_id]);
	}
}