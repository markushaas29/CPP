#include <tuple>
#include <type_traits>

#pragma once

template <typename T, typename Tuple>
struct has_type;

template <typename T>
struct has_type<T, std::tuple<>> : std::false_type {};

template <typename T, typename U, typename... Ts>
struct has_type<T, std::tuple<U, Ts...>> : has_type<T, std::tuple<Ts...>> {};

template <typename T, typename... Ts>
struct has_type<T, std::tuple<T, Ts...>> : std::true_type {};
	
template <typename T, typename Tuple>
using tuple_contains_type = typename has_type<T, Tuple>::type;

template <typename T>
constexpr bool IsTuple = false;

template<typename ... types>
constexpr bool IsTuple<std::tuple<types...>>   = true;
