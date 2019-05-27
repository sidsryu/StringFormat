#include "FSContext.h"
#include "arguments.h"
#include "dictionary.h"

namespace string_format {
	Evaluator::Evaluator(const Arguments& args,
		const std::shared_ptr<Dictionary>& dict)
		: m_args(args)
		, m_dict(dict)
		, m_nFormatGroup(0)
		, m_nEscapeBracketDepth(0)
	{
		// initial state
		Trans(&Evaluator::NormalTextState);
	}

	std::wstring Evaluator::Evaluate(std::wstring formater)
	{
		for (wchar_t token : formater)
		{
			Dispatch(token);
		}

		return m_strResult + m_strBracket;
	}

	void Evaluator::NormalTextState(wchar_t token)
	{
		if (token == L'{')
		{
			m_strBracket.clear();
			m_strBracket.push_back(token);
			Trans(&Evaluator::OpeningBracketState);
		}
		else
		{
			m_strResult.push_back(token);
		}
	}

	void Evaluator::OpeningBracketState(wchar_t token)
	{
		size_t nIndex = token - L'0';

		if (0 <= nIndex && nIndex < 10)
		{
			if (nIndex < m_args.GetSize())
			{
				m_strCasting = m_args.GetString(nIndex);
			}
			else
			{
				m_strCasting.push_back(token);
			}

			m_strBracket.push_back(token);
			Trans(&Evaluator::IndexState);
		}
		else
		{
			m_strBracket.push_back(token);
			m_strResult += m_strBracket;
			m_strBracket.clear();
			Trans(&Evaluator::NormalTextState);
		}
	}

	void Evaluator::IndexState(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			m_strResult += m_strCasting + m_strPostfix;
			m_strCasting.clear();
			m_strPostfix.clear();
			m_strBracket.clear();
			Trans(&Evaluator::NormalTextState);
			break;
		case L',':
			m_strBracket.push_back(token);
			Trans(&Evaluator::CastingState);
			break;
		case L':':
			m_strBracket.push_back(token);
			m_strFormat.clear();
			m_nFormatGroup = 0;
			Trans(&Evaluator::FormatState);
			break;
		default:
			m_strBracket.push_back(token);
			m_strResult += m_strBracket;
			m_strBracket.clear();
			Trans(&Evaluator::NormalTextState);
			break;
		}
	}

	void Evaluator::CastingState(wchar_t token)
	{
		switch (token)
		{
		case L'을':
		case L'를':
			if (HasJonsung())	m_strPostfix.push_back(L'을');
			else m_strPostfix.push_back(L'를');

			m_strBracket.push_back(token);
			Trans(&Evaluator::CastEndState);
			break;
		case L'이':
		case L'가':
			if (HasJonsung())	m_strPostfix.push_back(L'이');
			else m_strPostfix.push_back(L'가');

			m_strBracket.push_back(token);
			Trans(&Evaluator::CastEndState);
			break;
		case L'은':
		case L'는':
			if (HasJonsung())	m_strPostfix.push_back(L'은');
			else m_strPostfix.push_back(L'는');

			m_strBracket.push_back(token);
			Trans(&Evaluator::CastEndState);
			break;
		case L's':
		case L'S':
			if (m_dict != nullptr)
			{
				m_strCasting = m_dict->Plural(m_strCasting);
			}

			m_strBracket.push_back(token);
			Trans(&Evaluator::CastEndState);
			break;
		default:
			m_strBracket.push_back(token);
			m_strResult += m_strBracket;
			m_strBracket.clear();
			Trans(&Evaluator::NormalTextState);
			break;
		}
	}

	void Evaluator::CastEndState(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			m_strResult += m_strCasting + m_strPostfix;
			m_strCasting.clear();
			m_strPostfix.clear();
			m_strBracket.clear();
			Trans(&Evaluator::NormalTextState);
			break;
		case L':':
			m_strBracket.push_back(token);
			m_strFormat.clear();
			m_nFormatGroup = 0;
			Trans(&Evaluator::FormatState);
			break;
		default:
			m_strBracket.push_back(token);
			m_strResult += m_strBracket;
			m_strBracket.clear();
			Trans(&Evaluator::NormalTextState);
			break;
		}
	}

	void Evaluator::FormatState(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			m_strResult += m_strFormat + m_strPostfix;
			m_strPostfix.clear();
			m_strCasting.clear();
			m_strBracket.clear();
			Trans(&Evaluator::NormalTextState);
			break;
		case L'{':
			m_strBracket.push_back(token);
			m_strNestedBracket.clear();
			m_strNestedBracket.push_back(token);
			Trans(&Evaluator::NestedBracketState);
			break;
		case L'$':
			m_strBracket.push_back(token);
			m_strFormat += m_strCasting;
			break;
		case L';':
			if (m_nFormatGroup == _wtoi(m_strCasting.c_str()))
			{
				m_nEscapeBracketDepth = 1;
				Trans(&Evaluator::EscapeBracketState);
			}
			else
			{
				m_strFormat.clear();
				m_nFormatGroup++;
			}

			m_strBracket.push_back(token);
			break;
		default:
			m_strFormat.push_back(token);
			m_strBracket.push_back(token);
			break;
		}
	}

	void Evaluator::NestedBracketState(wchar_t token)
	{
		switch (token)
		{
		case L'}':
			{
				m_strNestedBracket.push_back(token);

				Evaluator evaluator(m_args, m_dict);
				m_strFormat += evaluator.Evaluate(m_strNestedBracket);

				m_strBracket.push_back(token);
				Trans(&Evaluator::FormatState);
			}
			break;
		default:
			m_strNestedBracket.push_back(token);
			m_strBracket.push_back(token);
			break;
		}
	}

	void Evaluator::EscapeBracketState(wchar_t token)
	{
		switch (token)
		{
		case L'{':
			m_nEscapeBracketDepth++;
			m_strBracket.push_back(token);
			break;
		case L'}':
			m_nEscapeBracketDepth--;
			m_strBracket.push_back(token);

			if (m_nEscapeBracketDepth == 0)
			{
				m_strResult += m_strFormat + m_strPostfix;
				m_strCasting.clear();
				m_strPostfix.clear();
				m_strBracket.clear();
				Trans(&Evaluator::NormalTextState);
			}
			break;
		default:
			m_strBracket.push_back(token);
			break;
		}
	}

	bool Evaluator::HasJonsung(void) const
	{
		if (m_strCasting.empty()) return false;


		wchar_t lastLetter = *m_strCasting.rbegin();

		// 유니코드 한글페이지
		if (44032 <= lastLetter && lastLetter <= 55203)
		{
			return 0 != (lastLetter - 44032) % 28;
		}
		else
		{
			std::wstring letter(1, lastLetter);
			switch (_wtoi(letter.c_str()) % 10)
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
}
