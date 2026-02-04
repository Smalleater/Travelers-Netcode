#ifndef TRA_ECS_COMPONENT_HPP
#define TRA_ECS_COMPONENT_HPP

#include "TRA/ecs/componentRegister.hpp"

#define TRA_ECS_REGISTER_COMPONENT(NAME, ...) \
namespace tra::ecs \
{ \
	struct NAME \
	{ \
		__VA_ARGS__ \
	private: \
		static inline ComponentRegister<NAME> m_registrar{}; \
	}; \
} \

#endif
