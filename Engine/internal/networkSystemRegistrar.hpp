#ifndef TRA_NETCODE_ENGINE_NETWORK_SYSTEM_REGISTRAR_HPP
#define TRA_NETCODE_ENGINE_NETWORK_SYSTEM_REGISTRAR_HPP

namespace tra::netcode::engine
{
	class NetworkEcs;
	namespace NetworkSystemRegistrar
	{
		void registerNetworkSystems(NetworkEcs* _networkEcs);
	}
}

#endif
