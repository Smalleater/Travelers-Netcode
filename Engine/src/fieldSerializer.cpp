#include "TRA/netcode/engine/fieldSerializer.hpp"

namespace tra::netcode::engine::fieldSerializer
{
    void serializeField(std::vector<uint8_t>& _data, int _value)
    {
        _data.insert(_data.end(), reinterpret_cast<const uint8_t*>(&_value),
            reinterpret_cast<const uint8_t*>(&_value) + sizeof(_value));
    }

    void serializeField(std::vector<uint8_t>& _data, float _value)
    {
        _data.insert(_data.end(), reinterpret_cast<const uint8_t*>(&_value),
            reinterpret_cast<const uint8_t*>(&_value) + sizeof(_value));
    }

    void serializeField(std::vector<uint8_t>& _data, const std::string& _value)
    {
        uint32_t size = static_cast<uint32_t>(_value.size());
        _data.insert(_data.end(), reinterpret_cast<const uint8_t*>(&size),
            reinterpret_cast<const uint8_t*>(&size) + sizeof(size));
        _data.insert(_data.end(), _value.begin(), _value.end());
    }

    void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, int& _value)
    {
        _value = *reinterpret_cast<const int*>(&_data[_offset]);
        _offset += sizeof(int);
    }

    void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, float& _value)
    {
        _value = *reinterpret_cast<const float*>(&_data[_offset]);
        _offset += sizeof(float);
    }

    void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, std::string& _value)
    {
        uint32_t size = *reinterpret_cast<const uint32_t*>(&_data[_offset]);
        _offset += sizeof(uint32_t);
        _value.assign(reinterpret_cast<const char*>(&_data[_offset]), size);
        _offset += size;
    }
}
