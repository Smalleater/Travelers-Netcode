#ifndef TRA_NETCODE_ENGINE_FIELD_SERIALIZER_HPP
#define TRA_NETCODE_ENGINE_FIELD_SERIALIZER_HPP

#include "TRA/export.hpp"

#include <vector>
#include <string>

namespace tra::netcode::engine::fieldSerializer
{
    TRA_API void serializeField(std::vector<uint8_t>& _data, int _value);
    TRA_API void serializeField(std::vector<uint8_t>& _data, float _value);
    TRA_API void serializeField(std::vector<uint8_t>& _data, const std::string& _value);

    TRA_API void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, int& _value);
    TRA_API void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, float& _value);
    TRA_API void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, std::string& _value);
}

#endif
