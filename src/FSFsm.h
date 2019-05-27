#pragma once

#include <cassert>
#include <functional>

namespace string_format {
	template<typename Derived>
	class StateMethodFsm
	{
	public:
		using State = void (Derived::*)(wchar_t token);

		StateMethodFsm() : m_method(nullptr)
		{}

		virtual ~StateMethodFsm()
		{}

		void Dispatch(wchar_t token)
		{
			assert(m_method != nullptr);
			std::mem_fn(m_method)(static_cast<Derived*>(this), token);
		}

		void Trans(State method)
		{
			m_method = method;
		}

	private:
		State m_method;
	};
}
