#ifndef TRA_NETCODE_ENGINE_INTERNAL_SOCKET_COMPONENT_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_SOCKET_COMPONENT_HPP

#include <memory>

#include "TRA/ecs/component.hpp"

#include "TRA/netcode/core/tcpSocket.hpp"

namespace tra::netcode::engine::internal::components
{
	TRA_ECS_REGISTER_COMPONENT(TcpListenSocketComponent,
		std::unique_ptr<core::TcpSocket> m_tcpSocket;

		TcpListenSocketComponent() = default;
		TcpListenSocketComponent(TcpListenSocketComponent&&) = default;
		TcpListenSocketComponent& operator=(TcpListenSocketComponent&&) = default;

		~TcpListenSocketComponent()
		{
			if (m_tcpSocket)
			{
				m_tcpSocket->shutdownSocket();
				m_tcpSocket->closeSocket();
			}
		}
	)

	TRA_ECS_REGISTER_COMPONENT(TcpConnectSocketComponent,
		std::unique_ptr<core::TcpSocket> m_tcpSocket;

		TcpConnectSocketComponent() = default;
		TcpConnectSocketComponent(core::TcpSocket* _tcpSocket)
			: m_tcpSocket(_tcpSocket) {}

		TcpConnectSocketComponent(TcpConnectSocketComponent&&) = default;
		TcpConnectSocketComponent& operator=(TcpConnectSocketComponent&&) = default;

		~TcpConnectSocketComponent()
		{
			if (m_tcpSocket)
			{
				m_tcpSocket->shutdownSocket();
				m_tcpSocket->closeSocket();
			}
		}
	)
}

#endif
