#ifndef TRA_ECS_SPARSE_SET_HPP
#define TRA_ECS_SPARSE_SET_HPP

#include <vector>
#include <type_traits>
#include <limits>
#include <unordered_map>

#include "TRA/debugUtils.hpp"

#include "TRA/ecs/entity.hpp"
#include "TRA/ecs/iComponent.hpp"

namespace tra::ecs
{
	struct ISparseSet
	{
		virtual ~ISparseSet() = default;
	};

	template<typename Component>
	class SparseSet : public ISparseSet
	{
	public:
		SparseSet() = default;
		~SparseSet() = default;

		void insert(const Entity _entity, const Component& _component)
		{
			static_assert(std::is_base_of<IComponent, Component>::value, "Ecs: Component must derive from IComponent");

			if (m_sparse.find(_entity) != m_sparse.end())
			{
				TRA_WARNING_LOG("Ecs: Attempted to insert duplicate component for entity with Id: %I32u:%I32u", _entity.m_id, _entity.m_version);
				return;
			}

			if (m_dense.size() >= std::numeric_limits<size_t>::max())
			{
				TRA_ERROR_LOG("Ecs: SparseSet component limit reached");
				return;
			}

			m_sparse[_entity] = m_dense.size();
			m_dense.emplace_back(_component);
		}

	private:
		std::vector<Component> m_dense;
		std::unordered_map<Entity, size_t> m_sparse;
	};
}

#endif