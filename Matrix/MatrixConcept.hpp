 #pragma once
 
template<typename P> 
concept MatrixConcept = requires(P p)
{
    P::Order;
};
