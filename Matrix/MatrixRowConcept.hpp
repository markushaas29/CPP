 #pragma once
 
template<typename P> 
concept MatrixRowConcept = requires(P p)
{
    P::Size;
    P::TypeIdentifier;
    p.template At<0>();
};
