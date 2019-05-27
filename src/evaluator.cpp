#include "evaluator.h"
#include "arguments.h"
#include "dictionary.h"

namespace string_format {
namespace evaluator {
	Evaluator::Evaluator(const Arguments& args,
		const std::shared_ptr<Dictionary>& dict)
		: m_args(args)
		, m_dict(dict)
	{
		// initial state
		Trans(&Evaluator::OnPlainText);
	}

	std::wstring Evaluator::Evaluate(std::wstring formater)
	{
		for (wchar_t token : formater)
		{
			Dispatch(token);
		}

		return m_ctx.result + m_ctx.rootBracket;
	}

	void Evaluator::OnPlainText(wchar_t token)
	{
		switch (token)
		{
		case L'{':
			m_ctx.rootBracket = { token };
			Trans(&Evaluator::OnInRootBracket);
			break;
		default:
			m_ctx.result.push_back(token);
			break;
		}
	}

	void Evaluator::OnInRootBracket(wchar_t token)
	{
		switch (token)
		{
		case L'0': // fall through
		case L'1': // fall through
		case L'2': // fall through
		case L'3': // fall through
		case L'4': // fall through
		case L'5': // fall through
		case L'6': // fall through
		case L'7': // fall through
		case L'8': // fall through
		case L'9':
			{
				int index = token - L'0';
				if (index < m_args.GetSize())
				{
					m_ctx.captured = m_args.GetString(index);
				}
				else
				{
					m_ctx.captured += token;
				}

				m_ctx.rootBracket += token;
				Trans(&Evaluator::OnArgumentCaptured);
			}
			break;
		default:
			m_ctx.result += m_ctx.rootBracket + token;
			m_ctx.rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		}
	}

	void Evaluator::OnArgumentCaptured(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			m_ctx.result += m_ctx.captured + m_ctx.affix;
			m_ctx.affix = {};
			m_ctx.captured = {};
			m_ctx.rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		case L',':
			m_ctx.rootBracket += token;
			Trans(&Evaluator::OnProcessGrammar);
			break;
		case L':':
			m_ctx.rootBracket += token;
			Trans(&Evaluator::OnDecorate);
			break;
		default:
			m_ctx.result += m_ctx.rootBracket + token;
			m_ctx.rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		}
	}

	void Evaluator::OnProcessGrammar(wchar_t token)
	{
		if (ProcessKorean(token)) return;
		if (ProcessEnglish(token)) return;

		m_ctx.result += m_ctx.rootBracket + token;
		m_ctx.rootBracket = {};
		Trans(&Evaluator::OnPlainText);
	}

	bool Evaluator::ProcessKorean(wchar_t token)
	{
		switch (token)
		{
		case L'을':
		case L'를':
			m_ctx.affix = EndsInConsonant(m_ctx.captured) ? L'을' : L'를';
			break;
		case L'이':
		case L'가':
			m_ctx.affix = EndsInConsonant(m_ctx.captured) ? L'이' : L'가';
			break;
		case L'은':
		case L'는':
			m_ctx.affix = EndsInConsonant(m_ctx.captured) ? L'은' : L'는';
			break;		
		case L'과':
		case L'와':
			m_ctx.affix = EndsInConsonant(m_ctx.captured) ? L'과' : L'와';
			break;
		default:
			return false;
		}

		m_ctx.rootBracket += token;
		Trans(&Evaluator::OnGrammarProcessed);
		return true;
	}

	bool Evaluator::EndsInConsonant(const std::wstring& word) const
	{
		if (word.empty()) return false;

		auto lastLetter = *word.rbegin();

		// Unicode Korean page [가-힣]
		if (44032 <= lastLetter && lastLetter <= 55203)
		{
			// Extract the final consonant
			return 0 != (lastLetter - 44032) % 28;
		}
		else
		{
			// Read Sino-Korean numbers
			std::wstring letter(1, lastLetter);
			switch (std::stoi(letter) % 10)
			{
			case 0:
			case 1:
			case 3:
			case 6:
			case 7:
			case 8:
				return true;
			case 2:
			case 4:
			case 5:
			case 9:
				return false;
			}
		}

		return true;
	}

	bool Evaluator::ProcessEnglish(wchar_t token)
	{
		switch (token)
		{
		case L's':
		case L'S':
			if (m_dict != nullptr) m_ctx.captured = m_dict->Plural(m_ctx.captured);
			break;
		default:
			return false;
		}

		m_ctx.rootBracket += token;
		Trans(&Evaluator::OnGrammarProcessed);
		return true;
	}

	void Evaluator::OnGrammarProcessed(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			m_ctx.result += m_ctx.captured + m_ctx.affix;
			m_ctx.affix = {};
			m_ctx.captured = {};
			m_ctx.rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		case L':':
			m_ctx.rootBracket += token;
			Trans(&Evaluator::OnDecorate);
			break;
		default:
			m_ctx.result += m_ctx.rootBracket + token;
			m_ctx.rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		}
	}

	void Evaluator::OnDecorate(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			m_ctx.result += m_ctx.decorated + m_ctx.affix;
			m_ctx.affix = {};
			m_ctx.captured = {};
			m_ctx.rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		case L'{':
			m_ctx.subBracket = token;
			m_ctx.rootBracket += token;
			Trans(&Evaluator::OnInSubBracket);
			break;
		case L';':
			if (m_ctx.enumIndex == std::stoi(m_ctx.captured))
			{
				m_ctx.restEnumerationDepth = 1;
				Trans(&Evaluator::OnIgnoreRestEnumeration);
			}
			else
			{
				m_ctx.enumIndex++;
				m_ctx.decorated = {};
			}

			m_ctx.rootBracket.push_back(token);
			break;
		case L'$':
			m_ctx.decorated += m_ctx.captured;
			m_ctx.rootBracket += token;
			break;
		default:
			m_ctx.decorated += token;
			m_ctx.rootBracket += token;
			break;
		}
	}

	void Evaluator::OnInSubBracket(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			{
				m_ctx.subBracket.push_back(token);

				Evaluator evaluator(m_args, m_dict);
				m_ctx.decorated += evaluator.Evaluate(m_ctx.subBracket);

				m_ctx.rootBracket.push_back(token);
				Trans(&Evaluator::OnDecorate);
			}
			break;
		default:
			m_ctx.subBracket.push_back(token);
			m_ctx.rootBracket.push_back(token);
			break;
		}
	}

	void Evaluator::OnIgnoreRestEnumeration(wchar_t token)
	{
		switch (token)
		{
		case L'{':
			m_ctx.restEnumerationDepth++;
			m_ctx.rootBracket += token;
			break;
		case L'}':
			m_ctx.restEnumerationDepth--;
			m_ctx.rootBracket += token;

			if (m_ctx.restEnumerationDepth == 0)
			{
				m_ctx.result += m_ctx.decorated + m_ctx.affix;
				m_ctx.affix = {};
				m_ctx.captured = {};
				m_ctx.rootBracket = {};
				Trans(&Evaluator::OnPlainText);
			}
			break;
		default:
			m_ctx.rootBracket += token;
			break;
		}
	}
}
}