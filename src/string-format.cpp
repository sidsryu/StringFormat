#include "string-format.h"
#include "evaluator.h"

namespace string_format {
	namespace {
		std::shared_ptr<Dictionary> g_dictionary;
	}

	std::shared_ptr<Dictionary> SetDictionary(std::shared_ptr<Dictionary> dictionary)
	{
		std::swap(g_dictionary, dictionary);
		return dictionary;
	}

	namespace detail {
		std::wstring Format(const std::wstring& formater, const Arguments& args)
		{
			Evaluator context(args, g_dictionary);
			return context.Evaluate(formater);
		}
	}
}
