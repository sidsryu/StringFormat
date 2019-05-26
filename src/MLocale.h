#pragma once

#include "tstring-type.h"

namespace MLocale {
std::wstring ConvTCHARToUTF16(const std::tstring& str)
{
	return L"";
}

std::tstring ConvUTF16ToTCHAR(const std::wstring& str)
{
	return "";
}
}
