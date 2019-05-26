#pragma once

#include <cassert>

template <typename Derive>
class FSFsm
{
public:
	typedef void (Derive::*FSState)(wchar_t token);

public:
	FSFsm(): m_state(nullptr)
	{
		// do nothing
	}

	virtual ~FSFsm()	
	{
		// do nothing
	}

	void Dispatch(wchar_t token)
	{
		assert(m_state != nullptr);
		(((Derive*)this)->*m_state)(token);
	}

	void Trans(FSState state)
	{
		m_state = state;
	}

private:
	FSState m_state;
};
