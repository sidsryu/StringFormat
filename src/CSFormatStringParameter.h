#pragma once

#include <vector>
#include <sstream>

class FSParam
{
public:
	template<typename First, typename... Rest>
	FSParam(First first, Rest... rest) : FSParam(rest...)
	{
		std::wstringstream ss;
		ss << first;
		m_listofParam.insert(m_listofParam.begin(), ss.str());
	}

	FSParam() 
	{}

	const std::vector<std::wstring>& GetParams() const;

private:
	std::vector<std::wstring> m_listofParam;
};
