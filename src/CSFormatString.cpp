#include "CSFormatString.h"
#include "FSContext.h"
#include "MLocale.h"

#include <vector>
namespace {
std::vector<std::wstring> split(const std::wstring& s, wchar_t delimiter)
{
	std::vector<std::wstring> tokens;
	std::wstring token;
	std::wistringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}
}

CStringResMgr* CSFormatString::sm_pDictionary = NULL;

std::tstring CSFormatString::Format(std::tstring formater, FSParam param)
{
	// 유니코드로 처리한다. TCHAR는 클라이언트 툴 빌드를 위해 인터페이스에만 사용한다.
	std::wstring strFormater = MLocale::ConvTCHARToUTF16(formater.c_str());
	std::wstring strParam = MLocale::ConvTCHARToUTF16(param.m_strParam.c_str());
	std::wstring strSeparator = MLocale::ConvTCHARToUTF16(FSP_SEPARATOR);

	
	// 외부 인자 추출	
	std::vector<std::wstring> vecParam = split(strParam, strSeparator.front());

	// 파싱
	FSContext context(vecParam, sm_pDictionary);

	for (wchar_t token : formater)
	{
		context.Dispatch(token);
	}

	std::wstring strResult = context.GetResult();


	// 유니코드 결과 tstring으로 변환
	return MLocale::ConvUTF16ToTCHAR(strResult.c_str());
}

void CSFormatString::SetDictionary(CStringResMgr* pDictionary)
{
	sm_pDictionary = pDictionary;
}
