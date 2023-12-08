 #include <vector>

#pragma once 
 
template<typename T>
class Unique
{
public:
	using Type = T;
	template<typename P>
	auto operator()(const std::vector<P>& v) const
	{
		std::vector<T> result;
		std::for_each(v.cbegin(), v.cend(), [&](auto& vis) 
			{ 
				if(std::find_if(result.begin(), result.end(), [&](auto l) { return l == vis.template To<T>(); } ) == result.end())
					result.push_back(vis.template To<T>()); 
			});
		return result;
	}
};

template<typename T>
class Less
{
public:
	using Type = T;
	Less(const T& v): value{v} {}
	template<typename P>
	auto operator()(const std::vector<P>& v) const
	{
		std::vector<T> result;
		std::for_each(v.cbegin(), v.cend(), [&](auto& vis) 
			{ 
				if(vis.template To<T>() < value)
					result.push_back(vis.template To<T>());
			});
		return result;
	}
private:
	T value;
};
