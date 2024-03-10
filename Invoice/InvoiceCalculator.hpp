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
		Base{fE,fB,y}, tokenFactory{fT}, file{std::make_shared<std::ofstream>(std::string(T::Name)+ y.ToString() +".txt")} {};
	template<size_t N, typename Tup>
	auto calcAll(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path) const 
	{
	    stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path);
	    auto costs = calcCosts<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path).Rows(N+1);
	
	    auto extraCosts = YearlyExtraCostsCalculator<std::tuple_element_t<N,Tup>>{tokenFactory,elementFactory,visitorFactory, Base::year,path};
	
	    auto e = extraCosts()[0];
	    return costs()[0].template To<Quantity<Sum>>() + extraCosts()[0].template As<Quantity<Sum>>();
	}
private:
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::shared_ptr<std::ofstream> file;
    friend  std::ostream& operator<<(std::ostream& out, const Invoice& s)   {   return out<<"Result: "<<s.result;   }
    std::ostream& display(std::ostream& out) const { return out<<(*this); }
    virtual typename Base::MatrixType exec(std::shared_ptr<std::ofstream> f) const  {	*f<<"EDC";return typename Base::MatrixType(typename Base::DescriptorType({1}),{std::make_shared<Quantity<Sum>>(value(f))});    };
    virtual typename Base::QuantityType value(std::shared_ptr<std::ofstream> f) const
    {
        using MDS2 = MatrixDescriptor<2,std::string>;
        using MS2 = Matrix<2,MDS2>;                                                                                       
        using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
        using EVF = Factory<BaseVisitor>;                                                     
        auto u22 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };        
        auto u23 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2023.csv" };          
        auto u24 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2024.csv" };          
        auto sNew = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN_Name.csv" };
        auto m22r = MatrixReader(u22);        
        auto m23r = MatrixReader(u23);        
        auto m24r = MatrixReader(u24);              
        auto mS = MatrixReader(sNew).template M<2>();
        auto m22S = m22r.template M<2>();
        auto m23S = m23r.template M<2>();
        auto m24S = m24r.template M<2>();
        std::vector<MS2> accountFiles{m22S, m23S, m24S};        
        M3 accountMatrix(accountFiles);        
        
        auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"RentIndexToken"},{"ExtraCostsIndexToken"},{"HeatExtraCostsIndexToken"} });
        Matcher smatcher(std::move(stageIndexTokens));        
        
        auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});        
        Matcher imatcher(std::move(csvIndexTokens));        
    
        auto v = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});        
        Matcher matcher(std::move(v));        
    
        auto parsedAccountMatrix = accountMatrix.Match(imatcher).Parse(matcher);        
        
        auto typeFactory = std::make_shared<TF>(Base::elementFactory);        
        
        auto visitorFactory = std::make_shared<Factory<BaseVisitor>>();
        auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>,AccumulationVisitor<Quantity<Volume>>,ConsumptionVisitor<Quantity<Volume>>>(&(*visitorFactory));        
    
        auto mps = mS.Match(smatcher).Parse(matcher).Cols(2,3,4,5,6,7).template To<Quantity<Scalar>>();
        auto stageQ = mS.Match(smatcher).Parse(matcher);        
        auto payment = stageQ.Cols(8,9,10).template To<Quantity<Sum>>();
        
        using AllStages = std::tuple<Bottom, Middle, Top>;        
        
        const std::string path = "/home/markus/Downloads/CSV_TestFiles_2";        

		auto prop = ProportionCalculator<Stage>{tokenFactory,Base::elementFactory,Base::visitorFactory, Year{2023},path};
		prop(file);
                                                                                                       
        return calcAll<Stage::Index-1,AllStages>(mps,tokenFactory,Base::elementFactory,visitorFactory, path);          
    };

	template<size_t N, typename Tup>
	auto process(auto& stageMatrix, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p) const 
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
	auto calcCosts(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path) const
	{
	    auto account = AccountCalculator{tokenFactory,elementFactory,visitorFactory, Base::year, path}; 
	    stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path);
	    
		//assert(account.Value().Equals(Quantity<Sum>{-7977.75},0.02));
	    auto sumMatrix = account(file).To<Quantity<Sum>>();  
	    auto stagesDiv = (stageMatrix / stageMatrix.ColSum());
	    return stagesDiv * sumMatrix;                                                                                                       
	}
	
    typename Base::QuantityType result;
    std::string name;
};
