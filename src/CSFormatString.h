#pragma once

#include "CSFormatStringParameter.h"
#include <string>
#include <memory>

class Dictionary;

namespace CSFormatString {
	template<typename... Args>
	static std::wstring Format(const std::wstring& formater, Args... args)
	{
		return detail::Format(formater, FSParam{ args... });
	}

	std::shared_ptr<Dictionary> SetDictionary(std::shared_ptr<Dictionary> dictionary);

	namespace detail {
		std::wstring Format(const std::wstring& formater, const FSParam& params);
	}
}
