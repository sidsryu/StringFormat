#include "CSFormatString.h"
#include "FSContext.h"

namespace CSFormatString {
	namespace {
		std::shared_ptr<Dictionary> g_dictionary;
	}

	std::shared_ptr<Dictionary> SetDictionary(std::shared_ptr<Dictionary> dictionary)
	{
		std::swap(g_dictionary, dictionary);
		return dictionary;
	}

	namespace detail {
		std::wstring Format(const std::wstring& formater, const FSParam& params)
		{
			FSContext context(params.GetParams(), g_dictionary.get());
			for (wchar_t token : formater)
			{
				context.Dispatch(token);
			}

			return context.GetResult();
		}
	}
}
