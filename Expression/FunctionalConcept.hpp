 #pragma once
 
template<typename P> 
concept FunctionalConcept = requires(P p)
{
    P::LeftType;
    P::RightType;
	(double)p;
	p();
	p(4);
};
