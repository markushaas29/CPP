#include "../Logger/Logger.hpp"
#include "../String/Literal.hpp"
#include "../String/Format.hpp"

#pragma once

//template<typename T>
class IChain
{
//protected:
public:
	IChain(){}
//	bool operator()(bool c)
//	{
//		return c;
//	};
	static constexpr std::size_t N = 1;
private:
	friend std::ostream& operator<<(std::ostream& s, const IChain& c) { return c.display(s);  }
    virtual std::ostream& display(std::ostream& s) const { return s; }; //= 0;
};
