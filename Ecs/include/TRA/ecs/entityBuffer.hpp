#ifndef TRA_ECS_ENTITY_BUFFER_HPP
#define TRA_ECS_ENTITY_BUFFER_HPP

#include <vector>
#include <memory>

#include "TRA/ecs/entity.hpp"

namespace tra::ecs
{
	class ComponentManager;

	class EntityBuffer
	{
	public:
		EntityBuffer(const std::shared_ptr<ComponentManager>& _componentManager);
		~EntityBuffer() = default;

		void addEntity(const Entity _entity);
		void removeEntity(const Entity _entity);



	private:
		std::weak_ptr<ComponentManager> m_componentManager;

		std::vector<Entity> m_entities;
		std::unordered_map<Entity, size_t> m_entityToIndex;
	};
}

#endif
