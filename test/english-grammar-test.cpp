#include "../extern/catch.hpp"
#include "../src/string-format.h"
#include "../src/dictionary.h"

TEST_CASE("Decorate parameters by English grammar", "[english][grammar]")
{
	class EnglishDictionary : public string_format::Dictionary
	{
	public:
		virtual std::wstring Plural(const std::wstring& singular) override
		{
			return L"wolves";
		}
	};

	auto old = string_format::SetDictionary(std::make_shared<EnglishDictionary>());

	SECTION("Casting plural parameters")
	{
		auto format = L"{0}, {0,s}";
		auto result = string_format::Format(format, L"wolf");

		CHECK(result == L"wolf, wolves");
	}

	string_format::SetDictionary(old);
}
