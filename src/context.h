#pragma once

#include <string>

namespace string_format {
namespace evaluator {
	struct Context
	{
		std::wstring result; // result
		std::wstring rootBracket; // { inter-brackets text }
		std::wstring captured; // replace parameter text from argument
		std::wstring affix; // postfix for grammar
		std::wstring subBracket; // for recursive brackets
		std::wstring decorated;
		int	enumIndex; // mapping enumeration index
		int restEnumerationDepth; // ignore rest enumeration (with recursived bracket) after mapping

		Context();
	};
}
}
