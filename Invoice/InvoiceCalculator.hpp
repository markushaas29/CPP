#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"
#include "../Functional/Functional.hpp"

#pragma once

template<typename Q>
class IInvoice
{
public:
    using QuantityType = Q;
    using FuncType = Acc<Q>;
    virtual Q Value() const { return value(); };
    virtual Q operator()() { return calc(); };
private:
    friend  std::ostream& operator<<(std::ostream& out, const IInvoice& s) {    return s.display(out);  }
    virtual std::ostream& display(std::ostream& out)    const = 0;
    virtual Q value() const = 0;
    virtual Q calc() const = 0;
};

template<typename T>
class Invoice: public IInvoice<Quantity<Sum>>
{
    using Base = IInvoice<Quantity<Sum>>;
public:
//  Invoice(const Q&& q, const MType&& m = MType(), const std::string& n =""): value{q}, item(m), name{n} {};
private:
    friend  std::ostream& operator<<(std::ostream& out, const Invoice& s)   {   return out<<"Result: "<<s.result;   }
    std::ostream& display(std::ostream& out) const { return out<<(*this); }
    virtual typename Base::QuantityType value() const { return result; }
    virtual typename Base::QuantityType calc() const
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
        
        auto tokenFactory = Build<IToken, WorkToken, VolumeToken, WordToken, SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, ValueToken, QuantityToken, WordToken,IBANIndexToken, BICIndexToken, NameIndexToken, SumIndex
        auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"RentIn
        Matcher smatcher(std::move(stageIndexTokens));        
        
        auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});        
        Matcher imatcher(std::move(csvIndexTokens));        
    
        auto v = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});        
        Matcher matcher(std::move(v));        
    
        auto parsedAccountMatrix = accountMatrix.Match(imatcher).Parse(matcher);        
        
        auto elementFactory = Build<IElement,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Year, Month,Index<int>, Entry,Empty>();        
        auto typeFactory = std::make_shared<TF>(elementFactory);        
        
        auto visitorFactory = std::make_shared<Factory<BaseVisitor>>();
        auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>,AccumulationVisitor<Quantity<Volume>>,ConsumptionVisitor<Quantity<Volume>>>(&(*visitorFactory));        
    
        auto mps = mS.Match(smatcher).Parse(matcher).Cols(2,3,4,5,6,7).template To<Quantity<Scalar>>();
        auto stageQ = mS.Match(smatcher).Parse(matcher);        
        auto payment = stageQ.Cols(8,9,10).template To<Quantity<Sum>>();
        
        using AllStages = std::tuple<Bottom, Middle, Top>;        
        
        const std::string path = "/home/markus/Downloads/CSV_TestFiles_2";        
        
        auto extra_Bottom = ExtraCostItems<Bottom>{tokenFactory,elementFactory,visitorFactory, path};        
        auto extra_Middle = ExtraCostItems<Middle>{tokenFactory,elementFactory,visitorFactory, path};        
        auto extra_Top = ExtraCostItems<Top>{tokenFactory,elementFactory,visitorFactory, path};
        auto extraCostsBottom = YearlyExtraCosts<Bottom>{tokenFactory,elementFactory,visitorFactory, path};        
        auto extraCostsMiddle = YearlyExtraCosts<Middle>{tokenFactory,elementFactory,visitorFactory, path};        
                                                                                                       
        return calcAll<0,AllStages>(mps,tokenFactory,elementFactory,visitorFactory, path);          
    };
    typename Base::QuantityType result;
    std::string name;
};
