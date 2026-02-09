#ifndef TRA_NETCODE_ENGINE_INTERNAL_NETWORK_SYSTEM_REGISTRAR_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_NETWORK_SYSTEM_REGISTRAR_HPP

namespace tra::ecs
{
	class World;
}

namespace tra::netcode::engine::internal
{
	namespace NetworkSystemRegistrar
	{
		void registerNetworkSystems(tra::ecs::World* _world);
	}
}

#endif
