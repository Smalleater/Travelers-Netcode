#ifndef TRA_ECS_SPARSE_SET_HPP
#define TRA_ECS_SPARSE_SET_HPP

#include <vector>
#include <type_traits>
#include <limits>
#include <unordered_map>
#include <optional>

#include "TRA/debugUtils.hpp"

#include "TRA/ecs/utils.hpp"
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

		void insert(const Entity& _entity, const Component& _component)
		{
			if (m_sparse.find(_entity) != m_sparse.end())
			{
				TRA_WARNING_LOG(("Ecs: Attempted to insert duplicate component for entity with Id: " + TRA_ENTITY_TO_STRING(_entity)).c_str());
				return;
			}

			m_sparse[_entity] = m_dense.size();
			m_dense.emplace_back(_component);
			m_denseToEntity.push_back(_entity);
		}

		void remove(const Entity& _entity)
		{
			auto it = m_sparse.find(_entity);
			if (it == m_sparse.end())
			{
				TRA_WARNING_LOG(("Ecs: Attempted to remove non-existent component for entity with Id: " + TRA_ENTITY_TO_STRING(_entity)).c_str());
				return;
			}
			else
			{
				size_t denseIndex = it->second;
				size_t lastDenseIndex = m_dense.size() - 1;

				if (denseIndex != lastDenseIndex)
				{
					m_dense[denseIndex] = m_dense[lastDenseIndex];
					m_denseToEntity[denseIndex] = m_denseToEntity[lastDenseIndex];
					m_sparse[m_denseToEntity[denseIndex]] = denseIndex;
				}

				m_dense.pop_back();
				m_denseToEntity.pop_back();
				m_sparse.erase(it);
			}
		}

		Component* get(const Entity& _entity)
		{
			auto it = m_sparse.find(_entity);
			if (it == m_sparse.end())
			{
				return nullptr;
			}

			return &m_dense[it->second];
		}

		bool hasComponent(const Entity& _entity)
		{
			return m_sparse.find(_entity) != m_sparse.end();
		}

	private:
		std::vector<Component> m_dense;
		std::vector<Entity> m_denseToEntity;
		std::unordered_map<Entity, size_t> m_sparse;
	};
}

#endif