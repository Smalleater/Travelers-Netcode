#ifndef TRA_NETCODE_ENGINE_INTERNAL_MESSAGE_SYSTEM_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_MESSAGE_SYSTEM_HPP

#include "TRA/ecs/ISystem.hpp"

namespace tra::netcode::engine::internal
{
	struct SendTcpMessageSystem : ecs::ISystem
	{
		void update(ecs::World* _world) override;
	};

	struct ReceiveTcpMessageSystem : ecs::ISystem
	{
		void update(ecs::World* _world) override;
	};
}

#endif
