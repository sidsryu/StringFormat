#pragma once

#include <string>

namespace string_format {
	class Dictionary
	{
	public:
		virtual std::wstring Plural(const std::wstring& singular) = 0;
		virtual ~Dictionary() = default;
	};
}
