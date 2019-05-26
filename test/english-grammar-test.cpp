#include "../extern/catch.hpp"
#include "../src/CSFormatString.h"
#include "../src/CStringResMgr.h"

TEST_CASE("Decorate parameters by English grammar", "[english][grammar]")
{
	CStringResMgr dictionary;
	CSFormatString::SetDictionary(&dictionary);

	SECTION("Casting plural parameters")
	{
		auto format = L"{0}, {0,s}";
		auto result = CSFormatString::Format(format, { L"wolf" });

		CHECK(result == L"wolf, wolves");
	}

	CSFormatString::SetDictionary(nullptr);
}
