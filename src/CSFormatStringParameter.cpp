#include "CSFormatStringParameter.h"

namespace string_format {
	const std::vector<std::wstring>& FSParam::GetParams() const
	{
		return m_listofParam;
	}
}
