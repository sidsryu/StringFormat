#include "CSFormatString.h"
#include "FSContext.h"
#include <vector>

namespace {
std::vector<std::wstring> split(const std::wstring& s, wchar_t delimiter)
{
	std::vector<std::wstring> result;
	if (s.empty()) return result;

	size_t pos = 0;
	while (1)
	{
		auto it = s.find(delimiter, pos);
		if (it == std::string::npos)
		{
			result.push_back(s.substr(pos));
			return result;
		}
		
		result.push_back(s.substr(pos, it - pos));
		pos = it + 1;
	}
}
}

CStringResMgr* CSFormatString::sm_pDictionary = NULL;

std::wstring CSFormatString::Format(std::wstring formater, FSParam param)
{
	// 외부 인자 추출	
	std::vector<std::wstring> vecParam = split(param.m_strParam, FSP_SEPARATOR);

	// 파싱
	FSContext context(vecParam, sm_pDictionary);

	for (wchar_t token : formater)
	{
		context.Dispatch(token);
	}

	return context.GetResult();
}

void CSFormatString::SetDictionary(CStringResMgr* pDictionary)
{
	sm_pDictionary = pDictionary;
}
