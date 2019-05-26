#pragma once

#include "tstring-type.h"

namespace MLocale {
std::wstring ConvTCHARToUTF16(const std::tstring& str);
std::tstring ConvUTF16ToTCHAR(const std::wstring& str);
}
