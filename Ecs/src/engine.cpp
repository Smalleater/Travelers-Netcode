#include "TRA/ecs/engine.hpp"

#include "TRA/debugUtils.hpp"

namespace tra::ecs
{
	Engine::Engine()
	{
		m_entityManager = std::make_unique<EntityManager>();
		m_systemManager = std::make_unique<SystemManager>();
		m_componentManager = std::make_shared<ComponentManager>();
		m_entityBuffer = std::make_shared<EntityBuffer>(m_componentManager);
	}

	Entity Engine::createEntity()
	{
		assert(m_entityManager != nullptr && "Ecs: EntityManager does not exist");
		Entity newEntity = m_entityManager->createEntity();
		m_entityBuffer->addEntity(newEntity);
		return newEntity;
	}

	void Engine::deleteEntity(Entity _entity)
	{
		assert(m_entityManager != nullptr && "Ecs: EntityManager does not exist");
		m_entityBuffer->removeEntity(_entity);
		m_entityManager->Delete(_entity);
	}

	void Engine::beginUpdate()
	{
		assert(m_systemManager != nullptr && "Ecs: SystemManager does not exist");
		m_systemManager->beginUpdate(this);
	}

	void Engine::endUpdate()
	{
		assert(m_systemManager != nullptr && "Ecs: SystemManager does not exist");
		m_systemManager->endUpdate(this);
	}

	std::shared_ptr<EntityBuffer> Engine::queryEntity()
	{
		return m_entityBuffer;
	}
}