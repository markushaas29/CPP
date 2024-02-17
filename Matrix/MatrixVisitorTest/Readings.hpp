#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixComposition.hpp"
#include "../M3.hpp"
#include "../MatrixStrategy.hpp"
#include "../../Builder/Builder.hpp"
#include "../../Counter/ICounter.hpp"
#include "../../ObjectFactory/Factory.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../CSV/Matcher.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../Visitor/CollectorVisitor.hpp"

#pragma once

class XBase
{
	inline static constexpr const char TypeIdentifier[] = "Matrix";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
public:
    inline static constexpr size_t Order = 1;
	using ElementType = std::shared_ptr<IElement>;
	using DescriptorType = MatrixDescriptor<1,ElementType>;
	using MatrixType = Matrix<1, DescriptorType>;
	virtual MatrixType operator()() const = 0;
protected:
	XBase(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p):tokenFactory{fT}, elementFactory{fE}, visitorFactory{fB}, path{p} {};
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::shared_ptr<Factory<IElement>> elementFactory;
	std::shared_ptr<Factory<BaseVisitor>> visitorFactory;
	const std::string path;
private:
	friend std::ostream& operator<<(std::ostream& s, const XBase& m) { return s; }
};

template<typename S>
class Readings: public XBase
{
	using Base = XBase;
	using Stage = S;
public:
	Readings(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): XBase{fT,fE,fB, p} {};
	typename Base::MatrixType operator()() const
	{
		auto fbv = std::make_shared<Factory<BaseVisitor>>();
        auto reg3 = Registration<Factory<BaseVisitor>,DifferenceVisitor<Quantity<Energy, KiloHour>>,DifferenceVisitor<Date>, AccumulationVisitor<Quantity<Volume>>>(&(*fbv));

		Builder<ICounter,Counter,TopHotDesc, TopColdDesc, MiddleHotDesc, MiddleColdDesc, BottomHotDesc, BottomColdDesc> b;
		auto cV = b(path, tokenFactory);

		auto els = std::vector<std::shared_ptr<IElement>>{};
		
		std::for_each(cV->begin(), cV->end(), [&](const auto& i)
				{ 
					std::unique_ptr<BaseVisitor> civ = std::make_unique<ConsumptionVisitor<Quantity<Volume, Pure, double>>>();
					civ = i->Accept(std::move(civ));
					auto consV = civ->template As<ConsumptionVisitor<Quantity<Volume>>>();
					els.push_back(consV());	
				});

		auto med = Init(els);
		auto readings = med();

        auto accBV = (*fbv)("Accumulation","");
		accBV = readings.Accept(std::move(accBV));

		int h = S::Index*2;
		int l = (S::Index-1)*2;
		auto accV = accBV->template As<AccumulationVisitor<Quantity<Volume>>>();
		auto d = Quantity<Volume>((*accV(l,h)).To<Quantity<Volume>>());

	 	auto sum = (*accV()).To<Quantity<Volume>>();
        std::vector<typename Base::ElementType> resQ = { std::make_shared<Quantity<Scalar>>(d / (*accV()).To<Quantity<Volume>>()) };
		std::cout<<"RES:"<<(Quantity<Volume>((*accV(4,6)).To<Quantity<Volume>>())/ (*accV()).To<Quantity<Volume>>())<<std::endl;
		std::cout<<"RES:"<<(Quantity<Volume>((*accV(2,4)).To<Quantity<Volume>>())/ (*accV()).To<Quantity<Volume>>())<<std::endl;
		std::cout<<"RES:"<<sum<<(Quantity<Volume>((*accV(0,2)).To<Quantity<Volume>>())/ (*accV()).To<Quantity<Volume>>())<<std::endl;

        return Matrix<Order,DescriptorType>(typename Base::DescriptorType({resQ.size()}),ToDataType(resQ));
	}
private:
};

