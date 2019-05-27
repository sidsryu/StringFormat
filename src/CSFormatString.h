#pragma once

#include "CSFormatStringParameter.h"
#include <string>

class CStringResMgr;

class CSFormatString
{
public:
	// formater는 %s나 %d 대신 {0}, {1} 등 인자 순서로 입력합니다.
	static std::wstring Format(std::wstring formater, FSParam param = FSParam());
	
	static void SetDictionary(CStringResMgr* pDictionary);

private:
	static CStringResMgr* sm_pDictionary;
};
