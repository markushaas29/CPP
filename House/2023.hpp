#pragma once

class Invoice_2023
{
	public:
		int Run(const auto& tokenFactory, const auto& elementFactory,const auto& visitorFactory,const auto& path)
		{
			std::cout<<"\nSTART 2023\n"<<std::endl;
			auto b1 = std::chrono::steady_clock::now();
		
			auto account = std::make_shared<AccountCalculator>(tokenFactory,elementFactory,visitorFactory, path);
			auto bM = HtmlBuilder<German>("Middle_T_2003.html");
			
			auto hall = std::make_unique<Hall<HallOne>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM = (*hall)(Year(2025),bM). template To<Quantity<Sum>>();
			assert(hallM[0][0]().Equals(Quantity<Sum>{680.40},0.01));
			assert(hallM[0][1]().Equals(Quantity<Sum>{2653.2},0.01));
			assert(hall->Value(Year(2025),bM).Equals(Quantity<Sum>{3333.6},0.01));
			
			auto hall2 = std::make_unique<Hall<HallTwo>>(tokenFactory,elementFactory,visitorFactory,path);
			(*hall2)(Year(2024),bM). template To<Quantity<Sum>>();
			auto hallM2 = (*hall2)(Year(2025),bM). template To<Quantity<Sum>>();
//			assert(hallM2[0][0]().Equals(Quantity<Sum>{1350},0.01));
//			assert(hallM2[0][1]().Equals(Quantity<Sum>{990},0.01));
//			assert(hall2->Value(Year(2023),bM).Equals(Quantity<Sum>{2340},0.01));
			
			auto hall3 = std::make_unique<Hall<HallThree>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM3 = (*hall3)(Year(2025),bM). template To<Quantity<Sum>>();
			assert(hall3->Value(Year(2025),bM).Equals(Quantity<Sum>{2770.20},0.01));
			assert(hallM3[0][0]().Equals(Quantity<Sum>{2770.20},0.01));
			
			auto propB23 = ExtraCostsCalculator<Bottom>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto costs = propB23.Costs(Year{2023})[0]. template To<QS>();
			assert(costs[0]().Equals(Quantity<Sum>{-296.31},0.01));
			assert(costs[1]().Equals(Quantity<Sum>{-3326.63},0.01));
			assert(costs[2]().Equals(Quantity<Sum>{-1951.57},0.01));
			assert(costs[3]().Equals(Quantity<Sum>{-682.74},0.01));
			assert(costs[4]().Equals(Quantity<Sum>{-423.00},0.01));
			assert(costs[5]().Equals(Quantity<Sum>{-1061.32},0.01));
			
			auto bMS = propB23(Year{2023}, bM). template To<Quantity<Sum>>();
			auto propB = propB23.AdvanceItems()[1];
			assert(propB23.AdvancePayment().Equals(Quantity<Sum>{2424},0.01));
			assert(propB23.Result(Year{2023}).Equals(Quantity<Sum>{-46.28},0.01));
			assert(propB()[0]. template As<Quantity<Sum>>()==Quantity<Sum>{458});
	        assert(propB()[1]. template As<Quantity<Sum>>()==Quantity<Sum>{135});
          	assert(propB()[2]. template As<Quantity<Sum>>()==Quantity<Sum>{67});
			assert(bMS[0][0]().Equals(Quantity<Sum>{-98.77},0.01));
			assert(bMS[0][1]().Equals(Quantity<Sum>{-1232.09},0.01));
			assert(bMS[0][2]().Equals(Quantity<Sum>{-487.89},0.01));
			assert(bMS[0][3]().Equals(Quantity<Sum>{-136.55},0.01));
			assert(bMS[0][4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(bMS[0][5]().Equals(Quantity<Sum>{-350.66},0.01));
			assert(propB23.Value(Year{2023}).Equals(Quantity<Sum>{-2470.28},0.01));

			auto propM23 = ExtraCostsCalculator<Middle>(account, tokenFactory,elementFactory,visitorFactory,path);
			
			auto mMS = propM23(Year{2023}, bM). template To<Quantity<Sum>>();
			auto propM = propM23.AdvanceItems()[1];
			assert(propM23.AdvancePayment().Equals(Quantity<Sum>{2520},0.01));
			//assert(propM23.Result(Year{2023}).Equals(Quantity<Sum>{-158.42},0.01));
			assert(propM()[0]. template As<Quantity<Sum>>()==Quantity<Sum>{525});
	        assert(propM()[1]. template As<Quantity<Sum>>()==Quantity<Sum>{0});
          	assert(propM()[2]. template As<Quantity<Sum>>()==Quantity<Sum>{210});
			assert(mMS[0][0]().Equals(Quantity<Sum>{-148.16},0.01));
			assert(mMS[0][1]().Equals(Quantity<Sum>{-1232.09},0.01));
			assert(mMS[0][2]().Equals(Quantity<Sum>{-487.89},0.01));
			assert(mMS[0][3]().Equals(Quantity<Sum>{-136.55},0.01));
			assert(mMS[0][4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(mMS[0][5]().Equals(Quantity<Sum>{-462.19},0.01));
			assert(propM23.Value(Year{2023}).Equals(Quantity<Sum>{-2631.20},0.01));

			auto propT23 = ExtraCostsCalculator<Top>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto mTS = propT23(Year{2023}, bM). template To<Quantity<Sum>>();
			auto propT = propT23.AdvanceItems()[1];
			assert(propT23.AdvancePayment().Equals(Quantity<Sum>{16200},0.01));
			assert(propT23.Result(Year{2023}).Equals(Quantity<Sum>{13559.91},0.01));
			assert(propT()[0]. template As<Quantity<Sum>>()==Quantity<Sum>{1});
	        assert(propT()[1]. template As<Quantity<Sum>>()==Quantity<Sum>{1000});
          	assert(propT()[2]. template As<Quantity<Sum>>()==Quantity<Sum>{350});
			assert(mTS[0][0]().Equals(Quantity<Sum>{-49.38},0.01));
			assert(mTS[0][1]().Equals(Quantity<Sum>{-862.46},0.01));
			assert(mTS[0][2]().Equals(Quantity<Sum>{-975.78},0.01));
			assert(mTS[0][3]().Equals(Quantity<Sum>{-409.64},0.01));
			assert(mTS[0][4]().Equals(Quantity<Sum>{-94.36},0.01));
			assert(mTS[0][5]().Equals(Quantity<Sum>{-248.47},0.01));
			assert(propT23.Value(Year{2023}).Equals(Quantity<Sum>{-2640.09},0.01));
			
			auto hC = std::make_unique<HallCurrentCalculator>(tokenFactory,elementFactory,visitorFactory,path);

			auto hC23 = (*hC)(Year(2023),bM). template To<Quantity<Sum>>();
			assert(hC->Value(Year{2023}).Equals(Quantity<Sum>{-124.31},0.01));
			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();

			auto e2 = std::chrono::steady_clock::now();
			std::cout << "2023 Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - b1).count() << "[ms]" << std::endl;

			std::cout<<"\nEND 2023\n"<<std::endl;
		   
			return 0;
		}
};
