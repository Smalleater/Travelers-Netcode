#include "TRA/ecs/systemManager.hpp"

namespace tra::ecs
{
	void SystemManager::beginUpdate(Engine* _engine)
	{
		for (size_t i = 0; i < m_beginUpdateSystems.size(); i++)
		{
			m_beginUpdateSystems[i]->update(_engine);
		}
	}

	void SystemManager::endUpdate(Engine* _engine)
	{
		for (size_t i = 0; i < m_endUpdateSystems.size(); i++)
		{
			m_endUpdateSystems[i]->update(_engine);
		}
	}
}