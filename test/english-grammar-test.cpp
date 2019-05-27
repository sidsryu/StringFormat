#include "../extern/catch.hpp"
#include "../src/CSFormatString.h"
#include "../src/dictionary.h"

TEST_CASE("Decorate parameters by English grammar", "[english][grammar]")
{
	class EnglishDictionary : public Dictionary
	{
	public:
		virtual std::wstring Plural(const std::wstring& singular) override
		{
			return L"wolves";
		}
	};

	auto old = CSFormatString::SetDictionary(std::make_shared<EnglishDictionary>());

	SECTION("Casting plural parameters")
	{
		auto format = L"{0}, {0,s}";
		auto result = CSFormatString::Format(format, L"wolf");

		CHECK(result == L"wolf, wolves");
	}

	CSFormatString::SetDictionary(old);
}
