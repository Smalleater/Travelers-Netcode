#ifndef TRA_NETCODE_ENGINE_INTERNAL_TAGS_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_TAGS_HPP

#include "TRA/ecs/tag.hpp"

namespace tra::netcode::engine::internal::tags
{
	TRA_ECS_REGISTER_TAG(DestroyTag)
	TRA_ECS_REGISTER_TAG(PendingDisconnectTag)
}

#endif
