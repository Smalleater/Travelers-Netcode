#ifndef TRA_ECS_SYSTEM_MANAGER_HPP
#define TRA_ECS_SYSTEM_MANAGER_HPP

#include <vector>
#include <memory>

#include "TRA/debugUtils.hpp"
#include "TRA/ecs/iSystem.hpp"

namespace tra::ecs
{
	class Engine;

	class SystemManager
	{
	public:
		SystemManager() = default;
		~SystemManager() = default;

		template<typename System>
		void addBeginUpdateSystem()
		{
			static_assert(std::is_base_of<ISystem, System>::value, "Ecs: System must derive from ISystem");

			for (const auto& sys : m_beginUpdateSystems)
			{
				if (typeid(*sys) == typeid(System))
				{
					TRA_WARNING_LOG("Ecs: Attempted to add duplicate begin update system");
					return;
				}
			}

			m_beginUpdateSystems.emplace_back(std::make_unique<System>());
		}

		template<typename System>
		void addEndUpdateSystem()
		{
			static_assert(std::is_base_of<ISystem, System>::value, "Ecs: System must derive from ISystem");

			for (const auto& sys : m_endUpdateSystems)
			{
				if (typeid(*sys) == typeid(System))
				{
					TRA_WARNING_LOG("Ecs: Attempted to add duplicate end update system");
					return;
				}
			}

			m_endUpdateSystems.emplace_back(std::make_unique<System>());
		}

		void beginUpdate(Engine* _engine);
		void endUpdate(Engine* _engine);

	private:
		std::vector<std::unique_ptr<ISystem>> m_beginUpdateSystems;
		std::vector<std::unique_ptr<ISystem>> m_endUpdateSystems;
	};
}

#endif
