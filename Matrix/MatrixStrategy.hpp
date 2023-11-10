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

template<typename Q, typename M, typename U>
class StrategyResult
{
public:
	using QuantityType = Q;
	using MatrixType = M;
	using UnitType = U;
	StrategyResult(const QuantityType& q, const MatrixType& m, const std::vector<UnitType>& u, const std::string& n =""): result{q}, items(m), units{u}, name{n} {};
	decltype(auto) Result() { return result; }
	decltype(auto) Items() { return items; }
private:
	friend 	std::ostream& operator<<(std::ostream& out, const StrategyResult& s)
	{ 
		out<<"Name: "<<s.name<<"Units:\n";
		std::for_each(s.units.cbegin(), s.units.cend(), [&](auto& u) { out<<u<<"\n"; });
		return out<<"\nItems:\n"<<s.items<<"\n\nResult: "<<s.result;	
	}
	QuantityType result;
	MatrixType items;
	std::vector<UnitType> units;
	std::string name;
};

template<typename T, typename Q, typename U>
class IMatrixStrategy
{
public:
	using MatrixType = T;
	using QuantityType = Q;
	using UnitType = U;
	using ElementType = T::ElementType;
	using ResultType = StrategyResult<QuantityType,MatrixType,UnitType>;
	virtual ResultType operator()(T& m) const = 0;
	virtual std::string_view Name() = 0;
};

template<typename T, typename Q = Quantity<Sum>>
class BaseMatrixStrategy : public IMatrixStrategy<T,Q,FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>>>
{
protected:
	using Base = IMatrixStrategy<T,Q,FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>>>;
	using QueryType = MatrixQuery<T>;
	using UnitType = FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>>;
	using FactoryType = std::shared_ptr<FactoryStack<IMatrixCategory<std::string>, Factory<IMatrixCategory<std::string>>>>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixStrategy";
    inline static constexpr Literal TypeId{TypeIdentifier};

	BaseMatrixStrategy(FactoryType f, const std::vector<UnitType>& u, const std::string& n): name{n}, units{u},factory{f} {}
	virtual typename Base::ResultType operator()(Base::MatrixType& m) const
	{
		std::vector<UnitType> eunits = enrich(units);
        auto mq = MatrixQuery<typename Base::MatrixType,std::string>(factory, eunits);
        auto resM = m.M(mq);
		auto q = Quantity<Sum>(resM.ColSum(11));
		return typename Base::ResultType(q,resM,eunits,name);
	}
	virtual std::string_view Name() { return name; };
protected:
	virtual std::vector<UnitType> enrich(const std::vector<UnitType>& v) const = 0;
private:
	std::string name;
	std::vector<UnitType> units;
	FactoryType factory;
	template<typename U> using IsT =  Is<U,TypeId>;
	friend std::ostream& operator<<(std::ostream& s, const BaseMatrixStrategy& m) 
	{ 
		s<<"Name: "<<m.name<<std::endl;
		std::vector<UnitType> eunits = m.enrich(m.units);
		std::for_each(eunits.begin(), eunits.end(), [&](auto& u) { s<<u<<"\n"; });
		return s;  
	}
};

template<typename T, typename Q = Quantity<Sum>>
class YearStrategy : public BaseMatrixStrategy<T,Q>
{
	using Base = BaseMatrixStrategy<T,Q>;
public:
	YearStrategy(typename Base::FactoryType f, const std::vector<typename Base::UnitType>& u, const Year& y,const std::string& n): Base{f,u,n}, year{y} {}
private:
	Year year;
	virtual std::vector<typename Base::UnitType> enrich(const std::vector<typename Base::UnitType>& v) const { 
		std::vector<typename Base::UnitType> result(v.cbegin(), v.cend());	
		FactoryUnit<std::string, std::string> fuy = {"C", year.ToString()};
		std::for_each(result.begin(), result.end(), [&](auto& u) { u.Add(fuy); });
		return result;
	}
};

template<typename T, typename Q = Quantity<Sum>>
class IDStrategy : public BaseMatrixStrategy<T,Q>
{
	using Base = BaseMatrixStrategy<T,Q>;
public:
	IDStrategy(typename Base::FactoryType f, const std::vector<typename Base::UnitType>& u, const Year& y, const std::string& i, const std::string& n): Base{f,u,n}, year{y}, id{i} {}
private:
	Year year;
	std::string id;
	virtual std::vector<typename Base::UnitType> enrich(const std::vector<typename Base::UnitType>& v) const { 
		std::vector<typename Base::UnitType> result(v.cbegin(), v.cend());	
		FactoryUnit<std::string, std::string> fuy = {"C", year.ToString()};
		FactoryUnit<std::string, std::string> fuID = {"C", id};
		std::for_each(result.begin(), result.end(), [&](auto& u) { u.Add(fuy); });
		std::for_each(result.begin(), result.end(), [&](auto& u) { u.Add(fuID); });
		auto inv = typename Base::UnitType{ "A",{{"EQ", "DE68600501010002057075"}, {"C","Rechnung"}, fuID, {"C", year.Next().ToString()}}};
		result.push_back(inv);
		return result;
	}
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
