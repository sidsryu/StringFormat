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
		void OnPlainText(wchar_t token);
		void OnOpenBracket(wchar_t token);
		void OnIndexed(wchar_t token);
		void OnGrammar(wchar_t token);
		void OnGrammarFinished(wchar_t token);
		void OnDecorating(wchar_t token);
		void OnNestedBracket(wchar_t token);
		void OnCloseBracket(wchar_t token);

		bool HasJonsung(void) const; // Korean grammar helper

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
