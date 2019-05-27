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

		Arguments()
		{}

		const std::vector<std::wstring>& GetStrings() const;

	private:
		std::vector<std::wstring> m_listofArgument;
	};
}
