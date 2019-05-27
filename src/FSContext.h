#pragma once

#include "state-method-fsm.h"
#include <vector>
#include <string>
#include <memory>

namespace string_format {
	class Arguments;
	class Dictionary;

	class Evaluator : public StateMethodFsm<Evaluator>
	{
	public:
		Evaluator(const Arguments& args, const std::shared_ptr<Dictionary>& dict);
		virtual ~Evaluator() = default;

		std::wstring Evaluate(std::wstring formater);

	private:
		void NormalTextState(wchar_t token);
		void OpeningBracketState(wchar_t token);
		void IndexState(wchar_t token);
		void CastingState(wchar_t token);
		void CastEndState(wchar_t token);
		void FormatState(wchar_t token);
		void NestedBracketState(wchar_t token);
		void EscapeBracketState(wchar_t token);

	private:
		bool HasJonsung(void) const;

	private:
		const Arguments& m_args;
		const std::shared_ptr<Dictionary>& m_dict;

		std::wstring m_strResult;
		std::wstring m_strBracket;
		std::wstring m_strCasting;
		std::wstring m_strPostfix;
		std::wstring m_strNestedBracket;
		std::wstring m_strFormat;
		int	m_nFormatGroup;
		int m_nEscapeBracketDepth;
	};
}
