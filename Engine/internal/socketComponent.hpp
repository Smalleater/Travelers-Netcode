#ifndef TRA_NETCODE_ENGINE_INTERNAL_SOCKET_COMPONENT_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_SOCKET_COMPONENT_HPP

#include "TRA/ecs/component.hpp"

#include "TRA/netcode/core/tcpSocket.hpp"

namespace tra::netcode::engine::internal::components
{
	TRA_ECS_REGISTER_COMPONENT(TcpListenSocketComponent,
		std::unique_ptr<core::TcpSocket> m_tcpSocket;

		TcpListenSocketComponent() : m_tcpSocket(nullptr) {}
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
		TcpConnectSocketComponent(core::TcpSocket&& _tcpSocket) 
			: m_tcpSocket(std::make_unique<core::TcpSocket>(std::move(_tcpSocket))) {}

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
