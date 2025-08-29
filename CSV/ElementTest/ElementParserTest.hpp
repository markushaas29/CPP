#include <iostream>
#include <regex>
#include <cassert>
#include "../ElementParser.hpp"

class ElementParserTest{
public:
int Run()
{
	std::cout<<"\nStartElementParser\n"<<std::endl;
	
	ReceiverParser r;
	auto v = r("Empfänger: Markus HaasKto/IBAN: DE05660623660009331409 BLZ/BIC: GENODE61DET Buchungstext: Leana Ref. JM22503421509571/2");
	assert(r.Is("Empfänger: Markus HaasKto/IBAN: DE05660623660009331409 BLZ/BIC: GENODE61DET Buchungstext: Leana Ref. JM22503421509571/2"));
	assert(v.size()==5);
	v = r("Empf�nger: Markus HaasKto/IBAN: DE05660623660009331409 BLZ/BIC: GENODE61DET  Buchungstext: Leana Ref. JM22503421509571/2");
	
	BookingTextParser b;
	v = b(" Buchungstext: Entgelt Visa-Kreditkarte Zeitraum: 01.01.2025 bis 31.01.2025 Ref. AB2C215N3OLPIN1H/583968");
	assert(b.Is(" Buchungstext: Entgelt Visa-Kreditkarte Zeitraum: 01.01.2025 bis 31.01.2025 Ref. AB2C215N3OLPIN1H/583968"));
	assert(!b.Is("Buchungstext"));
	assert(v.size()==5);
	
	HeaderParser h;
	v = h("Buchungstext");
	assert(!h.Is(" Buchungstext: Entgelt Visa-Kreditkarte Zeitraum: 01.01.2025 bis 31.01.2025 Ref. AB2C215N3OLPIN1H/583968"));
	assert(h.Is("Buchungstext"));
	assert(v.size()==5);
	
	std::cout<<"\nCLIENT\n"<<std::endl;
	ClientParser c;
	v = c("Auftraggeber: Ballreich, Klaus Buchungstext: Miete Werkstatt Januar Ref. JF225031C0758672/75032");
	assert(c.Is("Auftraggeber: Ballreich, Klaus Buchungstext: Miete Werkstatt Januar Ref. JF225031C0758672/75032"));
	assert(!c.Is("-1.000,00"));
	assert(v.size()==5);

	std::unique_ptr<IElementParser> p = std::make_unique<ReceiverParser>();
	(*p)("Empfänger: Markus HaasKto/IBAN: DE05660623660009331409 BLZ/BIC: GENODE61DET Buchungstext: Leana Ref. JM22503421509571/2");
	
	p = std::make_unique<ClientParser>();
	(*p)("Auftraggeber: Ballreich, Klaus Buchungstext: Miete Werkstatt Januar Ref. JF225031C0758672/75032");

	return 0;
}
};
