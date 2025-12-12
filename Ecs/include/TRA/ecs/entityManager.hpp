#ifndef TRA_ECS_ENTITY_MANAGER_HPP
#define TRA_ECS_ENTITY_MANAGER_HPP

#include <queue>

#include "TRA/ecs/entity.hpp"

namespace tra::ecs
{
	class EntityManager
	{
	public:
		EntityManager() = default;
		~EntityManager() = default;

		Entity createEntity();
		void Delete(const Entity& _entity);

	private:
		std::vector<Entity> m_entities;
		std::queue<EntityId> m_freeEntity;
	};
}

#endif
