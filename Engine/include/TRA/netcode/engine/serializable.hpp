#ifndef TRA_NETCODE_ENGINE_SERIALIZABLE_HPP
#define TRA_NETCODE_ENGINE_SERIALIZABLE_HPP

#include <functional>
#include <unordered_map>

#include "TRA/netcode/engine/fieldSerializer.hpp"

namespace tra::netcode::engine
{
	struct TRA_API Serializable
	{
	public:
		using SerializerFunc = std::function<void(const void*, std::vector<uint8_t>&)>;
		using DeserializerFunc = std::function<void(const void*, const std::vector<uint8_t>&, size_t&)>;

		using SerializerEntry = std::pair<std::string, std::pair<size_t, SerializerFunc>>;
		using DeserializerEntry = std::pair<std::string, std::pair<size_t, DeserializerFunc>>;

		using SerializerMap = std::unordered_map<uint32_t, std::vector<SerializerEntry>>;
		using DeserializerMap = std::unordered_map<uint32_t, std::vector<DeserializerEntry>>;

		virtual ~Serializable() = default;

		virtual std::vector<uint8_t> serialize() const = 0;

		static SerializerMap& getSerializers();
		static DeserializerMap& getDeserializers();

		template<typename T>
		static void registerSerializer(const uint32_t _messageId, const std::string& _fieldName, size_t _fieldOffset)
		{
			auto& serializers = getSerializers();
			serializers[_messageId].emplace_back(_fieldName, std::make_pair(_fieldOffset, [_fieldOffset](const void* base, std::vector<uint8_t>& data) {
				const T* field = reinterpret_cast<const T*>(static_cast<const char*>(base) + _fieldOffset);
				field_serializer::serializeField(data, *field);
				}));
		}

		template<typename T>
		static void registerDeserializer(const uint32_t _messageId, const std::string& _fieldName, size_t _fieldOffset)
		{
			auto& deserializers = getDeserializers();
			deserializers[_messageId].emplace_back(_fieldName, std::make_pair(_fieldOffset,
				[_fieldOffset](const void* base, const std::vector<uint8_t>& data, size_t& offset) {
					T* field = reinterpret_cast<T*>(static_cast<char*>(const_cast<void*>(base)) + _fieldOffset);
					field_serializer::deserializeField(data, offset, *field);
				}));
		}
	};
}

#endif
