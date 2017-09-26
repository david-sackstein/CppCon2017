#include <boost/coroutine2/all.hpp>
#include <iostream>

template<typename T>
using pull_type = typename boost::coroutines2::asymmetric_coroutine<T>::pull_type;

template<typename T>
using push_type = typename boost::coroutines2::asymmetric_coroutine<T>::push_type;

static pull_type<int> GenerateIntegers()
{
	return pull_type<int> ([](push_type<int>& sink)
	{
		for (int i = 0; i < 5; i++)
		{
			sink(i);
		}
	});
}

static pull_type<int> GenerateSquares(pull_type<int>& source)
{
	return pull_type<int> ([&](push_type<int>& sink)
	{
		for (int i : source)
		{
			sink(i * i);
		}
	});
}

void BoostGenerator()
{
	auto integers = GenerateIntegers();

	for (int i : GenerateSquares(integers))
	{
		std::cout << i << std::endl;
	}
}