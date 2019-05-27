#pragma once

#include <string>

class Dictionary
{
public:
	virtual std::wstring Plural(const std::wstring& singular) = 0;	
	virtual ~Dictionary() = default;
};
