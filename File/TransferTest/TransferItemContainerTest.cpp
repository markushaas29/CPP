#include <iostream>
#include "../Transfer.hpp"
#include "../Node.hpp"
#include "../Raiffeisenbank.hpp"
#include "../../Common/Configuration.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../String/String_.hpp"
using namespace ShortNames;

int main()
{   
	using CSVSeparator = T::char_<';'> ;    
    std::cout<<"START"<<std::endl;
	
	auto keys = FS::ReadLines("/home/markus/Downloads/CSV_Files/Raiba.keys");
	TI::Instance().UpdateKeyIndices(keys.cbegin(), keys.end());
	
	std::string indicesLine = "Bezeichnung Auftragskonto;IBAN Auftragskonto;BIC Auftragskonto;Bankname Auftragskonto;Buchungstag;Valutadatum;Name Zahlungsbeteiligter;IBAN Zahlungsbeteiligter;BIC (SWIFT-Code) Zahlungsbeteiligter;Buchungstext;Verwendungszweck;Betrag;Waehrung;Saldo nach Buchung;Bemerkung;Kategorie;Steuerrelevant;Glaeubiger ID;Mandatsreferenz";
	std::string indicesLineErr = "Bezeichnung Auftragskonto;IBN Auftragskonto;BI Auftragskonto;Bankname Auftragkonto;Buungstag;Valutatum;Namet Zahlungsbeteihjligter;IBN Zahlungeteiligter;BC (SWIFT-Code) Zahlungsteiligter;Bucungstext;Verwenungszweck;Berag;Waerung;Saldo nach Buchung;Bemerkung;Kategorie;Steuerrelevant;Glaeubiger ID;Mandatsreferenz";
	std::string t1Line =	"Kontokorrent-/Girokonto;DE19660623660009232702;GENODE61DET;RAIFFEISENBANK HARDT-BRUHRAIN;02.01.2023;02.01.2023;SV Gebaeudeversicherung;DE97500500000003200029;HELADEFFXXX;Basislastschrift;SV AgrarPolice V50042043040 1917,62.EUR. EREF: 060033695188 0485614667 MREF: IGS00004749611 CRED: DE9300200000021090 IBAN: DE97500500000003200029 BIC: HELADEFF ABWA: SV SparkassenVersicherung Gebaudeversicherung AG;-1917,62;EUR;-831,09;;Sonstiges;;DE9300200000021090;IGS00004749611";
	std::string t2Line ="Kontokorrent-/Girokonto;DE19660623660009232702;GENODE61DET;RAIFFEISENBANK HARDT-BRUHRAIN;02.01.2023;02.01.2023;Anita Erna Brustat;DE83660623660009262008;GENODE61DET;DA-GUTSCHRIFT;MIETE;660,00;EUR;1086,53;;Sonstiges;;;";
	std::vector<std::string> file {t1Line,t2Line};
	
	auto indexValues = String_::Split<CSVSeparator>(String_::Remove<String_::CR>(indicesLineErr));
	bool result = TI::Instance().Check(indexValues);
	assert(!result);
	
	indexValues = String_::Split<CSVSeparator>(String_::Remove<String_::CR>(indicesLine));
	result = TI::Instance().Check(indexValues);
	assert(result);
	
    return 0;
}
