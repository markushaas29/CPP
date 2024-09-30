#include <memory>
#include <tuple>
#include <vector>
#include <sstream>
#include "InvoiceCalculatorBase.hpp"
#include "InvoiceCalculators.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"
#include "../Functional/Functional.hpp"

#pragma once

template<typename T>
class Invoice: public CalculatorBase<Quantity<Sum>,Invoice<T>>
{
	using Base = CalculatorBase<Quantity<Sum>,Invoice<T>>;
    using Stage = T;
public:
//  Invoice(const Q&& q, const MType&& m = MType(), const std::string& n =""): value{q}, item(m), name{n} {};
    Invoice(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y,const std::string& p): 
		Base{fE,fB,y}, tokenFactory{fT}, file{HtmlBuilder<German>(std::string(T::Name)+ y.ToString() +".html")}, 
		account{std::make_unique<AccountCalculator>(tokenFactory,Base::elementFactory,Base::visitorFactory, Base::year, p)},
		proportion{std::make_unique<ProportionCalculator<Stage>>(tokenFactory,Base::elementFactory,Base::visitorFactory, Base::year,p)},
		path{p}{ }
	template<size_t N, typename Tup>
	auto calcAll(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path) 
	{
	    stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path);
	    auto costs = calcCosts<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path).Rows(N+1);
	
	    auto extraCosts = YearlyExtraCostsCalculator<std::tuple_element_t<N,Tup>>{tokenFactory,elementFactory,visitorFactory, Base::year,path};
	
	    auto e = extraCosts(file)[0];
	    return costs()[0].template To<Quantity<Sum>>() + extraCosts(file)[0].template As<Quantity<Sum>>();
	}
	auto Costs() { auto b = HtmlBuilder<German>("");return (*account)(b).To<Quantity<Sum>>(); }
	auto Prop() { return (*proportion)(file); }
private:
	std::shared_ptr<Factory<IToken>> tokenFactory;
	HtmlBuilder<German> file;
	std::string path;
    friend  std::ostream& operator<<(std::ostream& out, const Invoice& s)   {   return out<<"Result: "<<s.result;   }
    std::ostream& display(std::ostream& out) const { return out<<(*this); }
    virtual typename Base::MatrixType exec(const HtmlBuilder<German>& f, const Year& y)  {	return typename Base::MatrixType(typename Base::DescriptorType({1}),{std::make_shared<Quantity<Sum>>(value(f))});    };
    virtual typename Base::QuantityType value(const HtmlBuilder<German>& f) 
    {
        using MDS2 = MatrixDescriptor<2,std::string>;
        using MS2 = Matrix<2,MDS2>;                                                                                       
        using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
        using EVF = Factory<BaseVisitor>;                                                     
        auto sNew = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN_Name.csv" };
        auto mS = MatrixReader(sNew).template M<2>();
        
        auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"RentIndexToken"},{"ExtraCostsIndexToken"},{"HeatExtraCostsIndexToken"},{"GarageRentIndexToken"} });
        Matcher smatcher(std::move(stageIndexTokens));        
        
        auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});        
        Matcher imatcher(std::move(csvIndexTokens));        
    
        auto v = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});        
        Matcher matcher(std::move(v));        
        
        auto typeFactory = std::make_shared<TF>(Base::elementFactory);        
        
        auto visitorFactory = std::make_shared<Factory<BaseVisitor>>();
        auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>,AccumulationVisitor<Quantity<Volume>>,ConsumptionVisitor<Quantity<Volume>>>(&(*visitorFactory));        
    
        auto mps = mS.Parse(smatcher, matcher).Cols(2,3,4,5,6,7).template To<Quantity<Scalar>>();
        auto stageQ = mS.Parse(smatcher, matcher);        
		//std::cout<<"Q"<<(stageQ[1][6].template To<Quantity<Scalar>>()) / (stageQ[2][6].template To<Quantity<Scalar>>())<<std::endl;
        auto payment = stageQ.Cols(8,9,10).template To<Quantity<Sum>>();
        
        using AllStages = std::tuple<Bottom, Middle, Top>;        
        
        const std::string path = "/home/markus/Downloads/CSV_TestFiles_2";        

		auto pm = (*proportion)(file);
		std::cout<<"PM: "<<pm<<std::endl;
                                                                                                       
        return calcAll<Stage::Index-1,AllStages>(mps,tokenFactory,Base::elementFactory,visitorFactory, path);          
    };

	template<size_t N, typename Tup>
	auto process(auto& stageMatrix, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p) 
	{
	    if constexpr (std::tuple_size<Tup>()==N)
	        return stageMatrix;
	    else
	    {
	        using Type = std::tuple_element_t<N,Tup>;
	        auto readings = Readings<Type>{fT,fE,fB, Base::year,p};
	        stageMatrix = stageMatrix.Set(readings(file)[0].template As<Quantity<Scalar>>(),Type::Index,((int)stageMatrix.Cols()-1));
	        return process<N+1,Tup>(stageMatrix,fT,fE,fB,p);
	    }
	}
	
	template<size_t N, typename Tup>
	auto calcCosts(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path)
	{
	    stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path);
	    auto sumMatrix = (*account)(file).To<Quantity<Sum>>();
	    auto stagesDiv = (stageMatrix / stageMatrix.ColSum());
	    return stagesDiv * sumMatrix;                                                                                                       
	}
	
	std::unique_ptr<AccountCalculator> account;
	std::unique_ptr<ProportionCalculator<Stage>> proportion;
    typename Base::QuantityType result;
	std::string name;
};
