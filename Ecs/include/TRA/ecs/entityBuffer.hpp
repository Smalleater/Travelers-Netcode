#ifndef TRA_ECS_ENTITY_BUFFER_HPP
#define TRA_ECS_ENTITY_BUFFER_HPP

#include "TRA/export.hpp"

#include <vector>
#include <memory>

#include "TRA/ecs/entity.hpp"
#include "componentManager.hpp"

namespace tra::ecs
{
	class ComponentManager;

	class EntityBuffer
	{
	public:
		TRA_API EntityBuffer(const std::shared_ptr<ComponentManager>& _componentManager);
		TRA_API EntityBuffer(const std::shared_ptr<ComponentManager>& _componentManager, std::vector<Entity>&& _entities);
		TRA_API ~EntityBuffer() = default;

		TRA_API void addEntity(const Entity _entity);
		TRA_API void removeEntity(const Entity _entity);

		TRA_API std::vector<Entity> getEntities();

		template<typename... Component>
		std::shared_ptr<EntityBuffer> queryEntityWith()
		{
			std::shared_ptr<ComponentManager> componentManager = m_componentManager.lock();
			if (componentManager == nullptr)
			{
				TRA_ERROR_LOG("Ecs: Failed to access ComponentManager, weak_ptr has expired");
				return nullptr;
			}

			std::vector<Entity> resultEntities;
			resultEntities = componentManager->queryEntityWith<Component...>(m_entities);
			return std::make_shared<EntityBuffer>(componentManager, std::move(resultEntities));
		}

		template<typename... Component>
		std::shared_ptr<EntityBuffer> queryEntityWithout()
		{
			std::shared_ptr<ComponentManager> componentManager = m_componentManager.lock();
			if (componentManager == nullptr)
			{
				TRA_ERROR_LOG("Ecs: Failed to access ComponentManager, weak_ptr has expired");
				return nullptr;
			}
			
			std::vector<Entity> resultEntities;
			resultEntities = componentManager->queryEntityWithout<Component...>(m_entities);
			return std::make_shared<EntityBuffer>(componentManager, std::move(resultEntities));
		}

	private:
		std::weak_ptr<ComponentManager> m_componentManager;

		std::vector<Entity> m_entities;
	};
}

#endif
