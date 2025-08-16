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

	return 0;
}
};
