#ifndef TRA_NETCODE_ENGINE_SOCKET_COMPONENT_HPP
#define TRA_NETCODE_ENGINE_SOCKET_COMPONENT_HPP

#include "TRA/netcode/core/tcpSocket.hpp"

#include "TRA/netcode/engine/iNetworkComponent.hpp"

namespace tra::netcode::engine
{
	struct TcpListenSocketComponent : public INetworkComponent
	{
		core::TcpSocket* m_tcpSocket;

		TcpListenSocketComponent() : m_tcpSocket(nullptr) {}

		~TcpListenSocketComponent()
		{
			if (m_tcpSocket)
			{
				m_tcpSocket->shutdownSocket();
				m_tcpSocket->closeSocket();
				delete m_tcpSocket;
			}
		}
	};

	struct TcpConnectSocketComponent : public INetworkComponent
	{
		core::TcpSocket* m_tcpSocket;

		TcpConnectSocketComponent() : m_tcpSocket(nullptr) {}

		~TcpConnectSocketComponent()
		{
			if (m_tcpSocket)
			{
				m_tcpSocket->shutdownSocket();
				m_tcpSocket->closeSocket();
				delete m_tcpSocket;
			}
		}
	};
}

#endif
