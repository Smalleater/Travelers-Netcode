#ifndef TRA_NETCODE_CORE_SOCKET_UTILS_HPP
#define TRA_NETCODE_CORE_SOCKET_UTILS_HPP

#include "TRA/errorCode.hpp"

#include <utility>
#include <string>

#include "TRA/netcode/core/networkInclude.hpp"

namespace tra::netcode::core
{
    namespace SocketUtils
    {
        int getLastSocketError();
        bool isWouldBlockError(int _err);
        std::pair<ErrorCode, int> setBlocking(socket_t& _socket, bool _blocking);
        std::pair<ErrorCode, uint16_t> getSocketPort(socket_t& _socket);
    }
}

#endif