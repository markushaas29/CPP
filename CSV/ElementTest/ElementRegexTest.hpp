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
		v->push_back(std::make_unique<KeyValueToken>());
		v->push_back(std::make_unique<DateIndexToken>());
		v->push_back(std::make_unique<EmptyToken>());

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
		auto matches = matcher.Split(line);
		for(auto& i : matches)
			std::cout<<"MATCHES: "<<*i<<std::endl;

		auto it = IBANToken("DE19660623660009232702");
		t = "DE19660623660009232702";
  		assert(!it.Match(t));
		auto pe = it.Create(t);

		auto ibans = it(line);
		for(auto& i : ibans)
			std::cout<<"IB "<<*i<<std::endl;


		std::cout<<"PE:"<<VolumeToken::Identifier<<std::endl;
		
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
		t = "4,30";
  		assert(st.Match(t));
		t = "4.30";
  		assert(st.Match(t));
		t = "430";
  		assert(!st.Match(t));
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
		
		auto wk = EnergyToken();
		t = "12.30kWs";
  		assert(wk.Match(t));
		t = "12.3kWs";
  		assert(wk.Match(t));
		t = "12,3kWs";
  		assert(wk.Match(t));
		t = "12312,3kWs";
  		assert(wk.Match(t));
		t = "0.3kWs";
  		assert(wk.Match(t));
		t = "0.312321kWs";
  		assert(wk.Match(t));
		t = "0.312321 kWs";
  		assert(!wk.Match(t));
		t = "0.312321Wo";
  		assert(!wk.Match(t));
		t = "0.312321WW";
  		assert(!wk.Match(t));
		t = "0.312321€";
  		assert(!wk.Match(t));
		t = "0.312321Ws";
  		assert(!wk.Match(t));
		t = "0.312321kW";
  		assert(!wk.Match(t));
		
		auto vt = VolumeToken();
		t = "12.30m^3";
  		assert(vt.Match(t));
		t = "12.30m^(3)";
  		assert(vt.Match(t));
		t = "12.30m^(-3)";
  		assert(!vt.Match(t));
		t = "12.30m^4";
  		assert(!vt.Match(t));
		t = "12.30cm^3";
  		assert(!vt.Match(t));
		
		
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
		t = "AB ABC :12 AB";
//  		assert(kt.Match(t));
		t = "A1BC :12";
  		assert(!kt.Match(t));
		t = "A1BC :12.";
  		assert(!kt.Match(t));
		t = "ABC :12.";
  		assert(!kt.Match(t));
		
		auto ni = NameIndexToken();
		t = "Name";
  		assert(ni.Match(t));
		t = "Abname";
  		assert(ni.Match(t));
		t = "ad name";
  		//assert(ni.Match(t));
		
		auto bi = BICIndexToken();
		t = "BIC (SWIFT-Code) Zahlungsbeteiligter";
  		assert(bi.Match(t));
  		
		auto si = SumIndexToken();
		assert(decltype(si)::Identifier=="SumIndexToken");
		
		auto ii = IBANIndexToken();
		assert(decltype(si)::Identifier=="SumIndexToken");
		assert(IBANIndexToken::Identifier=="IBANIndexToken");
		assert(EnergyToken::Identifier=="WorkToken");
		assert(VolumeToken::Identifier=="VolumeToken");
		
		assert(UseIndexToken::Identifier=="VerwendungszweckIndexToken");
		assert(WasteIndexToken::Identifier=="WasteIndexToken");
		
		auto e = EmptyToken();
		t = "";
  		assert(e.Match(t));
		t = "   ";
  		assert(e.Match(t));
		t = "	";
  		assert(e.Match(t));
		t = "A";
  		assert(!e.Match(t));
		t = "A B";
  		assert(!e.Match(t));
		
		auto indexLine = std::string("Bezeichnung Auftragskonto;IBAN Auftragskonto;BIC Auftragskonto;Bankname Auftragskonto;Buchungstag;Valutadatum;Name Zahlungsbeteiligter;IBAN Zahlungsbeteiligter;BIC (SWIFT-Code) Zahlungsbeteiligter;Buchungstext;Verwendungszweck;Betrag;Waehrung;Saldo nach Buchung;Bemerkung;Kategorie;Steuerrelevant;Glaeubiger ID;Mandatsreferenz");
		auto iline20 = "Buchungstag;Valuta;Textschlüssel;Primanota;Zahlungsempfänger;ZahlungsempfängerKto;ZahlungsempfängerIBAN;ZahlungsempfängerBLZ;ZahlungsempfängerBIC;Vorgang/Verwendungszweck;Kundenreferenz;Währung;Umsatz;Soll/Haben";
		auto useGem = "588880002829/Wasser RUCHENSTR. 14 Abschlag/Abwasser RUCHENSTR. 14 Abschlag EREF: 140500005788 MREF: 588880002829001 CRED: DE70ZZZ00000146440 IBAN: DE12660623660000005703 BIC: GENODE61DET";
		auto vi = std::make_unique<std::vector<std::unique_ptr<IToken>>>();
		auto en = EntryToken();
		t = useGem;
  		assert(en.Match(t));
		auto useEnBW = "701033135722 Strom Abschlagsforderung DS-Info: enbw.com/datenschutz EREF: B21254239078 701033135722 MREF: V5500000054391824 CRED: DE6900000000084184 IBAN: DE56600501017402051588 BIC: SOLADEST600";
		t = useEnBW;
  		//assert(en.Match(t));
  		
		assert(!e.Match(t));
		vi->push_back(std::make_unique<DateIndexToken>());
		Matcher imatcher(std::move(vi));
		
		auto imatches = imatcher.Indices(indexLine);
		std::cout<<"MATCHES Index: "<<std::endl;
		for(auto& i : imatches)
			std::cout<<ni<<"MATCHES: "<<*i<<std::endl;


  		assert(imatches[0]->Id()==4);

		Factory<IToken> fmt;
     	fmt.Register("ID",[](const std::string& s) { return std::make_unique<IDToken>(); });
     	fmt.Register("IBAN",[](const std::string& s) { return std::make_unique<IBANToken>(s); });
     	auto idp = fmt("ID","B");
     	std::cout<<"Factory"<<fmt<<*idp<<std::endl;
//		std::string rs("^[0-9]+$");
//		auto r = std::regex( rs );
//  		assert(std::regex_match(t,r));

    	std::cout<<"\nEnd RegexTest"<<std::endl;

    	return true;
	}
};
