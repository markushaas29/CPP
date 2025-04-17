#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "Matrix.hpp"
#include "MatrixReader.hpp"
#include "MatrixDescriptor.hpp"
#include "MatrixComposition.hpp"
#include "M3.hpp"
#include "../Builder/Builder.hpp"
#include "../Visitor/ElementVisitor.hpp"
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
	static auto& Instance(auto... t)
    {
        static auto i = AccountParser(t...);
        return i;
    };
private:
	AccountParser(std::shared_ptr<Factory<IToken>> fT, const std::string& p): IMatrixParserBase{fT, p} {};
	const std::string fileName = "SN_Name.csv";
	M3<std::string> matrix() const
	{
        std::vector<std::string> paths{"//U_2022.csv", "//U_2023.csv","//U_2024.csv","//U_2025.csv"};
        std::vector<Matrix<2, MatrixDescriptor<2, std::string>>> accountFiles;
		for(auto s : paths)
		{
        	auto r = MatrixReader(path + s);
			accountFiles.push_back(r.M<2>());
		}

        return M3(accountFiles);
	}
	typename Base::MatrixType exec(bool h = false) const
	{
        auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});
        auto elementIndexTokens = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});
        
		return matrix().Parse(Matcher(std::move(csvIndexTokens)), Matcher(std::move(elementIndexTokens)));
	}
};

class ComdirectParser: public IMatrixParserBase<3>
{
	using Base = IMatrixParserBase;
public:
	static auto& Instance(auto... t)
    {
        static auto i = ComdirectParser(t...);
        return i;
    };
	static auto Elements(const std::string& sp)
	{
		auto s = std::string(sp);
		std::regex rgx("(\\s+)");
		std::sregex_token_iterator iter(s.begin(),s.end(),rgx,-1), end;
		std::vector<std::string> result;
		std::for_each(iter,end, [&result](const auto& s) { result.push_back(s);});
		std::vector<std::unique_ptr<IElement>> res;
		
		std::vector<std::string> headers;
		std::vector<std::string> entries;
		std::string temp = "";
		std::for_each(std::begin(result),std::end(result), [&](const auto& s) 
				{ 
					if(*(s.cend()-1)==':')
					{
						headers.push_back(std::string(s.cbegin(),s.cend()-1));
						if(temp!="")
							entries.push_back(temp);
						temp = "";
					}
					else
						temp += (temp=="" ? s : " " + s);
				});
		
		if(temp!="")
			entries.push_back(temp);

		std::vector<std::vector<std::string>> v = {headers,entries};
		auto m = Init(v)();
		auto tokenFactory = TokenFactoryCreator()();
        auto csvIndexTokens = (*tokenFactory)({{"VorgangIndexToken"},{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"TextIndexToken"}});
        auto elementIndexTokens = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"},{"BICToken"},{"NameIndexToken"},{"TextIndexToken"}});
        
		std::unique_ptr<BaseVisitor> ve = std::make_unique<ElementCollector<Date,Text,Name,IBAN,BIC>>();

		auto matrix = m().ParseByMatch(Matcher(std::move(csvIndexTokens)), true);
		std::cout<<"UNIQUE: \n"<<matrix<<std::endl;
	 	ve = matrix.Accept(std::move(ve));

		return ve->template Cast<ElementCollector<Date,Text,Name,IBAN,BIC>>()->Elements();
	}
private:
	ComdirectParser(std::shared_ptr<Factory<IToken>> fT, const std::string& p): IMatrixParserBase{fT, p} {};
	M3<std::string> matrix() const
	{
        std::vector<std::string> paths{"//Comdirect.csv"};
        std::vector<Matrix<2, MatrixDescriptor<2, std::string>>> accountFiles;
		for(auto s : paths)
		{
        	auto r = MatrixReader(path + s);
			accountFiles.push_back(r.M<2>());
		}

        return M3(accountFiles);
	}
	decltype(auto) matrix2() const
	{
        std::vector<std::string> paths{"//Comdirect.csv"};
        std::vector<Matrix<2, MatrixDescriptor<2, std::string>>> accountFiles;
		for(auto s : paths)
		{
        	auto r = MatrixReader(path + s);
        	return r.M<2>();
		}

	}
	typename Base::MatrixType exec(bool h = false) const
	{
        auto csvIndexTokens = (*tokenFactory)({{"VorgangIndexToken"},{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});
        auto elementIndexTokens = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});
        
		std::unique_ptr<BaseVisitor> v = std::make_unique<ElementCollector<Date>>();

		auto m = matrix().Parse(Matcher(std::move(csvIndexTokens)), Matcher(std::move(elementIndexTokens)));
	 	v = m.Collect(std::move(v));

		auto V = v->template Cast<ElementCollector<Date>>();
		auto vec = V->Elements();
		std::vector<std::shared_ptr<IElement>> uniques;	
		for(auto e : V->Elements())
			std::cout<<"VIS: "<<*e<<std::endl;

		auto same_i = [](auto const& v1, auto const& v2) { return v1->Data() == v2->Data(); };
		vec.erase(unique(vec.begin(), vec.end(), same_i), vec.end());

			std::cout<<"UNIQUE: \n"<<std::endl;
		for(auto e : vec)
			std::cout<<"VIS: "<<*e<<std::endl;

		return m;
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
		auto stageIndexTokens = (*tokenFactory)({{"PrenameIndexToken"},{"NameIndexToken"},{"StreetIndexToken"},{"StreetnumberIndexToken"},{"TownIndexToken"},{"PostcodeIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"RentIndexToken"},{"ExtraCostsIndexToken"},{"HeatExtraCostsIndexToken"}, {"GarageRentIndexToken"},{"SumPerAreaIndexToken"},{"LengthIndexToken"},{"WidthIndexToken"} });
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
		auto stageIndexTokens = (*tokenFactory)({{"PrenameIndexToken"},{"NameIndexToken"},{"StreetIndexToken"},{"StreetnumberIndexToken"},{"TownIndexToken"},{"PostcodeIndexToken"},{"RentIndexToken"},{"AreaIndexToken"},{"SumPerAreaIndexToken"},{"LengthIndexToken"},{"WidthIndexToken"} });
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
