#include "internal/networkSystemRegistrar.hpp"

#include "TRA/ecs/world.hpp"

#include "internal/acceptConnectionSystem.hpp"
#include "internal/messageSystem.hpp"
#include "internal/pendingDisconnectSystem.hpp"
#include "internal/disconnectSystem.hpp"

namespace tra::netcode::engine::internal
{
	void NetworkSystemRegistrar::registerNetworkSystems(ecs::World* _world)
	{
		// BeginUpdate
		_world->addBeginSystem(std::make_unique<DisconnectSystem>());
		_world->addBeginSystem(std::make_unique<PendingDisconnectSystem>());
		_world->addBeginSystem(std::make_unique<AcceptConnectionSystem>());
		_world->addBeginSystem(std::make_unique<ReceiveTcpMessageSystem>());

		// EndUpdate
		_world->addEndSystem(std::make_unique<SendTcpMessageSystem>());
	}
}