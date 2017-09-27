#include <iostream>
#include <experimental/generator>

template <typename T>
using generator = std::experimental::generator<T>;

static generator<int> GenerateIntegers()
{
	for (int i = 0; i < 5; i++)
	{
		co_yield i;
	}
}

static generator<int> GenerateSquares(generator<int>& anotherSource)
{
	for (int i : anotherSource)
	{
		co_yield i * i;
	}
}

void StdExpGenerator()
{
	auto integers = GenerateIntegers();
	for (int i : GenerateSquares(integers))
	{
		std::cout << i << std::endl;
	}
}
