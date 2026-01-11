#pragma once

class Invoice_2025
{
	public:
		int Run(const auto& tokenFactory, const auto& elementFactory,const auto& visitorFactory,const auto& path)
		{
			std::cout<<"\nSTART 2025"<<std::endl;
			auto b1 = std::chrono::steady_clock::now();

			auto account = std::make_shared<AccountCalculator>(tokenFactory,elementFactory,visitorFactory, path);
			std::chrono::steady_clock::time_point e1 = std::chrono::steady_clock::now();

			std::cout << "Calculation Time Init :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e1 - b1).count() << "[ms]" << std::endl;
			
			auto bM = HtmlBuilder<German>("Middle_T_2003.html");
			
			auto hall = std::make_unique<Hall<HallOne>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM = (*hall)(Year(2025),bM). template To<Quantity<Sum>>();
			assert(hallM[0][0]().Equals(Quantity<Sum>{680.40},0.01));
			assert(hallM[0][1]().Equals(Quantity<Sum>{2653.2},0.01));
			assert(hall->Value(Year(2025),bM).Equals(Quantity<Sum>{3333.6},0.01));
			
			auto hall2 = std::make_unique<Hall<HallTwo>>(tokenFactory,elementFactory,visitorFactory,path);
			(*hall2)(Year(2025),bM). template To<Quantity<Sum>>();
			auto hallM2 = (*hall2)(Year(2025),bM). template To<Quantity<Sum>>();
//			assert(hallM2[0][0]().Equals(Quantity<Sum>{1350},0.01));
//			assert(hallM2[0][1]().Equals(Quantity<Sum>{990},0.01));
//			assert(hall2->Value(Year(2023),bM).Equals(Quantity<Sum>{2340},0.01));
			
			auto hall3 = std::make_unique<Hall<HallThree>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM3 = (*hall3)(Year(2025),bM). template To<Quantity<Sum>>();
			std::cout<<"Hall"<<hallM3<<std::endl;
			std::cout<<"H "<<hall2->Value(Year(2025),bM)<<std::endl;
			assert(hall3->Value(Year(2025),bM).Equals(Quantity<Sum>{2770.20},0.01));
			assert(hallM3[0][0]().Equals(Quantity<Sum>{2770.20},0.01));
			
			std::chrono::steady_clock::time_point e2 = std::chrono::steady_clock::now();
			std::cout << "Calculation Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - e1).count() << "[ms]" << std::endl;

			auto calculatorB = ExtraCostsCalculator<Bottom>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto calculatorM = ExtraCostsCalculator<Middle>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto calculatorT = ExtraCostsCalculator<Top>(account, tokenFactory,elementFactory,visitorFactory,path);

			auto mB2025 = calculatorB(Year{2025}, bM). template To<Quantity<Sum>>();
			std::cout<<"mB "<<mB2025[0][1]<<std::endl;
			assert(mB2025[0][0]().Equals(Quantity<Sum>{-115.45},0.01));
			assert(mB2025[0][1]().Equals(Quantity<Sum>{-1579.03},0.01));
			assert(mB2025[0][2]().Equals(Quantity<Sum>{-539.76},0.01));
			assert(mB2025[0][3]().Equals(Quantity<Sum>{-629.76},0.01));
			assert(mB2025[0][4]().Equals(Quantity<Sum>{-252.59},0.01));
//			assert(mB2025[0][5]().Equals(Quantity<Sum>{-252.59},0.01));
//			assert(calculatorB.Value(Year{2025}).Equals(Quantity<Sum>{-32.42},0.01));
		
			auto mM2025 = calculatorM(Year{2025}, bM). template To<Quantity<Sum>>();
			assert(mM2025[0][0]().Equals(Quantity<Sum>{-173.18},0.01));
			assert(mM2025[0][1]().Equals(Quantity<Sum>{-1579.03},0.01));
			assert(mM2025[0][2]().Equals(Quantity<Sum>{-539.76},0.01));
			assert(mM2025[0][3]().Equals(Quantity<Sum>{-629.76},0.01));
			assert(mM2025[0][4]().Equals(Quantity<Sum>{-252.59},0.01));
//			assert(mM2025[0][5]().Equals(Quantity<Sum>{-252,59.18},0.01));
//			assert(calculatorM.Value(Year{2025}).Equals(Quantity<Sum>{-32.42},0.01));
//
			auto mT2025 = calculatorT(Year{2025}, bM). template To<Quantity<Sum>>();
			std::cout<<"mB "<<mT2025<<std::endl;
			assert(mT2025[0][0]().Equals(Quantity<Sum>{-57.73},0.01));
			assert(mT2025[0][1]().Equals(Quantity<Sum>{-1105.32},0.01));
			assert(mT2025[0][2]().Equals(Quantity<Sum>{-1079.52},0.01));
			assert(mT2025[0][3]().Equals(Quantity<Sum>{-1889.28},0.01));
			assert(mT2025[0][4]().Equals(Quantity<Sum>{-145.05},0.01));
//			assert(mT2025[0][5]().Equals(Quantity<Sum>{-57.73},0.01));
//			assert(calculatorT.Value(Year{2025}).Equals(Quantity<Sum>{-32.42},0.01));
			
			auto hC = std::make_unique<HallCurrentCalculator>(tokenFactory,elementFactory,visitorFactory,path);
			
			auto hC23 = (*hC)(Year(2023),bM). template To<Quantity<Sum>>();
			assert(hC->Value(Year{2023}).Equals(Quantity<Sum>{-124.31},0.01));
			
			auto hC24 = (*hC)(Year(2024),bM). template To<Quantity<Sum>>();
			assert(hC24[0][0]().Equals(Quantity<Sum>{-202.01},0.01));
			
			auto hC25 = (*hC)(Year(2025),bM). template To<Quantity<Sum>>();
			assert(hC25[0][0]().Equals(Quantity<Sum>{-227.73},0.01));
			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
		
			e2 = std::chrono::steady_clock::now();
			std::cout << "2025 Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - e1).count() << "[ms]" << std::endl;
			
			std::cout<<"\nEND 2025"<<std::endl;
		   
			return 0;
		}
};
