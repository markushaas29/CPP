#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixProjector.hpp"
#include "../MatrixFilter.hpp"
#include "../MatrixComposition.hpp"
#include "../MatrixElement.hpp"
#include "../M3.hpp"
#include "../../ObjectFactory/Factory.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../CSV/Element.hpp"
#include "../../CSV/Matcher.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../Visitor/CollectorVisitor.hpp"
#include "../../Invoice/Invoice.hpp"
#include "../../TypeCounter/TypeCounter.hpp"

#pragma once

class Invoice_2026
{
	public:
		int Run(const auto& tokenFactory, const auto& elementFactory,const auto& visitorFactory,const auto& path)
		{
			std::cout<<"\nSTART 2026"<<std::endl;
			auto b1 = std::chrono::steady_clock::now();

			auto account = std::make_shared<AccountCalculator>(tokenFactory,elementFactory,visitorFactory, path);
			std::chrono::steady_clock::time_point e1 = std::chrono::steady_clock::now();

			std::cout << "Calculation Time Init :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e1 - b1).count() << "[ms]" << std::endl;
			
			auto bM = HtmlBuilder<German>("Middle_T_2003.html");
			
//			auto hall = std::make_unique<Hall<HallOne>>(tokenFactory,elementFactory,visitorFactory,path);
//			auto hallM = (*hall)(Year(2026),bM). template To<Quantity<Sum>>();
//			assert(hallM[0][0]().Equals(Quantity<Sum>{680.40},0.01));
//			assert(hallM[0][1]().Equals(Quantity<Sum>{2653.2},0.01));
//			assert(hall->Value(Year(2026),bM).Equals(Quantity<Sum>{3333.6},0.01));
//			
//			auto hall2 = std::make_unique<Hall<HallTwo>>(tokenFactory,elementFactory,visitorFactory,path);
//			(*hall2)(Year(2024),bM). template To<Quantity<Sum>>();
//			auto hallM2 = (*hall2)(Year(2026),bM). template To<Quantity<Sum>>();
////			assert(hallM2[0][0]().Equals(Quantity<Sum>{1350},0.01));
////			assert(hallM2[0][1]().Equals(Quantity<Sum>{990},0.01));
////			assert(hall2->Value(Year(2023),bM).Equals(Quantity<Sum>{2340},0.01));
			
//			auto hall3 = std::make_unique<Hall<HallThree>>(tokenFactory,elementFactory,visitorFactory,path);
//			auto hallM3 = (*hall3)(Year(2026),bM). template To<Quantity<Sum>>();
//			std::cout<<"Hall"<<hallM3<<std::endl;
//			std::cout<<"H "<<hall2->Value(Year(2026),bM)<<std::endl;
//			assert(hall3->Value(Year(2026),bM).Equals(Quantity<Sum>{2770.20},0.01));
//			assert(hallM3[0][0]().Equals(Quantity<Sum>{2770.20},0.01));
			
			std::chrono::steady_clock::time_point e2 = std::chrono::steady_clock::now();
			std::cout << "Calculation Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - e1).count() << "[ms]" << std::endl;

			auto propB24 = ExtraCostsCalculator<Bottom>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto propM24 = ExtraCostsCalculator<Middle>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto propT24 = ExtraCostsCalculator<Top>(account, tokenFactory,elementFactory,visitorFactory,path);

			auto mB2026 = propB24(Year{2026}, bM). template To<Quantity<Sum>>();
			std::cout<<"mB "<<mB2026[0][4]<<std::endl;
//			assert(mB2026[0][0]().Equals(Quantity<Sum>{-115.45},0.01));
//			assert(mB2026[0][1]().Equals(Quantity<Sum>{-115.45},0.01));
//	  		assert(mB2026[0][2]().Equals(Quantity<Sum>{-539.76},0.01));
//	  		assert(mB2026[0][3]().Equals(Quantity<Sum>{-629.76},0.01));
//	  		assert(mB2026[0][4]().Equals(Quantity<Sum>{-252.59},0.01));
//	  		//assert(mB2026[0][5]().Equals(Quantity<Sum>{-252.59},0.01));
	
	  		auto mM2026 = propM24(Year{2026}, bM). template To<Quantity<Sum>>();
//	  		assert(mM2026[0][0]().Equals(Quantity<Sum>{-173.18},0.01));
//				assert(mM2026[0][1]().Equals(Quantity<Sum>{-173.18},0.01));
//	  		assert(mM2026[0][2]().Equals(Quantity<Sum>{-539.76},0.01));
//	  		assert(mM2026[0][3]().Equals(Quantity<Sum>{-629.76},0.01));
//	  		assert(mM2026[0][4]().Equals(Quantity<Sum>{-252.59},0.01));
//				assert(mM2026[0][5]().Equals(Quantity<Sum>{-252,59.18},0.01));
	
	  		auto mT2026 = propT24(Year{2026}, bM). template To<Quantity<Sum>>();
	  		std::cout<<"mB "<<mT2026<<std::endl;
//	  		assert(mT2026[0][0]().Equals(Quantity<Sum>{-57.73},0.01));
//				assert(mT2026[0][1]().Equals(Quantity<Sum>{-57.73},0.01));
//	  		assert(mT2026[0][2]().Equals(Quantity<Sum>{-1079.52},0.01));
//	  		assert(mT2026[0][3]().Equals(Quantity<Sum>{-1889.28},0.01));
//	  		assert(mT2026[0][4]().Equals(Quantity<Sum>{-145.05},0.01));
//				assert(mT2026[0][5]().Equals(Quantity<Sum>{-57.73},0.01));
//			
			auto hC = std::make_unique<HallCurrentCalculator>(tokenFactory,elementFactory,visitorFactory,path);

			auto hC25 = (*hC)(Year(2026),bM). template To<Quantity<Sum>>();
//			assert(hC25[0][0]().Equals(Quantity<Sum>{-227.73},0.01));
//			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
		
			auto e2 = std::chrono::steady_clock::now();
			std::cout << "2026 Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - e1).count() << "[ms]" << std::endl;
			
			std::cout<<"\nEND 2026"<<std::endl;
		   
			return 0;
		}
};
