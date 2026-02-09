#include "internal/networkSystemRegistrar.hpp"

#include "TRA/netcode/engine/networkEcs.hpp"

#include "internal/acceptConnectionSystem.hpp"
#include "internal/messageSystem.hpp"
#include "internal/pendingDisconnectSystem.hpp"
#include "internal/disconnectSystem.hpp"

namespace tra::netcode::engine
{
	void NetworkSystemRegistrar::registerNetworkSystems(NetworkEcs* _networkEcs)
	{
		// BeginUpdate
		_networkEcs->registerBeginUpdateSystem(std::make_unique<DisconnectSystem>());
		_networkEcs->registerBeginUpdateSystem(std::make_unique<PendingDisconnectSystem>());
		_networkEcs->registerBeginUpdateSystem(std::make_unique<AcceptConnectionSystem>());
		_networkEcs->registerBeginUpdateSystem(std::make_unique<ReceiveTcpMessageSystem>());

		// EndUpdate
		_networkEcs->registerEndUpdateSystem(std::make_shared<SendTcpMessageSystem>());
	}
}