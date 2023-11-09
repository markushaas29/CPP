#include <iostream>
#include <regex>
#include <cassert>
#include "../Element.hpp"
#include "../Elements.hpp"
#include "../Token.hpp"
#include "../Matcher.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../ObjectFactory/Factories.hpp"

class ElementRegexTest
{      
public:
	bool Run()
	{
    	std::cout<<"START RegexTest"<<std::endl;

		auto v = std::make_unique<std::vector<std::unique_ptr<IToken>>>();
		v->push_back(std::make_unique<DateToken>());
		v->push_back(std::make_unique<IBANToken>("DE19660623660009232702"));
		v->push_back(std::make_unique<BICToken>());
		v->push_back(std::make_unique<WordToken>());
		v->push_back(std::make_unique<SumToken>());
		v->push_back(std::make_unique<EntryToken>());
		v->push_back(std::make_unique<ValueToken>());
		v->push_back(std::make_unique<QuantityToken>());

		std::smatch m;
		std::string t = "29.5.1986";
		assert(std::regex_match(t,m,v->at(0)->Pattern()));
		assert(v->at(0)->Match(t));
		t = "29.05.1986";
  		assert(v->at(0)->Match(t));
		t = "02.05.1986";
  		assert(v->at(0)->Match(t));
		t = "2.5.1986";
  		assert(v->at(0)->Match(t));
		t = "12.11.1986";
  		assert(v->at(0)->Match(t));
		t = "12.13.1986";
  		assert(!v->at(0)->Match(t));
		t = "32.11.1986";
  		assert(!v->at(0)->Match(t));

		auto line = std::string("Kontokorrent-/Girokonto;DE19660623660009232702;GENODE61DET;RAIFFEISENBANK HARDT-BRUHRAIN;17.10.2023;17.10.2023;EnBW Energie Bad-Wuertt AG;DE56600501017402051588;SOLADEST600;Basislastschrift;701006843905 Strom Abschlagsforderung DS-Info: enbw.com/datenschutz EREF: B20005583399 701006843905 MREF: V5500000053148766 CRED: DE6900000000084184 IBAN: DE56600501017402051588 BIC: SOLADEST600;-40,00;EUR;704,83;;Sonstiges;;DE6900000000084184;V5500000053148766"); 
		Matcher matcher(std::move(v));
		auto matches = matcher(line);
		for(auto& i : matches)
			std::cout<<"MATCHES: "<<*i<<std::endl;

		auto it = IBANToken("DE19660623660009232702");
		t = "DE19660623660009232702";
  		assert(!it.Match(t));
		auto pe = it.Create(t);

		auto ibans = it(line);
		for(auto& i : ibans)
			std::cout<<"IB "<<*i<<std::endl;


		std::cout<<"PE:"<<*pe<<std::endl;
		
		t = "DE19660623660009234702";
  		assert(it.Match(t));
		
		auto bt = BICToken();
		t = "GENODE6145DET";
  		assert(!it.Match(t));
		t = "SOLADEST600";
  		assert(bt.Match(t));
		t = "GENODE61DET";
  		assert(bt.Match(t));
		
		auto wt = WordToken();
		t = "GENODE6145DET";
  		assert(!wt.Match(t));
		t = "ABC";
  		assert(wt.Match(t));
		t = "abc";
  		assert(wt.Match(t));
		t = "aBc";
  		assert(wt.Match(t));
		
		auto idt = IDToken();
		t = "GENODE6145DET";
  		assert(!idt.Match(t));
		t = "1234";
  		assert(idt.Match(t));
		t = "12.34";
  		assert(!idt.Match(t));
		t = "0";
  		assert(idt.Match(t));
		t = ".0";
  		assert(!idt.Match(t));
		
		auto st = SumToken();
		t = "12.30€";
  		assert(st.Match(t));
		t = "12,30€";
  		assert(st.Match(t));
		t = "0,30€";
  		assert(st.Match(t));
		t = "12.3€";
  		assert(!st.Match(t));
		t = "0123€";
  		assert(!st.Match(t));
		t = "1230€";
  		assert(st.Match(t));
		t = "1231€";
  		assert(st.Match(t));
		t = "12,303€";
  		assert(!st.Match(t));
		
		auto kt = KeyValueToken();
		t = "12:ABC";
  		assert(!kt.Match(t));
		t = "ABC:12";
  		assert(kt.Match(t));
		t = "ABC: 12";
  		assert(kt.Match(t));
		t = "ABC : 12";
  		assert(kt.Match(t));
		t = "ABC :12";
  		assert(kt.Match(t));
		t = "A1BC :12";
  		assert(!kt.Match(t));
		t = "A1BC :12.";
  		assert(!kt.Match(t));
		t = "ABC :12.";
  		assert(!kt.Match(t));
		
//		std::string rs("^[0-9]+$");
//		auto r = std::regex( rs );
//  		assert(std::regex_match(t,r));

    	std::cout<<"\nEnd RegexTest"<<std::endl;

    	return true;
	}
};
