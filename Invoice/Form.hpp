#include <memory>
#include <tuple>
#include <vector>
#include <sstream>
#include <chrono>
#include "InvoiceCalculatorBase.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"
#include "../Functional/Functional.hpp"

#pragma once

template<typename T>
class Form
{
    using Stage = T;
public:
    Form(const Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>& a, const Year& y,const std::string& p): 
		address(a),
		builder{HtmlBuilder<German>(std::string()+ y.ToString() +".html")}, 
		path{p}{ }
private:
	Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>> address;
    Quantity<Sum> sum;
	std::string name;
	std::string path;
	HtmlBuilder<German> builder;
    friend  std::ostream& operator<<(std::ostream& out, const Form& s)   {   return out<<"Result: "<<s.address;   }
    std::ostream& display(std::ostream& out) const { return out<<(*this); }
    auto exec(const HtmlBuilder<German>& f, const Year& y)  
	{
		 return Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>(MatrixDescriptor<2,std::shared_ptr<IElement>>({1,1}),{ std::make_shared<Quantity<Scalar>>(2) });
	}
	//{	return typename Base::MatrixType(typename Base::DescriptorType({1,1}), {std::make_shared<Quantity<Sum>>(9)} ); };
//    virtual typename Base::QuantityType value(const HtmlBuilder<German>& f) 
//    {
//		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//        using MDS2 = MatrixDescriptor<2,std::string>;
//        using MS2 = Matrix<2,MDS2>;                                                                                       
//        using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
//        using EVF = Factory<BaseVisitor>;                                                     
//        auto sNew = std::string{ "/home/markus/Downloads/CSV_Testbuilders_2/SN_Name.csv" };
//        auto mS = MatrixReader(sNew).template M<2>();
//        
//        auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"RentIndexToken"},{"ExtraCostsIndexToken"},{"HeatExtraCostsIndexToken"},{"GarageRentIndexToken"} });
//        Matcher smatcher(std::move(stageIndexTokens));        
//        
//        auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});        
//        Matcher imatcher(std::move(csvIndexTokens));        
//    
//        auto v = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});        
//        Matcher matcher(std::move(v));        
//        
//        auto typeFactory = std::make_shared<TF>(Base::elementFactory);        
//        
//        auto visitorFactory = std::make_shared<Factory<BaseVisitor>>();
//        auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>,AccumulationVisitor<Quantity<Volume>>,ConsumptionVisitor<Quantity<Volume>>>(&(*visitorFactory));        
//    
//        auto mps = mS.Parse(smatcher, matcher).Cols(2,3,4,5,6,7).template To<Quantity<Scalar>>();
//        auto stageQ = mS.Parse(smatcher, matcher);        
//        auto payment = stageQ.Cols(8,9,10).template To<Quantity<Sum>>();
//        
//        using AllStages = std::tuple<Bottom, Middle, Top>;        
//        
//        const std::string path = "/home/markus/Downloads/CSV_Testbuilders_2";        
//
//		auto pm = (*proportion)(builder,Base::year);
//		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//
//		std::cout << "Calculation Time "<<Stage::Index<<" :"<< std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
//                                                                                                       
//        return calcAll<Stage::Index-1,AllStages>(mps,tokenFactory,Base::elementFactory,visitorFactory, path);          
//    };
//
//	template<size_t N, typename Tup>
//	auto process(auto& stageMatrix, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p) 
//	{
//	    if constexpr (std::tuple_size<Tup>()==N)
//	        return stageMatrix;
//	    else
//	    {
//	        using Type = std::tuple_element_t<N,Tup>;
//	        auto readings = Readings<Type>{fT,fE,fB, Base::year,p};
//	        stageMatrix = stageMatrix.Set(readings(builder)[0].template As<Quantity<Scalar>>(),Type::Index,((int)stageMatrix.Cols()-1));
//	        return process<N+1,Tup>(stageMatrix,fT,fE,fB,p);
//	    }
//	}
//	
//	template<size_t N, typename Tup>
//	auto calcCosts(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path)
//	{
//	    stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path);
//	    auto sumMatrix = AccountCalculator::Instance(tokenFactory,elementFactory,visitorFactory, Base::year, path)(builder, Base::year).template To<Quantity<Sum>>();
//	    auto stagesDiv = (stageMatrix / stageMatrix.ColSum());
//	    return stagesDiv * sumMatrix;                                                                                                       
//	}
//	
};
