 #include "../Unit/Unit.hpp"
 #include "../Unit/Unit.hpp"
 #include "../Quantity/QuantityRatio.hpp"

#pragma once 
class Date;
class IElement;
template<typename,typename, typename> class Quantity;

class BaseVisitor
{
public:
	virtual ~BaseVisitor(){}
};

template<class T, typename R = void>
class MatrixVisitor
{
public:
	using ReturnType = R;
	virtual ReturnType Visit(T&) = 0;
};
