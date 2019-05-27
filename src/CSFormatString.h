#pragma once

#include "CSFormatStringParameter.h"
#include <string>
#include <memory>

class CStringResMgr;

namespace CSFormatString {
	template<typename... Args>
	static std::wstring Format(const std::wstring& formater, Args... args)
	{
		return detail::Format(formater, FSParam{ args... });
	}

	CStringResMgr* SetDictionary(CStringResMgr* dictionary);

	namespace detail {
		std::wstring Format(const std::wstring& formater, const FSParam& params);
	}
}
