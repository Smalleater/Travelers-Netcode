#ifndef TRA_NETCODE_ENGINE_MESSAGE_SYSTEM_HPP
#define TRA_NETCODE_ENGINE_MESSAGE_SYSTEM_HPP

#include "internal/iNetworkSystem.hpp"

namespace tra::netcode::engine
{
	struct SendTcpMessageSystem : public INetworkSystem
	{
		void update(NetworkEcs* _ecs) override;
	};

	struct ReceiveTcpMessageSystem : public INetworkSystem
	{
		void update(NetworkEcs* _ecs) override;
	};
}

#endif
