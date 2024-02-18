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

class IMatrixParser
{
public:
    inline static constexpr size_t Order = 2;
	using ElementType = std::shared_ptr<IElement>;
	using MatrixType = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>;
	MatrixType operator()() const { return exec(); };
private:
	virtual MatrixType exec() const = 0;
	virtual std::ostream& display(std::ostream& os) const = 0;
	friend std::ostream& operator<<(std::ostream& s, const IMatrixParser& m) { return s; }
};
class IMatrixParserBase: public IMatrixParser
{
	inline static constexpr const char TypeIdentifier[] = "MatrixParserBase";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
public:
	MatrixType operator()() const { return exec(); };
protected:
	IMatrixParserBase(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p):tokenFactory{fT}, elementFactory{fE}, visitorFactory{fB}, path{p} {};
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::shared_ptr<Factory<IElement>> elementFactory;
	std::shared_ptr<Factory<BaseVisitor>> visitorFactory;
	const std::string path;
private:
	virtual MatrixType exec() const = 0;
	virtual std::ostream& display(std::ostream& os) const { return os;};
	friend std::ostream& operator<<(std::ostream& s, const IMatrixParserBase& m) { return s; }
};

class StageParser: public IMatrixParserBase
{
	using Base = IMatrixParserBase;
public:
	StageParser(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): IMatrixParserBase{fT,fE,fB, p} {};
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
