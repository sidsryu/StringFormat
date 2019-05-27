#include "CSFormatString.h"
#include "FSContext.h"

CStringResMgr* CSFormatString::sm_pDictionary = NULL;

std::wstring CSFormatString::Format(std::wstring formater, const FSParam& params)
{
	FSContext context(params.GetParams(), sm_pDictionary);

	for (wchar_t token : formater)
	{
		context.Dispatch(token);
	}

	return context.GetResult();
}

void CSFormatString::SetDictionary(CStringResMgr* pDictionary)
{
	sm_pDictionary = pDictionary;
}
