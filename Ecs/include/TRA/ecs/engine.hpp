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
			assert(m_entityManager != nullptr && "Ecs: ComponentManager does not exist");
			m_componentManager->addComponentToEntity<Component>(_entity, _component);
		}

	private:
		std::unique_ptr<EntityManager> m_entityManager;
		std::unique_ptr<SystemManager> m_systemManager;
		std::unique_ptr<ComponentManager> m_componentManager;
	};
}

#endif
