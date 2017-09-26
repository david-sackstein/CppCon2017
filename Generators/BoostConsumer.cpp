#include <boost/coroutine2/all.hpp>
#include <functional>
#include <algorithm>
#include <iostream>

template<typename T>
using pull_type = typename boost::coroutines2::asymmetric_coroutine<T>::pull_type;

template<typename T>
using push_type = typename boost::coroutines2::asymmetric_coroutine<T>::push_type;

void LegacyConsumer(std::function<std::unique_ptr<char>()> source)
{
	std::unique_ptr<char> ptr = source();

	while (ptr)
	{
		std::cout << *ptr << std::endl;
		
		ptr = source();
	}
}

push_type<const char> CreateSink()
{
	return push_type<const char> ([](pull_type<const char>& source)
	{
		bool isFirst = true;

		LegacyConsumer([&]() -> std::unique_ptr<char>
		{
			if (!isFirst) {
				source();
			}
			isFirst = false;

			if (!source) {
				return nullptr;
			}
			return std::make_unique<char>(source.get());
		});
	});
}

void BoostSink()
{
	push_type<const char> sink = CreateSink();

	std::string hello = "Hello world";

	std::copy(hello.begin(), hello.end(), begin(sink));
}