#pragma once

template <typename T>
class TypeCounter
{
public:
    TypeCounter()
    {
        ++objects_created;
        ++objects_alive;
		//std::cout<<T::Identifier<<"\tCreated: "<<objects_created<<"\tAlive:\t"<<objects_alive<<std::endl;
    }
    static auto All()   { return objects_created; }
    static auto Copied()   { return objects_copied; }

    TypeCounter(const TypeCounter&)
    {
         ++objects_created;
         ++objects_copied;
         ++objects_alive;
    }   

protected:
    virtual ~TypeCounter()
    {
        --objects_alive;
    }
    inline static int objects_created = 0;
    inline static int objects_copied = 0;
    inline static int objects_alive = 0;
};

template<typename I,  typename... DS>
class TypeCounts
{
        using Tup = std::tuple<DS...>;
public:
        inline static constexpr const char TypeIdentifier[] = "Builder";
        inline static constexpr Literal TypeId{TypeIdentifier};
        template<size_t N>
        static auto Exec()
        {
                if constexpr (std::tuple_size<Tup>()==N)
                        return ;
                else
                {
                        using Type = std::tuple_element_t<N,Tup>;
						std::cout<<Type::Identifier<<" "<<TypeCounter<Type>::All()<<"\t copied:"<<TypeCounter<Type>::Copied()<<std::endl;
                        return Exec<N+1>();
                }
        }
};

