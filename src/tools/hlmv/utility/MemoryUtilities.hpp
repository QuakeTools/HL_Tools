#pragma once

#include <type_traits>
#include <utility>
#include <vector>

namespace utility
{
//See https://stackoverflow.com/questions/36994727/how-do-i-write-a-make-vector-similar-to-stdmake-tuple
template<typename T0, typename... T>
auto make_vector(T0&& arg0, T&&... args)
{
	using first_type = std::decay_t<T0>;

	std::vector<first_type> vec;

	vec.reserve(sizeof...(args) + 1);

	vec.emplace_back(std::move(arg0));

	using expander = int[];

	(void)expander
	{
		((void)vec.emplace_back(std::forward<T>(args)), 0)..., 0
	};

	return vec;
}
}
