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
#include "../Counter/ICounter.hpp"
#include "../ObjectFactory/Factory.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Matcher.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Functional/Functional.hpp"
#include "../Common/DateTimes.hpp"
#include "../Visitor/CollectorVisitor.hpp"

#pragma once

template<size_t N>
class IMatrixParser
{
public:
    inline static constexpr size_t Order = N;
	using ElementType = std::shared_ptr<IElement>;
	using MatrixType = Matrix<N, MatrixDescriptor<N, std::shared_ptr<IElement>>>;
	MatrixType operator()() const { return exec(); };
private:
	virtual MatrixType exec() const = 0;
	virtual std::ostream& display(std::ostream& os) const = 0;
	friend std::ostream& operator<<(std::ostream& s, const IMatrixParser& m) { return s; }
};

template<>
class IMatrixParser<3>
{
public:
    inline static constexpr size_t Order = 3;
	using ElementType = std::shared_ptr<IElement>;
	using MatrixType = M3<std::shared_ptr<IElement>, MatrixDescriptor<3, std::shared_ptr<IElement>>>;
	MatrixType operator()() const { return exec(); };
private:
	virtual MatrixType exec() const = 0;
	virtual std::ostream& display(std::ostream& os) const = 0;
	friend std::ostream& operator<<(std::ostream& s, const IMatrixParser& m) { return s; }
};

template<size_t N>
class IMatrixParserBase: public IMatrixParser<N>
{
	inline static constexpr const char TypeIdentifier[] = "MatrixParserBase";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
	using Base = IMatrixParser<N>;
public:
	typename Base::MatrixType operator()() const { return exec(); };
protected:
	IMatrixParserBase(std::shared_ptr<Factory<IToken>> fT, const std::string& p):tokenFactory{fT}, path{p} {};
	std::shared_ptr<Factory<IToken>> tokenFactory;
	const std::string path;
private:
	virtual typename Base::MatrixType exec() const = 0;
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
	typename Base::MatrixType exec() const
	{
		auto u23 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2023.csv" };
        auto u24 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2024.csv" };
        auto m23r = MatrixReader(u23);
        auto m24r = MatrixReader(u24);
        auto m23S = m23r.M<2>();
        auto m24S = m24r.M<2>();
        std::vector<decltype(m23S)> accountFiles{m23S, m24S};
        M3 accountMatrix(accountFiles);
        
        auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});
        Matcher indexTokenMatcher(std::move(csvIndexTokens));
        auto elementIndexTokens = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});
        Matcher elementTokenMatcher(std::move(elementIndexTokens));
        
        return accountMatrix.Match(indexTokenMatcher).Parse(elementTokenMatcher);
	}
};

class StageParser: public IMatrixParserBase<2>
{
	using Base = IMatrixParserBase;
public:
	StageParser(std::shared_ptr<Factory<IToken>> fT, const std::string& p): IMatrixParserBase{fT, p} {};
private:
	const std::string fileName = "SN_Name.csv";
	typename Base::MatrixType exec() const
	{
        auto stringMatrix = MatrixReader(path + "//" + fileName).M<2>();

		auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"RentIndexToken"},{"ExtraCostsIndexToken"},{"HeatExtraCostsIndexToken"} });
		Matcher indexTokenMatcher(std::move(stageIndexTokens));
		auto elementIndexTokens = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});
		Matcher elementTokenMatcher(std::move(elementIndexTokens));
	
		return stringMatrix.Match(indexTokenMatcher).Parse(elementTokenMatcher);
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
