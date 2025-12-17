#ifndef TRA_ECS_COMPONENT_MANAGER_HPP
#define TRA_ECS_COMPONENT_MANAGER_HPP

#include "TRA/debugUtils.hpp"
#include "TRA/ecs/sparseSet.hpp"

namespace tra::ecs
{
	class ComponentManager
	{
	public:
		ComponentManager() = default;
		~ComponentManager() = default;

		template<typename Component>
		void addComponentToEntity(const Entity& _entity, const Component& _component)
		{
			std::weak_ptr<SparseSet<Component>> sparseSet = getOrCreateComponentSparseSet<Component>();
			std::shared_ptr<SparseSet<Component>> lockedSparseSet = sparseSet.lock();

			if (lockedSparseSet == nullptr)
			{
				TRA_ERROR_LOG("Ecs: Failed to add component to entity. SparseSet is expired.");
				return;
			}

			lockedSparseSet->insert(_entity, _component);
		}

		template<typename Component>
		void removeComponentFromEntity();

		template<typename ...Component>
		void querryEntityWith();

		template<typename ...Component>
		void querryEntityWith(const std::vector<Entity>& _entitiesToQuery);

		template<typename ...Component>
		void querryEntityWithout();

		template<typename ...Component>
		void querryEntityWithout(const std::vector<Entity>& _entitiesToQuery);

	private:
		std::unordered_map<size_t, std::shared_ptr<ISparseSet>> m_sparseSets;

		template<typename Component>
		std::weak_ptr<SparseSet<Component>> getOrCreateComponentSparseSet()
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
