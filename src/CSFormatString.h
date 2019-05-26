#pragma once

#include "CSFormatStringParameter.h"
#include "tstring-type.h"

class CStringResMgr;

class CSFormatString
{
public:
	// formater는 %s나 %d 대신 {0}, {1} 등 인자 순서로 입력합니다.
	static std::tstring Format(std::tstring formater, FSParam param = FSParam());
	
	static void SetDictionary(CStringResMgr* pDictionary);

private:
	static CStringResMgr* sm_pDictionary;
};
