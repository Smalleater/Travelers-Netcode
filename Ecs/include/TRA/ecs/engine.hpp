#ifndef TRA_ECS_ENGINE_HPP
#define TRA_ECS_ENGINE_HPP

#include "TRA/export.hpp"

#include <memory>

#include "TRA/ecs/entityManager.hpp"
#include "TRA/ecs/systemManager.hpp"

namespace tra::ecs
{
	class Engine
	{
	public:
		Engine();
		~Engine() = default;

		TRA_API Entity CreateEntity();
		TRA_API void DeleteEntity(Entity _entity);

		template<typename System>
		void addBeginUpdateSystem()
		{
			m_systemManager->addBeginUpdateSystem<System>();
		}

		template<typename System>
		void addEndUpdateSystem()
		{
			m_systemManager->addEndUpdateSystem<System>();
		}

		TRA_API void beginUpdate();
		TRA_API void endUpdate();

	private:
		std::unique_ptr<EntityManager> m_entityManager;
		std::unique_ptr<SystemManager> m_systemManager;
	};
}

#endif
