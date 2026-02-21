#ifndef TRA_NETCODE_ENGINE_FIELD_SERIALIZER_HPP
#define TRA_NETCODE_ENGINE_FIELD_SERIALIZER_HPP

#include "TRA/export.hpp"

#include <vector>
#include <string>

namespace tra::netcode::engine::field_serializer
{
    TRA_API void serializeField(std::vector<uint8_t>& _data, const std::string& _field);
    TRA_API void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, std::string& _field);

    template<typename T>
    void serializeField(std::vector<uint8_t>& _data, const T& _field)
    {
        _data.insert(_data.end(), 
            reinterpret_cast<const uint8_t*>(&_field),
            reinterpret_cast<const uint8_t*>(&_field) + sizeof(T));
    }

    template<typename T>
    void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, T& _field)
    {
        std::memcpy(&_field, _data.data() + _offset, sizeof(T));
        _offset += sizeof(T);
    }

    template<typename T>
    void serializeField(std::vector<uint8_t>& _data, const std::vector<T>& _field)
    {
        size_t size = _field.size();
        serializeField(_data, size);

        for (const auto& element : _field)
        {
            serializeField(_data, element);
        }
    }

    template<typename T>
    void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, std::vector<T>& _field)
    {
        size_t size = 0;
        deserializeField(_data, _offset, size);

        _field.resize(size);
        for (auto& element : _field)
        {
            deserializeField(_data, _offset, element);
        }
    }
}

#endif
