#ifndef TRA_ECS_ENTITY_BUFFER_HPP
#define TRA_ECS_ENTITY_BUFFER_HPP

#include <vector>

#include "TRA/ecs/entity.hpp"

namespace tra::ecs
{
	class EntityBuffer
	{
	public:
		EntityBuffer() = default;
		~EntityBuffer() = default;

		void addEntity(const Entity _entity);
		void removeEntity(const Entity _entity);

	private:
		std::vector<Entity> m_entities;
	};
}

#endif
