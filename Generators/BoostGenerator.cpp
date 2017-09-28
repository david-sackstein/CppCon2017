#include <iostream>
#include <boost/coroutine2/all.hpp>

template<typename T>
using pull_type = typename boost::coroutines2::asymmetric_coroutine<T>::pull_type;

template<typename T>
using push_type = typename boost::coroutines2::asymmetric_coroutine<T>::push_type;

static pull_type<int> GenerateIntegers()
{
	pull_type<int> source ([](push_type<int>& sink)
	{
		for (int i = 0; i < 5; i++)
		{
			sink(i);
		}
	});

	return source;
}

static void ExtractedMethod(
	pull_type<int>& anotherSource, 
	push_type<int>& sink)
{
	for (int i : anotherSource)
	{
		sink(i * i);
	}
}

static pull_type<int> GenerateSquares(pull_type<int>& anotherSource)
{
	return pull_type<int> ([&](push_type<int>& sink)
	{
		ExtractedMethod(anotherSource, sink);
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