#ifndef TRA_ECS_TAG_HPP
#define TRA_ECS_TAG_HPP

#include "TRA/ecs/tagRegister.hpp"

#define TRA_ECS_REGISTER_TAG(NAME) \
namespace tra::ecs \
{ \
	struct NAME \
	{ \
	private: \
		static inline TagRegister<NAME> m_registrar{}; \
	}; \
} \

#endif
