#pragma once

#include "arguments.h"
#include <string>
#include <memory>

namespace string_format {
	class Dictionary;

	template<typename... Args>
	static std::wstring Format(const std::wstring& formater, Args... args)
	{
		return detail::Format(formater, Arguments{ args... });
	}

	std::shared_ptr<Dictionary> SetDictionary(std::shared_ptr<Dictionary> dictionary);

	namespace detail {
		std::wstring Format(const std::wstring& formater, const Arguments& args);
	}
}
