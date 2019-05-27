#pragma once

#include <string>
#include <sstream>

#define FSP_SEPARATOR	L'|'

struct FSParam
{
	std::wstring m_strParam;

	template<typename... Args>
	FSParam(Args... args)
	{
		m_strParam = Serialize(args...);
	}

private:
	template<typename First, typename... Rest>
	std::wstring Serialize(First first, Rest... rest)
	{
		std::wstringstream ss;
		ss << first;

		if (0 < sizeof...(rest))
		{
			ss << FSP_SEPARATOR << Serialize(rest...);
		}

		return ss.str();
	}

	std::wstring Serialize()
	{
		return {};
	}
};
