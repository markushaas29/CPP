#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "MatrixQuery.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"

#pragma once

template<std::size_t, typename> class Matrix;

template<typename T, typename Q>
class IMatrixStrategy
{
public:
	using MatrixType = T;
	using ElementType = T::ElementType;
	virtual Q operator()(T& m) = 0;
	virtual std::string_view Name() = 0;
};

template<typename T, typename Q = Quantity<Sum>>
class BaseMatrixStrategy : public IMatrixStrategy<T,Q>
{
	using Base = IMatrixStrategy<T,Q>;
	using QueryType = MatrixQuery<T>;
	using UnitType = FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>>;
	using FactoryType = std::shared_ptr<FactoryStack<IMatrixCategory<std::string>, Factory<IMatrixCategory<std::string>>>>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixStrategy";
    inline static constexpr Literal TypeId{TypeIdentifier};

	BaseMatrixStrategy(FactoryType f, const std::string& n): name{n}, factory{f} {}
	virtual Q operator()(Base::MatrixType& m) 
	{
		Matrix<2,MatrixDescriptor<2,std::string>> m33s = {
                 {"1", "2" ,"3"},
                 {"4", "5", "6"},
                 {"7", "8", "9"},
             };
		FactoryUnit<std::string, std::string> fuy = {"C", "2022"};
		std::for_each(units.begin(), units.end(), [&](auto& u) { u.Add(fuy); });
        auto mq = MatrixQuery<typename Base::MatrixType,std::string>(factory, units);
        auto resM = m.M(mq).Cols(4,6,7,9,11);
		return Quantity<Sum>(resM.ColSum(4));
	}
	virtual std::string_view Name() { return name; };
private:
	std::string name;
	std::vector<UnitType> units;
	std::unique_ptr<IMatrixQuery<typename Base::MatrixType, typename Base::ElementType>> query;	
	FactoryType factory;
	Base::ElementType matrix;
	template<typename U> using IsT =  Is<U,TypeId>;
	//friend std::ostream& operator<<(std::ostream& s, const MatrixStrategy& me) { return s<<me.matrix;  }
};

//template<typename T>
//class BinaryMatrixStrategy : public IMatrixStrategy<T>
//{
//	using Base = IMatrixStrategy<T>;
//public:
//	inline static constexpr const char TypeIdentifier[] = "MatrixStrategy";
//    inline static constexpr Literal TypeId{TypeIdentifier};
//
//	BinaryMatrixStrategy(std::vector<typename Base::ElementType> e): matrix(e) {}
//	
//	virtual Base::MatrixType operator()(const Base::MatrixType& m) { std::cout<<"Bin"<<std::endl; return typename Base::MatrixType(); };
//private:
//	std::vector<typename Base::ElementType> matrix;
//	template<typename U> using IsT =  Is<U,TypeId>;
//};
//
//template<typename T>
//class UnaryMatrixStrategy : public IMatrixStrategy<T>
//{
//	using Base = IMatrixStrategy<T>;
//	using QueryType = MatrixQuery<T>;
//	using UnitType = FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>>;
//public:
//	inline static constexpr const char TypeIdentifier[] = "MatrixStrategy";
//    inline static constexpr Literal TypeId{TypeIdentifier};
//
//	UnaryMatrixStrategy(std::string n): name{n} {}
//	
//	virtual Base::MatrixType operator()(const Base::MatrixType& m) { std::cout<<"Un"<<std::endl;return typename Base::MatrixType(); };
//	virtual std::string_view Name() { return name; };
//	//const MatrixType& operator()() const { return matrix; } 
//	//decltype(auto) operator()(size_t i, std::function<bool(const typename MatrixType::ElementType& i)> pred = [](const typename MatrixType::ElementType& e) { return e==0; }) const 
//	//{
//	//FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUZie  = { "A",  {{"EQ", "DE10660501011022126625"}, {"C", "Miete"},{"C","2022"}}}; 
////            factoryunit<std::string, std::vector<factoryunit<std::string, std::string>>> fuzei  = { "a",  {{"c", "zeiher"}, {"c", "miete"},{"c","2022"}}}; 
////            auto mz = matrixquery<decltype(m22s),std::string>(pfs, {fuzie, fuzei});
////            auto mzeiher =m22_23.m(mz).cols(4,6,7,9,11);
//	//	if constexpr (MatrixType::Order==2)
//    //    {
//    //	    typename MatrixType::IsT<Throwing>(Format("Index: ",i ," exceeds extents!"))(i<matrix.Cols());
//    //	    std::vector<typename MatrixType::DataType> result;
//    //	    std::array<size_t,MatrixType::Order> e = copy(matrix.descriptor.Extents());
//
//    //	    for(int j = 0; j < matrix.Rows(); ++j)
//    //	    {
//    //	        auto row = matrix.row(j);
//	//			if(pred(*row[i]))
//    //	        	std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
//    //	    }
//
//
//    //	    e[0] = result.size() / matrix.Cols();
//    //	    
//	//		return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix.descriptor.Strides())}, result);
//    //    }
//	//}
//	//decltype(auto) operator()(std::function<bool(const std::vector<typename MatrixType::DataType>& i)> pred) const 
//	//{
//	//	if constexpr (MatrixType::Order==2)
//    //    {
//    //	    std::vector<typename MatrixType::DataType> result;
//    //	    std::array<size_t,MatrixType::Order> e = copy(matrix.descriptor.Extents());
//    //	    
//	//		for(int i = 0; i < matrix.Rows(); ++i)
//    //	    {
//    //	        auto row = matrix.row(i);
//	//			if(pred(row))
//    //	        	std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
//    //	    }
//    //	    
//    //	    e[0] = result.size() / matrix.Cols();
//    //	    
//	//		return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix.descriptor.Strides())}, result);
//    //    }
//	//}
//private:
//	std::string name;
//	std::vector<UnitType> units;
//	std::unique_ptr<IMatrixQuery<typename Base::MatrixType, typename Base::ElementType>> query;	
//	Base::ElementType matrix;
//	template<typename U> using IsT =  Is<U,TypeId>;
//	//friend std::ostream& operator<<(std::ostream& s, const MatrixStrategy& me) { return s<<me.matrix;  }
//};
//
