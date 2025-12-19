#ifndef TRA_ECS_ENGINE_HPP
#define TRA_ECS_ENGINE_HPP

#include "TRA/export.hpp"

#include <memory>

#include "TRA/ecs/entityManager.hpp"
#include "TRA/ecs/systemManager.hpp"
#include "TRA/ecs/componentManager.hpp"
#include "TRA/ecs/entityBuffer.hpp"

namespace tra::ecs
{
	class Engine
	{
	public:
		TRA_API Engine();
		TRA_API ~Engine() = default;

		TRA_API Entity createEntity();
		TRA_API void deleteEntity(Entity _entity);

		template<typename System>
		void addBeginUpdateSystem()
		{
			assert(m_systemManager != nullptr && "Ecs: SystemManager does not exist");
			m_systemManager->addBeginUpdateSystem<System>();
		}

		template<typename System>
		void addEndUpdateSystem()
		{
			assert(m_systemManager != nullptr && "Ecs: SystemManager does not exist");
			m_systemManager->addEndUpdateSystem<System>();
		}

		TRA_API void beginUpdate();
		TRA_API void endUpdate();

		template<typename Component>
		void addComponentToEntity(const Entity& _entity, const Component& _component)
		{
			assert(m_componentManager != nullptr && "Ecs: ComponentManager does not exist");
			m_componentManager->addComponentToEntity<Component>(_entity, _component);
		}

		template<typename Component>
		void removeComponentFromEntity(const Entity& _entity)
		{
			assert(m_componentManager != nullptr && "Ecs: ComponentManager does not exist");
			m_componentManager->removeComponentFromEntity<Component>(_entity);
		}

		template<typename Component>
		Component* getEntityComponent(const Entity& _entity)
		{
			assert(m_componentManager != nullptr && "Ecs: ComponentManager does not exist");
			return m_componentManager->getEntityComponent<Component>(_entity);
		}

		template<typename Component>
		bool entityHasComponent(const Entity& _entity)
		{
			assert(m_componentManager != nullptr && "Ecs: ComponentManager does not exist");
			return m_componentManager->entityHasComponent<Component>(_entity);
		}

		template<typename... Component>
		std::vector<Entity> queryEntityWith(const std::vector<Entity>& _entities)
		{
			assert(m_componentManager != nullptr && "Ecs: ComponentManager does not exist");
			return m_componentManager->queryEntityWith<Component...>(_entities);
		}

		template<typename... Component>
		std::vector<Entity> queryEntityWithout(const std::vector<Entity>& _entities)
		{
			assert(m_componentManager != nullptr && "Ecs: ComponentManager does not exist");
			return m_componentManager->queryEntityWithout<Component...>(_entities);
		}

		TRA_API std::shared_ptr<EntityBuffer> queryEntity();

	private:
		std::unique_ptr<EntityManager> m_entityManager;
		std::unique_ptr<SystemManager> m_systemManager;
		std::shared_ptr<ComponentManager> m_componentManager;
		std::shared_ptr<EntityBuffer> m_entityBuffer;
	};
}

#endif
