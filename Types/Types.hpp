#include <tuple>

#pragma once

template<typename... T>
class Types    
{
    using Tuple = std::tuple<T...>;
public:
    inline static constexpr const char TypeIdentifier[] = "Types";
    
    Types() = delete;
    template<size_t N> using At = typename std::get<N, Tuple>::type;
private:
    friend std::ostream& operator<<(std::ostream& s, const Types& c){return s;}    
     
};   





