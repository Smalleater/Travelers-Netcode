#include "TRA/ecs/entityBuffer.hpp"

#include "TRA/ecs/componentManager.hpp"

namespace tra::ecs
{
	EntityBuffer::EntityBuffer(const std::shared_ptr<ComponentManager>& _componentManager)
	{
		m_componentManager = _componentManager;
	}

	void EntityBuffer::addEntity(const Entity _entity)
	{
		m_entityToIndex[_entity] = m_entities.size();
		m_entities.push_back(_entity);
	}

	void EntityBuffer::removeEntity(const Entity _entity)
	{
		auto it = m_entityToIndex.find(_entity);
		if (it != m_entityToIndex.end())
		{
			size_t index = it->second;
			size_t lastIndex = m_entities.size() - 1;

			if (index != lastIndex)
			{
				m_entities[index] = m_entities[lastIndex];
				m_entityToIndex[m_entities[index]] = index;
			}

			m_entities.pop_back();
			m_entityToIndex.erase(it);
		}
	}


}