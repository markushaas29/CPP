#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "Matrix.hpp"
#include "MatrixReader.hpp"
#include "MatrixDescriptor.hpp"
#include "MatrixComposition.hpp"
#include "M3.hpp"
#include "MatrixStrategy.hpp"
#include "../Builder/Builder.hpp"
#include "../ObjectFactory/Factory.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Matcher.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Functional/Functional.hpp"
#include "../Common/DateTimes.hpp"
#include "../Visitor/CollectorVisitor.hpp"

#pragma once

class IBaseMatrixParser
{
public:
	using ElementType = std::shared_ptr<IElement>;
private:
	virtual std::ostream& display(std::ostream& os) const = 0;
	friend std::ostream& operator<<(std::ostream& s, const IBaseMatrixParser& m) { return s; }
};

template<size_t N>
class IMatrixParser: public IBaseMatrixParser
{
public:
    inline static constexpr size_t Order = N;
	using Base = IBaseMatrixParser;
	using MatrixType = Matrix<N, MatrixDescriptor<N, Base::ElementType>>;
	using StringMatrix = Matrix<N, MatrixDescriptor<N, std::string>>;
	MatrixType operator()(bool h = false) const { return exec(h); };
	StringMatrix M() const { return matrix(); };
private:
	virtual MatrixType exec(bool h = false) const = 0;
	virtual StringMatrix matrix() const = 0;
};

template<>
class IMatrixParser<3>: public IBaseMatrixParser
{
public:
    inline static constexpr size_t Order = 3;
	using Base = IBaseMatrixParser;
	using MatrixType = M3<Base::ElementType, MatrixDescriptor<3, Base::ElementType>>;
	MatrixType operator()(bool h = false) const { return exec(h); };
	M3<std::string> M() const { return matrix(); };
private:
	virtual MatrixType exec(bool h = false) const = 0;
	virtual M3<std::string> matrix() const = 0;
};

template<size_t N>
class IMatrixParserBase: public IMatrixParser<N>
{
	inline static constexpr const char TypeIdentifier[] = "MatrixParserBase";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
	using Base = IMatrixParser<N>;
public:
	typename Base::MatrixType operator()(bool h = false) const { return exec(h); };
protected:
	IMatrixParserBase(std::shared_ptr<Factory<IToken>> fT, const std::string& p):tokenFactory{fT}, path{p} {};
	std::shared_ptr<Factory<IToken>> tokenFactory;
	const std::string path;
private:
	virtual typename Base::MatrixType exec(bool h = false) const = 0;
	virtual std::ostream& display(std::ostream& os) const { return os;};
	friend std::ostream& operator<<(std::ostream& s, const IMatrixParserBase& m) { return s; }
};

class AccountParser: public IMatrixParserBase<3>
{
	using Base = IMatrixParserBase;
public:
	AccountParser(std::shared_ptr<Factory<IToken>> fT, const std::string& p): IMatrixParserBase{fT, p} {};
private:
	const std::string fileName = "SN_Name.csv";
	M3<std::string> matrix() const
	{
		auto u23 = std::string{ path + "//U_2023.csv" };
        auto u24 = std::string{ path + "//U_2024.csv" };
        auto m23r = MatrixReader(u23);
        auto m24r = MatrixReader(u24);
        auto m23S = m23r.M<2>();
        auto m24S = m24r.M<2>();
        std::vector<decltype(m23S)> accountFiles{m23S, m24S};
        return M3(accountFiles);
	}
	typename Base::MatrixType exec(bool h = false) const
	{
        auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});
        auto elementIndexTokens = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});
        
		return matrix().Parse(Matcher(std::move(csvIndexTokens)), Matcher(std::move(elementIndexTokens)));
	}
};

template<typename T>
class CounterParser: public IMatrixParserBase<2>
{
	using Base = IMatrixParserBase;
	using Type = T;
public:
	CounterParser(std::shared_ptr<Factory<IToken>> fT, const std::string& p): IMatrixParserBase{fT, p} {};
private:
	typename Base::StringMatrix matrix() const	{  return MatrixReader(path).template M<2>();	}
	typename Base::MatrixType exec(bool h = false) const
	{
        auto elementTokens = (*tokenFactory)({{"DateToken"},{ Type::Unit::TokenName }});
        return matrix().Parse(Matcher(std::move(elementTokens)));
	}
};

class StageParser: public IMatrixParserBase<2>
{
	using Base = IMatrixParserBase;
public:
	StageParser(std::shared_ptr<Factory<IToken>> fT, const std::string& p): IMatrixParserBase{fT, p} {};
private:
	const std::string fileName = "SN_Name.csv";
	typename Base::StringMatrix matrix() const	{  return MatrixReader(path + "//" + fileName).M<2>();	}
	typename Base::MatrixType exec(bool h = false) const
	{
		auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"RentIndexToken"},{"ExtraCostsIndexToken"},{"HeatExtraCostsIndexToken"}, {"GarageRentIndexToken"} });
		return matrix().ParseByMatch(Matcher(std::move(stageIndexTokens)), h);
	}
};

class HallParser: public IMatrixParserBase<2>
{
	using Base = IMatrixParserBase;
public:
	HallParser(std::shared_ptr<Factory<IToken>> fT, const std::string& p): IMatrixParserBase{fT, p} {};
private:
	const std::string fileName = "Hall.csv";
	typename Base::StringMatrix matrix() const	{  return MatrixReader(path + "//" + fileName).M<2>();	}
	typename Base::MatrixType exec(bool h = false) const
	{
		auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StreetIndexToken"},{"StreetnumberIndexToken"},{"TownIndexToken"},{"PostcodeIndexToken"},{"RentIndexToken"},{"AreaIndexToken"},{"SumPerAreaIndexToken"},{"LengthIndexToken"} });
		return matrix().ParseByMatch(Matcher(std::move(stageIndexTokens)), h);
	}
};

//class Stages: public StageBase
//{
//	using Base = StageBase;
//public:
//	Stages(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): StageBase{fT,fE,fB, p} {};
//private:
//	virtual typename Base::MatrixType matrix(Matrix<2, MatrixDescriptor<2, std::string>> m, Matcher&& im, Matcher&& em) const { return m.Match(im).Parse(em).Cols(8,9,10)[1];	}
//};
//
//class ExtraCosts: public StageBase
//{
//	using Base = StageBase;
//public:
//	ExtraCosts(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): StageBase{fT,fE,fB, p} {};
//private:
//	virtual typename Base::MatrixType matrix(Matrix<2, MatrixDescriptor<2, std::string>> m, Matcher&& im, Matcher&& em) const
//	{
//        int payment = m.Match(im).Parse(em).Cols(8,9,10);
//        std::vector<std::shared_ptr<IElement>> extras = 
//			{	std::make_shared<Quantity<Sum>>(payment[1][1].To<Quantity<Sum>>()+payment[1][2].To<Quantity<Sum>>()),
//				std::make_shared<Quantity<Sum>>(payment[2][1].To<Quantity<Sum>>()+payment[2][2].To<Quantity<Sum>>())
//			}; 
//        std::vector<std::shared_ptr<IElement>> extrasR;
//
//        std::for_each(extras.begin(), extras.end(),[&](auto& e) { extrasR.push_back(std::make_shared<Quantity<Sum>>(e->template To<Quantity<Sum>>() * Quantity<Scalar>{12}) ); });
//
//        return Matrix<Order,DescriptorType>(typename Base::DescriptorType({extrasR.size()}),ToDataType(extrasR));
//	}
//};
