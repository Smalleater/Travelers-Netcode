#include "TRA/ecs/engine.hpp"

#include "systemManager.hpp"

namespace tra::ecs
{
	Engine::Engine()
	{
		m_systemManager = std::make_unique<SystemManager>();
	}

	void Engine::beginUpdate()
	{
		m_systemManager->beginUpdate(this);
	}
}