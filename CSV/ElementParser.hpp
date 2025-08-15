#include "../String/Literal.hpp"

#pragma once

class ElementParser
{
	inline static constexpr const char TypeIdentifier[] = "Element";
    inline static constexpr Literal LiteralType{TypeIdentifier};
public:
	auto operator()(const std::string& s)
	{
		std::vector<std::string> v;
		std::size_t found1 = s.find("Buchungstext");
	  	if (found1!=std::string::npos)
			v.push_back(std::string(s.begin()+found1,s.end()));
		std::size_t found2 = s.find("IBAN");
	  	if (found2!=std::string::npos)
			v.push_back(std::string(s.begin()+found2,s.begin()+found1));
		
//		if(s.starts_with("Auftraggeber"))
//			newVec.push_back("A");
//		if(s.starts_with("Empf"))
//		{
//			std::size_t found = s.find("IBAN");
//	  		if (found!=std::string::npos)
//				newVec.push_back(std::string(s.begin()+found,s.begin()+found+4));
//			else	
//				newVec.push_back("B");
//		}
//		if(s.starts_with("Buchungstext"))
//			newVec.push_back("C");
//		if(s.starts_with(" Buchungstext"))
//		newVec.push_back("D");
		for(auto t : v)
			std::cout<<t<<std::endl;
	}
private:
	friend std::ostream& operator<<(std::ostream& out, const ElementParser& e) {	return out<<e;}
};

