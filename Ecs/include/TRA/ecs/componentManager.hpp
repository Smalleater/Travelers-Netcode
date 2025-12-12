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
		template<typename Component>
		std::unordered_map<size_t, std::unique_ptr<SparseSet<Component>>> m_sparseSets;

		template<typename Component>
		const std::unique_ptr<SparseSet<Component>> getOrCreateSparseSet()
		{
			static_assert(static_assert(std::is_base_of<IComponent, Component>::value, "Ecs: Component must derive from IComponent"););

			size_t hashCode = typeid(Component).hash_code();
		}
	};
}

#endif
