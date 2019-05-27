#include "CSFormatStringParameter.h"

namespace string_format {
	const std::vector<std::wstring>& Arguments::GetStrings() const
	{
		return m_listofArgument;
	}
}
