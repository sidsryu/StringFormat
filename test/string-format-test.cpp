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


	// �Ķ���� ����
	SECTION("NoParam")
	{
		std::wstring strForamt = L"TestNoParam {0}, {1}, {2}";
		std::wstring strResult = CSFormatString::Format(strForamt);
		CHECK(L"TestNoParam 0, 1, 2" == strResult);
	}

	// ���ڿ� �Ķ����
	SECTION("EmptyStringParam")
	{
		std::wstring strForamt = L"EmptyParam_{0}{1}{2}{3}";
		std::wstring strResult = CSFormatString::Format(strForamt, FSParam(L"Test", L"", L"Param", L""));
		CHECK(L"EmptyParam_TestParam" == strResult);
	}

	// �ε��� ����
	SECTION("Index")
	{		
		std::wstring strFormat = L"{2}, {0}, {1}";
		std::wstring strObject[] = {L"������", L"�ٳ���", L"���"};

		std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0], strObject[1], strObject[2]));
		CHECK(L"���, ������, �ٳ���" == strResult);
	}

	// ������, ������
	SECTION("ParticleCasting")
	{
		std::wstring strFormat = L"{0,��} {1,��} {2,��}";
		std::wstring strObject[] = {L"�����", L"Ŭ����", L"�м�"};

		std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0], strObject[1], strObject[2]));
		CHECK(L"������� Ŭ������ �м⸦" == strResult);
	}


	// ������/������ �� ���̻縦 {}���� �ǵڿ� ���̵��� �ߴ� - �ܾ �ٹ��� ���� �� �ִ�.
	SECTION("PostfixParticleCasting")
	{
		std::wstring strFormat = L"����� {0,��:<<$>>} ���ʴϴ�.";
		std::wstring strObject[] = {L"����"};

		std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0]));
		CHECK(L"����� <<����>>�� ���ʴϴ�." == strResult);
	}

	// ����ܾ� ������
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

	// �ڱ����� ��¾��
	SECTION("SelfReferenceFormat")
	{
		std::wstring strFormat = L"{0:����� $ �Դϴ�.}";
		std::wstring strObject = L"�����";

		std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject));
		CHECK(L"����� ����� �Դϴ�." == strResult);
	}

	// ��� ��¾��
	SECTION("RecursiveFormat")
	{
		std::wstring strFormat = L"{0:����� {1,��} �����մϴ�.}";
		std::wstring strObject[] = {L"Ŭ����", L"�����"};

		std::wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0], strObject[1]));
		CHECK(L"����� ������� �����մϴ�." == strResult);
	}

	// �׷����� ��¾��
	SECTION("GroupFormat")
	{
		std::wstring strFormat = L"{0:����;�ϳ�;����}";

		CHECK(L"����" == CSFormatString::Format(strFormat, FSParam(0)));
		CHECK(L"�ϳ�" == CSFormatString::Format(strFormat, FSParam(1)));
		CHECK(L"����" == CSFormatString::Format(strFormat, FSParam(2)));
	}
}
