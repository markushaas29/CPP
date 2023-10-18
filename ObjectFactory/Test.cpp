#include <iostream>
#include "Factory.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixCategory.hpp"
#include "../Matrix/MatrixMultiCategory.hpp"

int main()
{
	std::cout<<"START"<<std::endl;

	Factory<int> f;
	Factory<int,std::function<int*()>> fi;

	std::string s;
	try {f(s,s); } catch(...) { }

	Factory<IMatrixCategory<std::string>> fm;
	fm.Register("EQ",[](const std::string& s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
	assert(fm.Size()==1);
	fm.Register("C",[](std::string_view s) { return std::make_unique<ContainCat<std::string>>(std::string(s)); });
	assert(fm.Size()==2);

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
	pf->Register("C",[](std::string_view s) { return std::make_unique<ContainCat<std::string>>(std::string(s)); });
	assert(pf->Size()==2);
	
	try {OrCat<std::string>(pf,units); } catch(...) { is = true; };	
	assert(!is);
	is = false;
	
	OrCat<std::string> oc(pf,units);
	assert(oc("2"));
	assert(!oc("3"));
	assert(oc("ABC"));
	assert(!oc("EF"));
	
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
	
	auto pfM =  std::make_shared<Factory<IMatrixCategory<std::string>,MultiCatUnit<std::string>>>();
	pfM->Register("A",[](const MultiCatUnit<std::string>& s) { return std::make_unique<AndCat<std::string>>(s); });

 	std::vector<FactoryUnit<std::string, std::string>> mU49 = { {"EQ", "4"}, {"EQ","9"}};
 	std::vector<FactoryUnit<std::string, std::string>> mU79 = { {"EQ", "7"}, {"EQ","9"}};
	std::cout<<"Start"<<std::endl;
    //MatrixQuery<M2S,std::string>(pfm, mU49);
    auto m49 = m33.M(MatrixRowQuery<M2S,std::string>(pfm, mU49));
	assert(m49.Rows()==1);
	assert(m49[0][0].To<int>()==4);
	assert(m49[0][2].To<int>()==6);

	std::unique_ptr<IMatrixCategory<std::string>> a79 = std::make_unique<AndCat<std::string>>(pfm,mU79);
	auto m79R = MatrixRowQuery<M2S,std::string>(std::move(a79));
	auto m79 = m33.M(m79R);
	assert(m79.Rows()==1);
	assert(m79[0][0].To<int>()==7);
	assert(m79[0][2].To<int>()==9);

	std::cout<<"M49"<<m49<<std::endl;
	std::cout<<"M49"<<m79<<std::endl;
	
	std::cout<<"END"<<std::endl;
   
	return 0;
}
