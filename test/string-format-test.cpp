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
		auto format = L"{0,이} {0,가} {1,이} {1,가}";
		auto result = CSFormatString::Format(format, { L"전사", L"도적" });

		CHECK(result == L"전사가 전사가 도적이 도적이");
	}

	SECTION("Casting topic particles in Korean grammar")
	{
		auto format = L"{0,은} {0,는} {1,은} {1,는}";
		auto result = CSFormatString::Format(format, { L"전사", L"도적" });

		CHECK(result == L"전사는 전사는 도적은 도적은");
	}

	SECTION("Casting object particles in Korean grammar")
	{
		auto format = L"{0,을} {0,를} {1,을} {1,를}";
		auto result = CSFormatString::Format(format, { L"전사", L"도적" });

		CHECK(result == L"전사를 전사를 도적을 도적을");
	}

	SECTION("Casting decorated parameters with particles in Korean grammer")
	{
		auto format = L"당신은 {0,를:<<$>>} 먹습니다.";
		auto result = CSFormatString::Format(format, { L"사과" });

		CHECK(result == L"당신은 <<사과>>를 먹습니다.");
	}

	SECTION("Recursive decorated parameters with particles in Korean grammer")
	{
		auto format = L"{0:{1,은} $ {1,을}}";
		auto result = CSFormatString::Format(format, { L"사과", L"오렌지" });

		CHECK(result == L"오렌지는 사과 오렌지를");
	}
}
