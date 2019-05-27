#include "../extern/catch.hpp"
#include "../src/CSFormatString.h"

TEST_CASE("Output replaced a string from a formated string with prarmeters", "[basic]")
{	
	SECTION("No arguments")
	{
		auto format = L"test {0}, {1}, {2}";
		auto result = string_format::Format(format);

		CHECK(result == L"test 0, 1, 2");
	}

	SECTION("Number type arguments")
	{
		auto format = L"test {0}, {1}";
		auto result = string_format::Format(format, 100, 3.14);

		CHECK(result == L"test 100, 3.14");
	}

	SECTION("Empty string arguments")
	{
		auto format = L"test {0}{1}{2}";
		auto result = string_format::Format(format, L"first", L"", L"third");

		CHECK(result == L"test firstthird");
	}

	SECTION("Empty string arguments")
	{
		auto format = L"test {0}{1}{2}";
		auto result = string_format::Format(format, L"", L"second", L"");

		CHECK(result == L"test second");
	}

	SECTION("Use multiple placed parameters")
	{
		auto format = L"{0}, {1}, {0}";
		auto result = string_format::Format(format, L"apple", L"orange");

		CHECK(result == L"apple, orange, apple");
	}

	SECTION("Use free placed parameters")
	{
		auto format = L"{2}, {0}, {1}";
		auto result = string_format::Format(format, L"apple", L"orange", L"banana");

		CHECK(result == L"banana, apple, orange");
	}

	SECTION("Casting decorated parameters")
	{
		auto format = L"{0:<<$>>}";
		auto result = string_format::Format(format, L"apple");

		CHECK(result == L"<<apple>>");
	}

	SECTION("Recursive decorated parameters")
	{
		auto format = L"{0:{1}${1}}";
		auto result = string_format::Format(format, L"apple", L"~~");

		CHECK(result == L"~~apple~~");
	}

	SECTION("Enumerated parameters")
	{
		auto format = L"{0:zero;one;many}";

		SECTION("at first")
		{
			auto result = string_format::Format(format, 0);
			CHECK(result == L"zero");
		}

		SECTION("at second")
		{
			auto result = string_format::Format(format, 1);
			CHECK(result == L"one");
		}

		SECTION("at third")
		{
			auto result = string_format::Format(format, 2);
			CHECK(result == L"many");
		}

		SECTION("at overflow")
		{
			auto result = string_format::Format(format, 10);
			CHECK(result == L"many");
		}
	}

	SECTION("Recursive enumerated parameters")
	{
		auto format = L"{2:$;{0};{1}}";

		SECTION("at first")
		{
			auto result = string_format::Format(format, L"apple", L"orange", 0);
			CHECK(result == L"0");
		}

		SECTION("at second")
		{
			auto result = string_format::Format(format, L"apple", L"orange", 1);
			CHECK(result == L"apple");
		}

		SECTION("at third")
		{
			auto result = string_format::Format(format, L"apple", L"orange", 2);
			CHECK(result == L"orange");
		}
	}
}
