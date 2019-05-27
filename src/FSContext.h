#pragma once

#include "FSFsm.h"
#include <vector>
#include <string>

namespace string_format {
	class Dictionary;

	class FSContext : public FSFsm<FSContext>
	{
	public:
		FSContext(const std::vector<std::wstring>& param, Dictionary* pDictionary = NULL);
		virtual ~FSContext();

		std::wstring GetResult(void) const;

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
		std::wstring m_strResult;
		std::wstring m_strBracket;
		std::wstring m_strCasting;
		std::wstring m_strPostfix;
		std::wstring m_strNestedBracket;
		std::wstring m_strFormat;
		int	m_nFormatGroup;
		int m_nEscapeBracketDepth;

		std::vector<std::wstring> m_vecParam;
		Dictionary* m_pDictionary;
	};
}
