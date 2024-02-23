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
