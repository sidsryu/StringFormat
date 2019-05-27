# StringFormat
String formater for string tables to localize


## Installation

VS2019 project. Copy all source files in `/src `.


## Normal Usage

```
namespace sf = string_format;
std::wstring result;

// indexed parameter
result = sf::Format(L"{1}, {0}", L"apple", L"orange");
assert(result == L"orange, apple");

// dynamic type parameter
result = sf::Format(L"Test {0}, {1}, {2}", 100, 3.14, L"apple");
assert(result == L"Test 100, 3.14, apple");

// use multiple index
result = sf::Format(L"{0}, {1}, {0}", L"apple", L"orange");
assert(result == L"apple, orange, apple");

// enumerated parameter
result = sf::Format(L"{0:zero;one;many}", 0);
assert(result == L"zero");

result = sf::Format(L"{0:zero;one;many}", 1);
assert(result == L"one");

result = sf::Format(L"{0:zero;one;many}", 2);
assert(result == L"many");

result = sf::Format(L"{0:zero;one;many}", 100);
assert(result == L"many");

// self reference enumerated paramter
result = sf::Format(L"{0:zero;$;many}", 1);
assert(result == L"1");

result = sf::Format(L"{0:no;a;$}", 100);
assert(result == L"100");

```


## Usage For Localize Korean 

```
namespace sf = string_format;
std::wstring result;

// auto matching subject particles
result = sf::Format(L"{0,이} {0,가}", L"사과");
assert(result == L"사과가 사과가");

// support other grammar
result = sf::Format(L"{0,을} {0,은} {0,과}", L"사과");
assert(result == L"사과를 사과는 사과와");

// decorated parameter without affix
result = sf::Format(L"{0,을:<<$>>} 맛있게.", L"사과");
assert(result == L"<<사과>>를 맛있게");

// recursive decorated parameter
result = sf::Format(L"{0,은:$({1})} 맛있다.", L"사과", L"달다");
assert(result == L"사과(달다)는 맛있다.");
````


## Usage For Localize English

```
namespace sf = string_format;
std::wstring result;

auto dict = std::make_shared<MyEnglishDictionay>();
auto old = sf::SetDictionary(dict);

// to plural
result = sf::Format(L"{0}, {0,s}", L"wolf");
assert(result == L"wolf, wolves");

sf::SetDictionary(old);
```

## Licensing

This project is licensed under the terms of [the MIT licnese][license].
