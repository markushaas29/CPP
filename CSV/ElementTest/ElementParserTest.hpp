#include <iostream>
#include <regex>
#include <cassert>
#include "../ElementParser.hpp"

class ElementParserTest{
public:
int Run()
{
	ElementParser p;

	p("Empfänger: Markus HaasKto/IBAN: DE05660623660009331409 BLZ/BIC: GENODE61DET Buchungstext: Leana Ref. JM22503421509571/2");
	
	ClientParser c;

	c("Auftraggeber: Ballreich, Klaus Buchungstext: Miete Werkstatt Januar Ref. JF225031C0758672/75032");

	return 0;
}
};
