#ifndef TRA_ECS_ENGINE_HPP
#define TRA_ECS_ENGINE_HPP

#include "TRA/export.hpp"

#include <memory>

namespace tra::ecs
{
	class SystemManager;

	class Engine
	{
	public:
		Engine();
		~Engine();

		TRA_API void beginUpdate();
		TRA_API void endUpdate();

	private:
		std::unique_ptr<SystemManager> m_systemManager;

	};
}

#endif
