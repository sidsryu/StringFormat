#pragma once

#include "CSFormatStringParameter.h"
#include <string>

class CStringResMgr;

class CSFormatString
{
public:
	template<typename... Args>
	static std::wstring Format(std::wstring formater, Args... args)
	{
		return Format(formater, FSParam{ args... });
	}

	static std::wstring Format(std::wstring formater, const FSParam& params);
	static void SetDictionary(CStringResMgr* pDictionary);

private:
	static CStringResMgr* sm_pDictionary;
};
