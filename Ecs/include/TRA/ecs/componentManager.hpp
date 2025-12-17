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
			SparseSet<Component>* sparseSet = getOrCreateComponentSparseSet<Component>();
			sparseSet->insert(_entity, _component);
		}

		template<typename Component>
		void removeComponentFromEntity(const Entity& _entity)
		{
			SparseSet<Component>* sparseSet = getOrCreateComponentSparseSet<Component>();
			sparseSet->remove(_entity);
		}

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
		SparseSet<Component>* getOrCreateComponentSparseSet()
		{
			static_assert(std::is_base_of<IComponent, Component>::value, "Ecs: Component must derive from IComponent");

			size_t hashCode = typeid(Component).hash_code();
			auto it = m_sparseSets.find(hashCode);
			if (it != m_sparseSets.end())
			{
				return static_cast<SparseSet<Component>*>(it->second.get());
			}

			m_sparseSets[hashCode] = std::make_unique<SparseSet<Component>>();
			return static_cast<SparseSet<Component>*>(m_sparseSets[hashCode].get());
		}
	};
}

#endif
