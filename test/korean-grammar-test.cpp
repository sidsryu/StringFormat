#include "../extern/catch.hpp"
#include "../src/CSFormatString.h"

TEST_CASE("Decorate parameters by Korean grammar", "[korean][grammar]")
{
	SECTION("Casting subject particles")
	{
		auto format = L"{0,이} {0,가} {1,이} {1,가}";
		auto result = CSFormatString::Format(format, { L"전사", L"도적" });

		CHECK(result == L"전사가 전사가 도적이 도적이");
	}

	SECTION("Casting topic particles")
	{
		auto format = L"{0,은} {0,는} {1,은} {1,는}";
		auto result = CSFormatString::Format(format, { L"전사", L"도적" });

		CHECK(result == L"전사는 전사는 도적은 도적은");
	}

	SECTION("Casting object particles")
	{
		auto format = L"{0,을} {0,를} {1,을} {1,를}";
		auto result = CSFormatString::Format(format, { L"전사", L"도적" });

		CHECK(result == L"전사를 전사를 도적을 도적을");
	}

	SECTION("Casting decorated parameters with particles")
	{
		auto format = L"당신은 {0,를:<<$>>} 먹습니다.";
		auto result = CSFormatString::Format(format, { L"사과" });

		CHECK(result == L"당신은 <<사과>>를 먹습니다.");
	}

	SECTION("Recursive decorated parameters with particles in Korean grammer")
	{
		auto format = L"{0,를:$({1})}";
		auto result = CSFormatString::Format(format, { L"사과", L"달다" });

		CHECK(result == L"사과(달다)를");
	}
}
