#include "../extern/catch.hpp"
#include "../src/CSFormatString.h"

TEST_CASE("Decorate parameters by Korean grammar", "[korean][grammar]")
{
	SECTION("Casting subject particles")
	{
		auto format = L"{0,��} {0,��} {1,��} {1,��}";
		auto result = CSFormatString::Format(format, { L"����", L"����" });

		CHECK(result == L"���簡 ���簡 ������ ������");
	}

	SECTION("Casting topic particles")
	{
		auto format = L"{0,��} {0,��} {1,��} {1,��}";
		auto result = CSFormatString::Format(format, { L"����", L"����" });

		CHECK(result == L"����� ����� ������ ������");
	}

	SECTION("Casting object particles")
	{
		auto format = L"{0,��} {0,��} {1,��} {1,��}";
		auto result = CSFormatString::Format(format, { L"����", L"����" });

		CHECK(result == L"���縦 ���縦 ������ ������");
	}

	SECTION("Casting decorated parameters with particles")
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
