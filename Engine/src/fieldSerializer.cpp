#include "TRA/netcode/engine/fieldSerializer.hpp"

namespace tra::netcode::engine::field_serializer
{
    void serializeField(std::vector<uint8_t>& _data, const std::string& _field)
    {
        size_t size = _field.size();
        serializeField(_data, size);

        _data.insert(_data.end(), _field.begin(), _field.end());
    }

    void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, std::string& _field)
    {
        uint32_t size = 0;
        deserializeField(_data, _offset, size);

        _field.assign(reinterpret_cast<const char*>(&_data[_offset]), size);
        _offset += size;
    }
}
