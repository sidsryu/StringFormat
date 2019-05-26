#pragma once

#include <string>

class CStringResMgr
{
public:
	virtual std::wstring Plural(const std::wstring& singular);
};
