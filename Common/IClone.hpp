#include <memory>

#pragma once

template<typename T>
class IClone
{   
	public:
		auto Clone() const  { return clone(); };
	private:
		virtual std::unique_ptr<T> clone() const  = 0;
};
