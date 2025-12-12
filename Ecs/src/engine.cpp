#include "TRA/ecs/engine.hpp"

#include "TRA/debugUtils.hpp"

namespace tra::ecs
{
	Engine::Engine()
	{
		m_entityManager = std::make_unique<EntityManager>();
		m_systemManager = std::make_unique<SystemManager>();
	}

	Entity Engine::CreateEntity()
	{
		assert(m_entityManager == nullptr && "Ecs: EntityManager does not exist");
		return m_entityManager->createEntity();
	}

	void Engine::DeleteEntity(Entity _entity)
	{
		assert(m_entityManager == nullptr && "Ecs: EntityManager does not exist");
		m_entityManager->Delete(_entity);
	}

	void Engine::beginUpdate()
	{
		assert(m_systemManager == nullptr && "Ecs: SystemManager does not exist");
		m_systemManager->beginUpdate(this);
	}

	void Engine::endUpdate()
	{
		assert(m_systemManager == nullptr && "Ecs: SystemManager does not exist");
		m_systemManager->endUpdate(this);
	}
}