#include "TRA/ecs/entityBuffer.hpp"

#include "TRA/ecs/componentManager.hpp"

namespace tra::ecs
{
	EntityBuffer::EntityBuffer(const std::shared_ptr<ComponentManager>& _componentManager)
	{
		m_componentManager = _componentManager;
	}

	EntityBuffer::EntityBuffer(const std::shared_ptr<ComponentManager>& _componentManager, std::vector<Entity>&& _entities)
	{
		m_componentManager = _componentManager;
		m_entities = std::move(_entities);
	}

	void EntityBuffer::addEntity(const Entity _entity)
	{
		m_entities.push_back(_entity);
	}

	void EntityBuffer::removeEntity(const Entity _entity)
	{
		auto it = std::find(m_entities.begin(), m_entities.end(), _entity);
		if (it != m_entities.end())
		{
			*it = m_entities.back();
			m_entities.pop_back();
		}
	}

	std::vector<Entity> EntityBuffer::getEntities()
	{
		return m_entities;
	}
}