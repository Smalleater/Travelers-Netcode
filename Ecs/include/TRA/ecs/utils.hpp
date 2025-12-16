#ifndef TRA_ECS_UTILS_HPP
#define TRA_ECS_UTILS_HPP

#include <string>

#include "TRA/ecs/entity.hpp"

namespace tra::ecs
{
	#define TRA_ENTITY_TO_STRING(_entity) std::to_string(_entity.m_id) + ":" + std::to_string(_entity.m_version)
}

#endif // !TRA_ECS_UTILS_HPP
