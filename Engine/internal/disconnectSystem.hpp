#ifndef TRA_NETCODE_ENGINE_INTERNAL_DISCONNECT_SYSTEM_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_DISCONNECT_SYSTEM_HPP

#include "TRA/ecs/ISystem.hpp"

namespace tra::netcode::engine::internal
{
	struct DisconnectSystem : ecs::ISystem
	{
		void update(ecs::World* _world) override;
	};
}

#endif
