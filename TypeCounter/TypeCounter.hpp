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

    TypeCounter(const TypeCounter&)
    {
         objects_created++;
         objects_alive++;
    }   

protected:
    virtual ~TypeCounter()
    {
        --objects_alive;
    }
    inline static int objects_created = 0;
    inline static int objects_alive = 0;
};
