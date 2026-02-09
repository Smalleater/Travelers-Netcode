#include "TRA/netcode/core/netUtils.hpp"

#include "TRA/debugUtils.hpp"

#include "TRA/netcode/core/networkInclude.hpp"

namespace tra::netcode::core
{
	namespace NetUtils
	{
		bool isValidIpV4Address(const std::string& _address)
		{
			TRA_ASSERT_REF_PTR_OR_COPIABLE(_address);

			sockaddr_in sa;
			int result = inet_pton(AF_INET, _address.c_str(), &(sa.sin_addr));
			return result == 1;
		}

		bool isValidPort(uint16_t _port)
		{
			return _port <= 65535;
		}
	}
}