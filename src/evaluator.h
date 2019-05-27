#pragma once

#include "state-method-fsm.h"
#include "context.h"
#include <vector>
#include <string>
#include <memory>

namespace string_format {
	class Arguments;
	class Dictionary;

namespace evaluator {
	class Evaluator : public StateMethodFsm<Evaluator>
	{
	public:
		Evaluator(const Arguments& args, const std::shared_ptr<Dictionary>& dict);
		virtual ~Evaluator() = default;

		std::wstring Evaluate(std::wstring formater);

	private:
		void OnPlainText(wchar_t token);
		void OnInRootBracket(wchar_t token);
		void OnArgumentCaptured(wchar_t token);
		void OnProcessGrammar(wchar_t token);
		void OnGrammarProcessed(wchar_t token);
		void OnDecorate(wchar_t token);
		void OnInSubBracket(wchar_t token);
		void OnIgnoreRestEnumeration(wchar_t token);

		bool ProcessKorean(wchar_t token);
		bool ProcessEnglish(wchar_t token);
		bool EndsInConsonant(const std::wstring& word) const;

	private:
		const Arguments& m_args;
		const std::shared_ptr<Dictionary>& m_dict;
		Context m_ctx;
	};
}
}
