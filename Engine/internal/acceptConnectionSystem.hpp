#ifndef TRA_NETCODE_ENGINE_INTERNAL_ACCEPT_CONNECTION_SYSTEM_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_ACCEPT_CONNECTION_SYSTEN_HPP

#include "TRA/ecs/ISystem.hpp"

namespace tra::netcode::engine::internal
{
	struct AcceptConnectionSystem : ecs::ISystem
	{
		void update(ecs::World* _world) override;
	};
}

#endif
