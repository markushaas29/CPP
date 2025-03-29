#include <iostream>
#include "Factory.hpp"
#include "FactoryUnitTest.cpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/IMatrixQuery.hpp"
#include "../Matrix/MatrixQuery.hpp"
#include "../Matrix/MatrixCategory.hpp"
#include "../Matrix/MatrixMultiCategory.hpp"

class FactoryTest
{
	public:
int Run()
{
	std::cout<<"START"<<std::endl;

	Factory<int> f;
	Factory<int,std::function<int*()>> fi;

	std::string s;
	try {f(s,s); } catch(...) { }

	Factory<IMatrixCategory<std::string>> fm;
	fm.Register("EQ",[](const std::string& s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
	assert(fm.Size()==1);
	fm.Register("C",[](std::string_view s) { return std::make_unique<HasCat<std::string>>(std::string(s)); });
	assert(fm.Size()==2);
	fm.Register("EQ",[](const std::string& s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
	assert(fm.Size()==2);
	fm.Register("EQ",[](const std::string& s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
	assert(fm.Size()==2);
	fm.Register("C",[](std::string_view s) { return std::make_unique<HasCat<std::string>>(std::string(s)); });
	assert(fm.Size()==2);
	fm.Register("CI",[](std::string_view s) { return std::make_unique<HasCat<std::string>>(std::string(s)); });
	assert(fm.Size()==3);

	auto eqp = fm["EQ"];
	auto eq2 = eqp("2");
	assert((*eq2)("2"));
	
	eq2 = fm("EQ","2");
	assert((*eq2)("2"));
	
	auto cb = fm["C"]("B");
	assert((*cb)("ABC"));
	
	cb = fm("C","B");
	assert((*cb)("ABC"));
	
	bool is = false;
	try {fm(s,s); } catch(...) { is = true; }
	assert(is);
	is = false;

	std::vector<FactoryUnit<std::string, std::string>> units = { {"EQ", "2"}, {"C", "B"}, {"C", "A"}, {"C", "C"}};
	auto uc = fm(units);
	assert((*(uc->at(0)))("2"));
	assert(!(*(uc->at(0)))("4"));
	assert((*(uc->at(1)))("ABC"));
	assert((*(uc->at(2)))("ABC"));
	assert((*(uc->at(3)))("ABC"));
	assert(!(*(uc->at(3)))("AB"));

	auto pf = std::make_shared<Factory<IMatrixCategory<std::string>>>();
	pf->Register("EQ",[](const std::string& s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
	pf->Register("C",[](std::string_view s) { return std::make_unique<HasCat<std::string>>(std::string(s)); });
	assert(pf->Size()==2);
	
	//try {OrCat<std::string>(pf,units); } catch(...) { is = true; };	
	assert(!is);
	is = false;
	
	using M2 = Matrix<2>;
	using MS2 = MatrixDescriptor<2,std::string>;
	using M2S = Matrix<2,MS2>;

    M2S m33 {
            {"1", "2" ,"3"},
            {"4", "5", "6"},
            {"7", "8", "9"},
        };


	auto pfm = std::make_shared<Factory<IMatrixCategory<std::string>>>();
	pfm->Register("EQ",[](const std::string& s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
	
	auto pfs =  std::make_shared<FactoryStack<IMatrixCategory<std::string>, Factory<IMatrixCategory<std::string>>>>(pfm);
	pfs->Register("A",[](std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<std::string>>>> s) { return std::make_unique<AndCat<std::string>>(std::move(s)); });
	pfs->Register("O",[](std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<std::string>>>> s) { return std::make_unique<OrCat<std::string>>(std::move(s)); });

 	std::vector<FactoryUnit<std::string, std::string>> mU49 = { {"EQ", "4"}, {"EQ","9"}};
 	std::vector<FactoryUnit<std::string, std::string>> mU79 = { {"EQ", "7"}, {"EQ","9"}};
 	std::vector<FactoryUnit<std::string, std::string>> mU39 = { {"EQ", "3"}, {"EQ","9"}};
 	std::vector<FactoryUnit<std::string, std::string>> mU89 = { {"EQ", "8"}, {"EQ","9"}};
 	FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> mUO49 = { "O", {{"EQ", "4"}, {"EQ","9"}}};
 	FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> mUA79 = { "A", {{"EQ", "7"}, {"EQ","9"}}};
 	FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> mUO1012 = { "O", {{"EQ", "10"},{ "EQ", "12"}}};
	
	auto po49 = ((*pfs)("O", mU49));
	assert((*po49)("4"));
	assert((*po49)("9"));
	assert(!(*po49)("12d"));
	
	auto pa49 = ((*pfs)("A", mU49));
	assert(!(*pa49)("4"));
	assert(!(*pa49)("9"));
    assert((*pa49->To<IMatrixStateCategory<std::string>>())());    
	auto pa_79 = dynamic_cast<IMatrixStateCategory<std::string>*>(pa49.release());
    assert((*pa_79)());    
	assert(pa_79->Reset());
	
	std::unique_ptr<IMatrixCategory<std::string>> or79 = (*pfs)("O", mU79);
	assert((*or79)("7"));
	assert((*or79)("9"));
	assert(!(*or79)("12"));
	
	std::unique_ptr<IMatrixCategory<std::string>> and79 = (*pfs)("A", mU79);
	assert(!(*and79)("7"));
	assert(!(*and79)("9"));
	auto p79 = dynamic_cast<IMatrixStateCategory<std::string>*>(and79.release());
	assert((*p79)());
	assert(p79->Reset());
	
	std::unique_ptr<IMatrixCategory<std::string>> pa79 = (*pfs)("A", mU79);
	assert(!(*pa79)("12"));
	assert(!(*pa79)("9"));
	auto pad_79 = dynamic_cast<IMatrixStateCategory<std::string>*>(pa79.release());
	//assert(!(*pad_79)("7"));
	assert(!(*pad_79)());
	assert((*pad_79).Reset());

	
	Factory<IToken> fmt;
	fmt.Register("ID",[](const std::string& s) { return std::make_unique<IDToken>(); });
	fmt.Register("IBAN",[](const std::string& s) { return std::make_unique<IBANToken>(s); });
	auto idp = fmt("ID","B");
	idp = fmt("IBAN","ABC");
	//assert((*cb)("ABC"));
	
	std::cout<<fmt<<*idp<<std::endl;
	FactoryUnitTest fu;
	fu.Run();
	std::cout<<"END factory"<<std::endl;
   
	return 0;
}
};
