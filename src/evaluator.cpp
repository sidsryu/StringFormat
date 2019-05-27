#include "evaluator.h"
#include "arguments.h"
#include "dictionary.h"

namespace string_format {
	Evaluator::Evaluator(const Arguments& args,
		const std::shared_ptr<Dictionary>& dict)
		: m_args(args)
		, m_dict(dict)
		, m_enumIndex(0)
		, m_restEnumerationDepth(0)
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

		return m_result + m_rootBracket;
	}

	void Evaluator::OnPlainText(wchar_t token)
	{
		switch (token)
		{
		case L'{':
			m_rootBracket = { token };
			Trans(&Evaluator::OnInRootBracket);
			break;
		default:
			m_result.push_back(token);
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
					m_captured = m_args.GetString(index);
				}
				else
				{
					m_captured += token;
				}

				m_rootBracket += token;
				Trans(&Evaluator::OnArgumentCaptured);
			}
			break;
		default:
			m_result += m_rootBracket + token;
			m_rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		}
	}

	void Evaluator::OnArgumentCaptured(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			m_result += m_captured + m_affix;
			m_affix = {};
			m_captured = {};
			m_rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		case L',':
			m_rootBracket += token;
			Trans(&Evaluator::OnProcessGrammar);
			break;
		case L':':
			m_rootBracket += token;
			Trans(&Evaluator::OnDecorate);
			break;
		default:
			m_result += m_rootBracket + token;
			m_rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		}
	}

	void Evaluator::OnProcessGrammar(wchar_t token)
	{
		if (ProcessKorean(token)) return;
		if (ProcessEnglish(token)) return;

		m_result += m_rootBracket + token;
		m_rootBracket = {};
		Trans(&Evaluator::OnPlainText);
	}

	bool Evaluator::ProcessKorean(wchar_t token)
	{
		switch (token)
		{
		case L'을':
		case L'를':
			m_affix = EndsInConsonant(m_captured) ? L'을' : L'를';
			break;
		case L'이':
		case L'가':
			m_affix = EndsInConsonant(m_captured) ? L'이' : L'가';
			break;
		case L'은':
		case L'는':
			m_affix = EndsInConsonant(m_captured) ? L'은' : L'는';
			break;
		default:
			return false;
		}

		m_rootBracket += token;
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
			if (m_dict != nullptr) m_captured = m_dict->Plural(m_captured);
			break;
		default:
			return false;
		}

		m_rootBracket += token;
		Trans(&Evaluator::OnGrammarProcessed);
		return true;
	}

	void Evaluator::OnGrammarProcessed(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			m_result += m_captured + m_affix;
			m_affix = {};
			m_captured = {};
			m_rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		case L':':
			m_rootBracket += token;
			Trans(&Evaluator::OnDecorate);
			break;
		default:
			m_result += m_rootBracket + token;
			m_rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		}
	}

	void Evaluator::OnDecorate(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			m_result += m_decorated + m_affix;
			m_affix = {};
			m_captured = {};
			m_rootBracket = {};
			Trans(&Evaluator::OnPlainText);
			break;
		case L'{':
			m_subBracket = token;
			m_rootBracket += token;
			Trans(&Evaluator::OnInSubBracket);
			break;
		case L';':
			if (m_enumIndex == std::stoi(m_captured))
			{
				m_restEnumerationDepth = 1;
				Trans(&Evaluator::OnIgnoreRestEnumeration);
			}
			else
			{
				m_enumIndex++;
				m_decorated = {};
			}

			m_rootBracket.push_back(token);
			break;
		case L'$':
			m_decorated += m_captured;
			m_rootBracket += token;
			break;
		default:
			m_decorated += token;
			m_rootBracket += token;
			break;
		}
	}

	void Evaluator::OnInSubBracket(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			{
				m_subBracket.push_back(token);

				Evaluator evaluator(m_args, m_dict);
				m_decorated += evaluator.Evaluate(m_subBracket);

				m_rootBracket.push_back(token);
				Trans(&Evaluator::OnDecorate);
			}
			break;
		default:
			m_subBracket.push_back(token);
			m_rootBracket.push_back(token);
			break;
		}
	}

	void Evaluator::OnIgnoreRestEnumeration(wchar_t token)
	{
		switch (token)
		{
		case L'{':
			m_restEnumerationDepth++;
			m_rootBracket += token;
			break;
		case L'}':
			m_restEnumerationDepth--;
			m_rootBracket += token;

			if (m_restEnumerationDepth == 0)
			{
				m_result += m_decorated + m_affix;
				m_affix = {};
				m_captured = {};
				m_rootBracket = {};
				Trans(&Evaluator::OnPlainText);
			}
			break;
		default:
			m_rootBracket += token;
			break;
		}
	}
}
