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
	// �����ڵ�� ó���Ѵ�. TCHAR�� Ŭ���̾�Ʈ �� ���带 ���� �������̽����� ����Ѵ�.
	std::wstring strFormater = MLocale::ConvTCHARToUTF16(formater.c_str());
	std::wstring strParam = MLocale::ConvTCHARToUTF16(param.m_strParam.c_str());
	std::wstring strSeparator = MLocale::ConvTCHARToUTF16(FSP_SEPARATOR);

	
	// �ܺ� ���� ����	
	std::vector<std::wstring> vecParam = split(strParam, strSeparator.front());

	// �Ľ�
	FSContext context(vecParam, sm_pDictionary);

	for (wchar_t token : formater)
	{
		context.Dispatch(token);
	}

	std::wstring strResult = context.GetResult();


	// �����ڵ� ��� tstring���� ��ȯ
	return MLocale::ConvUTF16ToTCHAR(strResult.c_str());
}

void CSFormatString::SetDictionary(CStringResMgr* pDictionary)
{
	sm_pDictionary = pDictionary;
}
