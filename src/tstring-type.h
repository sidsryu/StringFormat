#pragma once

#include <string>
#include <sstream>
#include <tchar.h>

#ifdef _UNICODE
namespace std {
typedef wstring tstring;
typedef wstringstream tstringstream;
}
#else // _MBCS
namespace std {
typedef string tstring;
typedef stringstream tstringstream;
}
#endif
