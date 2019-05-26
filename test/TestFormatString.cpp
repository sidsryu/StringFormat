#include "../extern/catch.hpp"
#include "../src/CSFormatString.h"
#include "../src/CStringResMgr.h"


TEST_CASE("FormatString")
{
	struct MockDictionary: public CStringResMgr
	{
		virtual std::wstring Plural(const std::wstring& singular)
		{
			return singular;
		}
	};

	struct TestFormatString
	{
		MockDictionary m_dic;

		TestFormatString()
		{
			CSFormatString::SetDictionary(&m_dic);
		}

		~TestFormatString()
		{
			CSFormatString::SetDictionary(NULL);
		}
	} testFormatString;


	// 파라메터 없음
	SECTION("NoParam")
	{
		std::wstring strForamt = L"TestNoParam {0}, {1}, {2}";
		std::wstring strResult = CSFormatString::Format(strForamt);
		CHECK(L"TestNoParam 0, 1, 2" == strResult);
	}

	// 빈문자열 파라메터
	SECTION("EmptyStringParam")
	{
		std::wstring strForamt = L"EmptyParam_{0}{1}{2}{3}";
		std::wstring strResult = CSFormatString::Format(strForamt, FSParam(L"Test", L"", L"Param", L""));
		CHECK(L"EmptyParam_TestParam" == strResult);
	}

	// 인덱스 순서
	SECTION("Index")
	{		
		std::wstring strFormat = L"{2}, {0}, {1}";
		std::wstring strObject[] = {L"오렌지", L"바나나", L"사과"};

		std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0], strObject[1], strObject[2]));
		CHECK(L"사과, 오렌지, 바나나" == strResult);
	}

	// 격조사, 보조사
	SECTION("ParticleCasting")
	{
		std::wstring strFormat = L"{0,는} {1,가} {2,를}";
		std::wstring strObject[] = {L"디펜더", L"클레릭", L"분쇄"};

		std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0], strObject[1], strObject[2]));
		CHECK(L"디펜더는 클레릭이 분쇄를" == strResult);
	}


	// 격조사/보조사 외 접미사를 {}내용 맨뒤에 붙이도록 했다 - 단어에 꾸밈을 붙일 수 있다.
	SECTION("PostfixParticleCasting")
	{
		std::wstring strFormat = L"당신은 {0,을:<<$>>} 마십니다.";
		std::wstring strObject[] = {L"물약"};

		std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0]));
		CHECK(L"당신은 <<물약>>을 마십니다." == strResult);
	}

	// 영어단어 복수형
	SECTION("PluralCasting")
	{
		//using testing::_;
		//using testing::Return;
		//EXPECT_CALL(m_dic, Plural(_))
		//	.WillRepeatedly(Return(L"wolves"));


		//std::wstring strFormat = L"{0,s} {0}";
		//std::wstring strObject = L"wolf";

		//std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject));
		//CHECK(L"wolves wolf" == strResult);
	}

	// 자기참조 출력양식
	SECTION("SelfReferenceFormat")
	{
		std::wstring strFormat = L"{0:당신은 $ 입니다.}";
		std::wstring strObject = L"디펜더";

		std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject));
		CHECK(L"당신은 디펜더 입니다." == strResult);
	}

	// 재귀 출력양식
	SECTION("RecursiveFormat")
	{
		std::wstring strFormat = L"{0:당신은 {1,을} 공격합니다.}";
		std::wstring strObject[] = {L"클레릭", L"디펜더"};

		std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0], strObject[1]));
		CHECK(L"당신은 디펜더를 공격합니다." == strResult);
	}

	// 그룹지정 출력양식
	SECTION("GroupFormat")
	{
		std::wstring strFormat = L"{0:없음;하나;많음}";

		CHECK(L"없음" == CSFormatString::Format(strFormat, FSParam(0)));
		CHECK(L"하나" == CSFormatString::Format(strFormat, FSParam(1)));
		CHECK(L"많음" == CSFormatString::Format(strFormat, FSParam(2)));
	}
}
