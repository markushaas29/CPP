#pragma once

class Invoice_2025
{
	public:
		int Run(const auto& tokenFactory, const auto& elementFactory,const auto& visitorFactory,const auto& path)
		{
			std::cout<<"\nSTART 2025\n"<<std::endl;
			auto b1 = std::chrono::steady_clock::now();

			auto account = std::make_shared<AccountCalculator>(tokenFactory,elementFactory,visitorFactory, path);
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

			auto costs = (*account)(Year{2025}, bM)[0].template To<Quantity<Sum>>();;
			std::cout<<"Costs "<<costs<<std::endl;
			assert(costs[0]().Equals(Quantity<Sum>{-346.35},0.01));
			assert(costs[1]().Equals(Quantity<Sum>{-4263.38},0.01));
			assert(costs[2]().Equals(Quantity<Sum>{-2159.03},0.01));
			assert(costs[3]().Equals(Quantity<Sum>{-3148.80},0.01));
			assert(costs[4]().Equals(Quantity<Sum>{-650.24},0.01));
			assert(costs[5]().Equals(Quantity<Sum>{-1010.17},0.01));

			auto hall3 = std::make_unique<Hall<HallThree>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM3 = (*hall3)(Year(2025),bM). template To<Quantity<Sum>>();
			std::cout<<"H "<<hall2->Value(Year(2025),bM)<<std::endl;
			assert(hall3->Value(Year(2025),bM).Equals(Quantity<Sum>{2770.20},0.01));
			assert(hallM3[0][0]().Equals(Quantity<Sum>{2770.20},0.01));

			auto calculatorB = ExtraCostsCalculator<Bottom>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto calculatorM = ExtraCostsCalculator<Middle>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto calculatorT = ExtraCostsCalculator<Top>(account, tokenFactory,elementFactory,visitorFactory,path);

			auto mB2025 = calculatorB(Year{2025}, bM). template To<Quantity<Sum>>();
			std::cout<<"mB "<<calculatorB.Value(Year{2025})<<std::endl;
			assert(mB2025[0][0]().Equals(Quantity<Sum>{-115.45},0.01));
			assert(mB2025[0][1]().Equals(Quantity<Sum>{-1579.03},0.01));
			assert(mB2025[0][2]().Equals(Quantity<Sum>{-539.76},0.01));
			assert(mB2025[0][3]().Equals(Quantity<Sum>{-629.76},0.01));
			assert(mB2025[0][4]().Equals(Quantity<Sum>{-252.59},0.01));
			assert(mB2025[0][5]().Equals(Quantity<Sum>{-328.47},0.01));
			assert(calculatorB.Value(Year{2025}).Equals(Quantity<Sum>{-3445.06},0.01));
		
			auto mM2025 = calculatorM(Year{2025}, bM). template To<Quantity<Sum>>();
			assert(mM2025[0][0]().Equals(Quantity<Sum>{-173.18},0.01));
			assert(mM2025[0][1]().Equals(Quantity<Sum>{-1579.03},0.01));
			assert(mM2025[0][2]().Equals(Quantity<Sum>{-539.76},0.01));
			assert(mM2025[0][3]().Equals(Quantity<Sum>{-629.76},0.01));
			assert(mM2025[0][4]().Equals(Quantity<Sum>{-252.59},0.01));
			assert(mM2025[0][5]().Equals(Quantity<Sum>{-424.31},0.01));
			assert(calculatorM.Value(Year{2025}).Equals(Quantity<Sum>{-3598.63},0.01));

			auto mT2025 = calculatorT(Year{2025}, bM). template To<Quantity<Sum>>();
			std::cout<<"mB "<<mT2025<<std::endl;
			assert(mT2025[0][0]().Equals(Quantity<Sum>{-57.73},0.01));
			assert(mT2025[0][1]().Equals(Quantity<Sum>{-1105.32},0.01));
			assert(mT2025[0][2]().Equals(Quantity<Sum>{-1079.52},0.01));
			assert(mT2025[0][3]().Equals(Quantity<Sum>{-1889.28},0.01));
			assert(mT2025[0][4]().Equals(Quantity<Sum>{-145.05},0.01));
			assert(mT2025[0][5]().Equals(Quantity<Sum>{-257.39},0.01));
			assert(calculatorT.Value(Year{2025}).Equals(Quantity<Sum>{-4534.29},0.01));
			
			auto hC = std::make_unique<HallCurrentCalculator>(tokenFactory,elementFactory,visitorFactory,path);
			
			auto hC25 = (*hC)(Year(2025),bM). template To<Quantity<Sum>>();
			assert(hC25[0][0]().Equals(Quantity<Sum>{-227.73},0.01));
			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
		
			auto e2 = std::chrono::steady_clock::now();
			std::cout << "2025 Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - b1).count() << "[ms]" << std::endl;
			
			std::cout<<"\nEND 2025"<<std::endl;
		   
			return 0;
		}
};
