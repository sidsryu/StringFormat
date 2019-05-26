#include "../extern/catch.hpp"
#include "../src/CSFormatString.h"

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
}
