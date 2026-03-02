#ifndef TRA_NETCODE_ENGINE_INTERNAL_SNAPSHOT_SYSTEM_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_SNAPSHOT_SYSTEM_HPP

#include "TRA/ecs/ISystem.hpp"

namespace tra::netcode::engine::internal
{
	struct SendSnapshotSystem : ecs::ISystem
	{
		void update(ecs::World* _world) override;
	};

	struct ReceiveSnapshotSystem : ecs::ISystem
	{
		void update(ecs::World* _world) override;
	};
}

#endif
