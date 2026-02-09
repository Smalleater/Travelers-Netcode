#ifndef TRA_NETCODE_ENGINE_MESSAGE_HEADER_HPP
#define TRA_NETCODE_ENGINE_MESSAGE_HEADER_HPP

#include <variant>
#include <string>
#include <unordered_map>

namespace tra::netcode::engine
{
    struct MessageHeader
    {
        uint32_t size;
    };
}

#endif