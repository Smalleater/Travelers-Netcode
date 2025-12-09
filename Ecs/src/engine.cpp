#include "TRA/ecs/engine.hpp"

#include "TRA/ecs/systemManager.hpp"
#include "entityManager.hpp"


namespace tra::ecs
{
	Engine::Engine()
	{
		m_entityManager = std::make_unique<EntityManager>();
		m_systemManager = std::make_unique<SystemManager>();
	}

	void Engine::beginUpdate()
	{
		m_systemManager->beginUpdate(this);
	}
}