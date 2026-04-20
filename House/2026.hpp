#pragma once

class Invoice_2026
{
	public:
		int Run(const auto& tokenFactory, const auto& elementFactory,const auto& visitorFactory,const auto& path)
		{
			std::cout<<"\nSTART 2026\n"<<std::endl;
			auto b1 = std::chrono::steady_clock::now();
			auto y = Year{2026};

			auto account = std::make_shared<AccountCalculator>(tokenFactory,elementFactory,visitorFactory, path);
			auto bM = HtmlBuilder<German>("Middle_T_2003.html");
			auto aq = account->GetYear(y,bM);
			std::cout<<"\nAQ 2026\n"<<aq<<std::endl;
			
			auto hall = std::make_unique<Hall<HallOne>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM = (*hall)(Year(2026),bM). template To<Quantity<Sum>>();
			assert(hallM[0][0]().Equals(Quantity<Sum>{680.40},0.01));
			assert(hallM[0][1]().Equals(Quantity<Sum>{2653.2},0.01));
			assert(hall->Value(Year(2026),bM).Equals(Quantity<Sum>{3333.6},0.01));
			
			auto hall2 = std::make_unique<Hall<HallTwo>>(tokenFactory,elementFactory,visitorFactory,path);
			(*hall2)(Year(2024),bM). template To<Quantity<Sum>>();
			auto hallM2 = (*hall2)(Year(2026),bM). template To<Quantity<Sum>>();
//			assert(hallM2[0][0]().Equals(Quantity<Sum>{1350},0.01));
//			assert(hallM2[0][1]().Equals(Quantity<Sum>{990},0.01));
//			assert(hall2->Value(Year(2023),bM).Equals(Quantity<Sum>{2340},0.01));
			
//			auto hall3 = std::make_unique<Hall<HallThree>>(tokenFactory,elementFactory,visitorFactory,path);
//			auto hallM3 = (*hall3)(Year(2026),bM). template To<Quantity<Sum>>();
//			std::cout<<"Hall"<<hallM3<<std::endl;
//			std::cout<<"H "<<hall2->Value(Year(2026),bM)<<std::endl;
//			assert(hall3->Value(Year(2026),bM).Equals(Quantity<Sum>{2770.20},0.01));
//			assert(hallM3[0][0]().Equals(Quantity<Sum>{2770.20},0.01));

			auto calculatorB = ExtraCostsCalculator<Bottom>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto calculatorM = ExtraCostsCalculator<Middle>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto calculatorT = ExtraCostsCalculator<Top>(account, tokenFactory,elementFactory,visitorFactory,path);

			auto aB26 = calculatorB.AdvancedItems(Year{2026});
			assert(aB26[0][0]()->Data()=="Miete");
			assert(aB26[0][1]()->Data()=="Heiznebenkosten");
			assert(aB26[0][2]()->Data()=="Nebenkosten");
			assert(*aB26[1][0]()==Quantity<Sum>{500});
			assert(*aB26[1][1]()==Quantity<Sum>{135});
			assert(*aB26[1][2]()==Quantity<Sum>{67});
			auto mB2026 = calculatorB(Year{2026}, bM). template To<Quantity<Sum>>();
			auto costs = calculatorB.Costs(Year{2026}, bM);//. template To<Quantity<Sum>>();
	  		std::cout<<"Costs \n"<<costs<<std::endl;
//			assert(mB2026[0][0]().Equals(Quantity<Sum>{-115.45},0.01));
//			assert(mB2026[0][1]().Equals(Quantity<Sum>{-115.45},0.01));
//	  		assert(mB2026[0][2]().Equals(Quantity<Sum>{-539.76},0.01));
//	  		assert(mB2026[0][3]().Equals(Quantity<Sum>{-629.76},0.01));
//	  		assert(mB2026[0][4]().Equals(Quantity<Sum>{-252.59},0.01));
//	  		//assert(mB2026[0][5]().Equals(Quantity<Sum>{-252.59},0.01));
	
			auto aM26 = calculatorM.AdvancedItems(Year{2026});
			assert(aM26[0][0]()->Data()=="Miete");
			assert(aM26[0][1]()->Data()=="Heiznebenkosten");
			assert(aM26[0][2]()->Data()=="Nebenkosten");
			assert(*aM26[1][0]()==Quantity<Sum>{600});
			assert(*aM26[1][1]()==Quantity<Sum>{0});
			assert(*aM26[1][2]()==Quantity<Sum>{310});
	  		auto mM2026 = calculatorM(Year{2026}, bM). template To<Quantity<Sum>>();
			auto advanced = calculatorM.AdvancedItems(Year{2026})[1];
//	  		assert(mM2026[0][0]().Equals(Quantity<Sum>{-173.18},0.01));
//				assert(mM2026[0][1]().Equals(Quantity<Sum>{-173.18},0.01));
//	  		assert(mM2026[0][2]().Equals(Quantity<Sum>{-539.76},0.01));
//	  		assert(mM2026[0][3]().Equals(Quantity<Sum>{-629.76},0.01));
//	  		assert(mM2026[0][4]().Equals(Quantity<Sum>{-252.59},0.01));
//				assert(mM2026[0][5]().Equals(Quantity<Sum>{-252,59.18},0.01));
	
	  		auto mT2026 = calculatorT(Year{2026}, bM). template To<Quantity<Sum>>();
//	  		assert(mT2026[0][0]().Equals(Quantity<Sum>{-57.73},0.01));
//				assert(mT2026[0][1]().Equals(Quantity<Sum>{-57.73},0.01));
//	  		assert(mT2026[0][2]().Equals(Quantity<Sum>{-1079.52},0.01));
//	  		assert(mT2026[0][3]().Equals(Quantity<Sum>{-1889.28},0.01));
//	  		assert(mT2026[0][4]().Equals(Quantity<Sum>{-145.05},0.01));
//				assert(mT2026[0][5]().Equals(Quantity<Sum>{-57.73},0.01));
//			
//			auto hC = std::make_unique<HallCurrentCalculator>(tokenFactory,elementFactory,visitorFactory,path);
//			
//			auto hC23 = (*hC)(Year(2023),bM). template To<Quantity<Sum>>();
//			assert(hC->Value(Year{2023}).Equals(Quantity<Sum>{-124.31},0.01));
//			
//			auto hC24 = (*hC)(Year(2024),bM). template To<Quantity<Sum>>();
//			assert(hC24[0][0]().Equals(Quantity<Sum>{-202.01},0.01));
//			
//			auto hC25 = (*hC)(Year(2026),bM). template To<Quantity<Sum>>();
//			assert(hC25[0][0]().Equals(Quantity<Sum>{-227.73},0.01));
//			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
		
			auto e2 = std::chrono::steady_clock::now();
			std::cout << "2026 Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - b1).count() << "[ms]" << std::endl;
			
			std::cout<<"\nEND 2026"<<std::endl;
		   
			return 0;
		}
};
