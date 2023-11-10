#pragma once

template<typename T>
decltype(auto) ToDataType(const std::vector<T>& v)
{
	std::vector<std::shared_ptr<T>> result;
	std::for_each(v.begin(),v.end(), [&result](const T& t) { result.push_back(std::make_shared<T>(t)); });
	return result;
};
