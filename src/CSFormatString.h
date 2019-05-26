#pragma once

#include "CSFormatStringParameter.h"
#include "tstring-type.h"

class CStringResMgr;

class CSFormatString
{
public:
	// formater�� %s�� %d ��� {0}, {1} �� ���� ������ �Է��մϴ�.
	static std::tstring Format(std::tstring formater, FSParam param = FSParam());
	
	static void SetDictionary(CStringResMgr* pDictionary);

private:
	static CStringResMgr* sm_pDictionary;
};