class Stages: public XBase
{
	using Base = XBase;
public:
	Stages(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): XBase{fT,fE,fB, p} {};
	typename Base::MatrixType operator()() const
	{
//		
//		auto u23 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2023.csv" };
//		auto u24 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2024.csv" };
//		auto rpath = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN.csv" };
//		auto sNew = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN_Name.csv" };
//		auto mrR = MatrixReader(rpath);
//		auto mR = mrR.M<2>().Cols(3,4,5,6,7,8);
//		auto msm = mR.To<Quantity<Scalar>>();
//		auto m22r = MatrixReader(u22);
//		auto m23r = MatrixReader(u23);
//		auto m24r = MatrixReader(u24);
//		auto mS = MatrixReader(sNew).M<2>();
//		auto m22S = m22r.M<2>();
//		auto m23S = m23r.M<2>();
//		auto m24S = m24r.M<2>();
//		auto t = false;
//		std::vector<MS2> accountFiles{m22S, m23S, m24S};
//		M3 accountMatrix(accountFiles);
//		
//		auto tokenFactory = Build<IToken, WorkToken, VolumeToken, WordToken, SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, ValueToken, QuantityToken, WordToken,IBANIndexToken, BICIndexToken, NameIndexToken, SumI
//		auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"Re
//		Matcher smatcher(std::move(stageIndexTokens));
//		auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});
//		Matcher imatcher(std::move(csvIndexTokens));
//		auto v = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});
//		Matcher matcher(std::move(v));
//		
//		auto parsedAccountMatrix = accountMatrix.Match(imatcher).Parse(matcher);
//		
//		auto elementFactory = Build<IElement,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Year, Month,Index<int>, Entry,Empty>();
//		auto typeFactory = std::make_shared<TF>(elementFactory);
//		
//		auto visitorFactory = std::make_shared<Factory<BaseVisitor>>();
//		auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>>(&(*visitorFactory));
//		
//		auto predicateVisitorFactory = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(elementFactory);
//		predicateVisitorFactory->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
//		auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*predicateVisitorFactory));
//		
//		std::vector<FactoryUnit<std::string, std::string>> fv{{"Accumulation"}};
//		for(uint i = 0; i < allFactoryUnits.size(); ++i)
//		        all->Add(MatrixComposite<decltype(parsedAccountMatrix)>::Create(typeFactory,visitorFactory,std::move(allFactoryUnits[i].Name()), allFactoryUnits[i].Units(),fv));
//		auto result = (*all)(parsedAccountMatrix);
//		//std::cout<<"\n-------------------All---------------------\n:\n"<<(*(*all)(parsedAccountMatrix))<<std::endl;
//		auto ms = result->Elements().To<Quantity<Sum>>();
//		auto fms = result->Funcs();
//		std::cout<<"\n-------------------All---------------------\n:\n"<<fms<<std::endl;
//		std::cout<<"\n-------------------All()---------------------\n:\n"<<fms()<<std::endl;
//		std::cout<<"\n-------------------All[0]()---------------------\n:\n"<<fms[0]()<<std::endl;
//		
//		auto mps = mS.Match(smatcher).Parse(matcher).Cols(2,3,4,5,6,7).To<Quantity<Scalar>>();
//		auto stageQ = mS.Match(smatcher).Parse(matcher);
//		auto payment = stageQ.Cols(8,9,10).To<Quantity<Sum>>();
//		std::vector<Quantity<Sum>> extras = {{payment[1][1].To<Quantity<Sum>>()+payment[1][2].To<Quantity<Sum>>()}, {payment[2][1].To<Quantity<Sum>>()+payment[2][2].To<Quantity<Sum>>()}}; 
//		std::for_each(extras.begin(), extras.end(),[&](auto& e) { e = e * Quantity<Scalar>{12}; });
//		std::cout<<"\n-------------------Stages()---------------------\n:\n"<<payment<<extras[0]<<std::endl;
        std::vector<typename Base::ElementType> resQ;// = { std::make_shared<typename Base::ElementType>("") };
        return Matrix<Order,DescriptorType>(typename Base::DescriptorType({resQ.size()}),ToDataType(resQ));
	}
private:
};
