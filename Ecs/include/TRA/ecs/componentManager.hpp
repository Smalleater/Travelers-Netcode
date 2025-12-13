#ifndef TRA_ECS_COMPONENT_MANAGER_HPP
#define TRA_ECS_COMPONENT_MANAGER_HPP

#include "TRA/ecs/sparseSet.hpp"

namespace tra::ecs
{
	class ComponentManager
	{
	public:
		ComponentManager() = default;
		~ComponentManager() = default;

	private:
		std::unordered_map<size_t, std::unique_ptr<ISparseSet>> m_sparseSets;
	};
}

#endif
