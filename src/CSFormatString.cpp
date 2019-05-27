#include "CSFormatString.h"
#include "FSContext.h"

namespace CSFormatString {
	namespace {
		CStringResMgr* g_dictionary = nullptr;
	}

	CStringResMgr* SetDictionary(CStringResMgr* dictionary)
	{
		std::swap(g_dictionary, dictionary);
		return dictionary;
	}

	namespace detail {
		std::wstring Format(const std::wstring& formater, const FSParam& params)
		{
			FSContext context(params.GetParams(), g_dictionary);
			for (wchar_t token : formater)
			{
				context.Dispatch(token);
			}

			return context.GetResult();
		}
	}
}
