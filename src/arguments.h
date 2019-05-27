#pragma once

#include <vector>
#include <sstream>

namespace string_format {
	class Arguments
	{
	public:
		template<typename First, typename... Rest>
		Arguments(First first, Rest... rest) : Arguments(rest...)
		{
			std::wstringstream ss;
			ss << first;
			m_listofArgument.insert(m_listofArgument.begin(), ss.str());
		}

		Arguments() = default;

		size_t GetSize() const;
		std::wstring GetString(size_t idx) const;

	private:
		std::vector<std::wstring> m_listofArgument;
	};
}
