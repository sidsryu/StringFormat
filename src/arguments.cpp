#include "arguments.h"

namespace string_format {
	size_t Arguments::GetSize() const
	{
		return m_listofArgument.size();
	}

	std::wstring Arguments::GetString(size_t idx) const
	{
		return m_listofArgument.at(idx);
	}
}
