#include "../extern/catch.hpp"
#include "../src/CSFormatString.h"
#include "../src/CStringResMgr.h"

TEST_CASE("Output replaced a string from a formated string with prarmeters", "[basic]")
{
	SECTION("No arguments")
	{
		auto format = L"test {0}, {1}, {2}";
		auto result = CSFormatString::Format(format);

		CHECK(result == L"test 0, 1, 2");
	}

	SECTION("Empty string arguments")
	{
		auto format = L"test {0}{1}{2}";
		auto result = CSFormatString::Format(format, { L"first", L"", L"third" });

		CHECK(result == L"test firstthird");
	}

	SECTION("Empty string arguments")
	{
		auto format = L"test {0}{1}{2}";
		auto result = CSFormatString::Format(format, { L"", L"second", L"" });

		CHECK(result == L"test second");
	}

	SECTION("Use multiple placed parameters")
	{
		auto format = L"{0}, {1}, {0}";
		auto result = CSFormatString::Format(format, { L"apple", L"orange" });

		CHECK(result == L"apple, orange, apple");
	}

	SECTION("Use free placed parameters")
	{
		auto format = L"{2}, {0}, {1}";
		auto result = CSFormatString::Format(format, { L"apple", L"orange", L"banana" });

		CHECK(result == L"banana, apple, orange");
	}

	SECTION("Casting decorated parameters")
	{
		auto format = L"{0:<<$>>}";
		auto result = CSFormatString::Format(format, { L"apple" });

		CHECK(result == L"<<apple>>");
	}

	SECTION("Recursive decorated parameters")
	{
		auto format = L"{0:{1}${1}}";
		auto result = CSFormatString::Format(format, { L"apple", L"~~" });

		CHECK(result == L"~~apple~~");
	}

	SECTION("Tests for require dictionary")
	{
		CStringResMgr dictionary;
		CSFormatString::SetDictionary(&dictionary);

		SECTION("Casting plural parameters in English grammer")
		{
			auto format = L"{0}, {0,s}";
			auto result = CSFormatString::Format(format, { L"wolf" });

			CHECK(result == L"wolf, wolves");
		}

		CSFormatString::SetDictionary(nullptr);
	}

	SECTION("Enumerated parameters")
	{
		auto format = L"{0:zero;one;many}";

		SECTION("Enumerated parameters at first")
		{
			auto result = CSFormatString::Format(format, { 0 });
			CHECK(result == L"zero");
		}

		SECTION("Enumerated parameters at second")
		{
			auto result = CSFormatString::Format(format, { 1 });
			CHECK(result == L"one");
		}

		SECTION("Enumerated parameters at third")
		{
			auto result = CSFormatString::Format(format, { 2 });
			CHECK(result == L"many");
		}

		SECTION("Enumerated parameters at overflow")
		{
			auto result = CSFormatString::Format(format, { 10 });
			CHECK(result == L"many");
		}
	}

	SECTION("Casting subject particles in Korean grammar")
	{
		auto format = L"{0,��} {0,��} {1,��} {1,��}";
		auto result = CSFormatString::Format(format, { L"����", L"����" });

		CHECK(result == L"���簡 ���簡 ������ ������");
	}

	SECTION("Casting topic particles in Korean grammar")
	{
		auto format = L"{0,��} {0,��} {1,��} {1,��}";
		auto result = CSFormatString::Format(format, { L"����", L"����" });

		CHECK(result == L"����� ����� ������ ������");
	}

	SECTION("Casting object particles in Korean grammar")
	{
		auto format = L"{0,��} {0,��} {1,��} {1,��}";
		auto result = CSFormatString::Format(format, { L"����", L"����" });

		CHECK(result == L"���縦 ���縦 ������ ������");
	}

	SECTION("Casting decorated parameters with particles in Korean grammer")
	{
		auto format = L"����� {0,��:<<$>>} �Խ��ϴ�.";
		auto result = CSFormatString::Format(format, { L"���" });

		CHECK(result == L"����� <<���>>�� �Խ��ϴ�.");
	}

	SECTION("Recursive decorated parameters with particles in Korean grammer")
	{
		auto format = L"{0:{1,��} $ {1,��}}";
		auto result = CSFormatString::Format(format, { L"���", L"������" });

		CHECK(result == L"�������� ��� ��������");
	}
}
