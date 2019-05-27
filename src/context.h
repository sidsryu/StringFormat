#pragma once

#include <string>

namespace string_format {
namespace evaluator {
	struct Context
	{
		std::wstring result;
		std::wstring rootBracket;
		std::wstring captured;
		std::wstring affix;
		std::wstring subBracket;
		std::wstring decorated;
		int	enumIndex;
		int restEnumerationDepth;

		Context();
		~Context() = default;
	};
}
}
