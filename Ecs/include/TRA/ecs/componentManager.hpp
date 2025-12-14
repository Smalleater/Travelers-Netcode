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
		std::unordered_map<size_t, std::shared_ptr<ISparseSet>> m_sparseSets;

		template<typename Component>
		std::weak_ptr<SparseSet<Component>> getOrCreateComponentStore()
		{
			static_assert(std::is_base_of<IComponent, Component>::value, "Ecs: Component must derive from IComponent");

			size_t hashCode = typeid(Component).hash_code();
			auto it = m_sparseSets.find(hashCode);
			if (it != m_sparseSets.end())
			{
				return std::static_pointer_cast<SparseSet<Component>>(it->second);
			}

			m_sparseSets[hashCode] = std::make_shared<SparseSet<Component>>();
			return std::static_pointer_cast<SparseSet<Component>>(m_sparseSets[hashCode]);
		}
	};
}

#endif
