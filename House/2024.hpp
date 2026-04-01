#pragma once

class Invoice_2024
{
	public:
		int Run(const auto& tokenFactory, const auto& elementFactory,const auto& visitorFactory,const auto& path)
		{
			std::cout<<"\nSTART 2024\n"<<std::endl;
			auto b1 = std::chrono::steady_clock::now();
        
			auto account = std::make_shared<AccountCalculator>(tokenFactory,elementFactory,visitorFactory, path);
			auto bM = HtmlBuilder<German>("Middle_T_2003.html");
			
			auto hall2 = std::make_unique<Hall<HallTwo>>(tokenFactory,elementFactory,visitorFactory,path);
			(*hall2)(Year(2024),bM). template To<Quantity<Sum>>();
			auto hallM2 = (*hall2)(Year(2025),bM). template To<Quantity<Sum>>();
//			assert(hallM2[0][0]().Equals(Quantity<Sum>{1350},0.01));
//			assert(hallM2[0][1]().Equals(Quantity<Sum>{990},0.01));
//			assert(hall2->Value(Year(2023),bM).Equals(Quantity<Sum>{2340},0.01));
//
			auto costs = (*account)(Year{2024}, bM)[0].template To<Quantity<Sum>>();;
			assert(costs[0]().Equals(Quantity<Sum>{-334.81},0.01));
			assert(costs[1]().Equals(Quantity<Sum>{-4093.15},0.01));
			assert(costs[2]().Equals(Quantity<Sum>{-2106.86},0.01));
			assert(costs[3]().Equals(Quantity<Sum>{-3081.28},0.01));
			assert(costs[4]().Equals(Quantity<Sum>{-423.01},0.01));
			assert(costs[5]().Equals(Quantity<Sum>{-932.00},0.01));
			assert(account->Value(Year{2024}).Equals(Quantity<Sum>{-10971.11},0.01));
			
			auto propB24 = ExtraCostsCalculator<Bottom>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto propM24 = ExtraCostsCalculator<Middle>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto propT24 = ExtraCostsCalculator<Top>(account, tokenFactory,elementFactory,visitorFactory,path);
			
		    auto bMS = propB24(Year{2024}, bM). template To<Quantity<Sum>>();

			auto aB26 = propB24.AdvancedItems(Year{2024});
			std::cout<<"mB "<<aB26<<std::endl;
			assert(aB26[0][0]()->Data()=="Miete");
			assert(aB26[0][1]()->Data()=="Heiznebenkosten");
			assert(aB26[0][2]()->Data()=="Nebenkosten");
			assert(*aB26[1][0]()==Quantity<Sum>{458});
			assert(*aB26[1][1]()==Quantity<Sum>{135});
			assert(*aB26[1][2]()==Quantity<Sum>{67});
			

			assert(propB24.Result(Year{2024}).Equals(Quantity<Sum>{-800.42},0.01));
			assert(bMS[0][0]().Equals(Quantity<Sum>{-111.60},0.01));
			assert(bMS[0][1]().Equals(Quantity<Sum>{-1515.98},0.01));
			assert(bMS[0][2]().Equals(Quantity<Sum>{-526.72},0.01));
			assert(bMS[0][3]().Equals(Quantity<Sum>{-616.26},0.01));
			assert(bMS[0][4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(bMS[0][5]().Equals(Quantity<Sum>{-289.54},0.01));
			assert(propB24.Value(Year{2024}).Equals(Quantity<Sum>{-3224.42},0.01));
			
			auto mMS = propM24(Year{2024}, bM). template To<Quantity<Sum>>();
			auto aM26 = propM24.AdvancedItems(Year{2025});
			assert(aM26[0][0]()->Data()=="Miete");
			assert(aM26[0][1]()->Data()=="Heiznebenkosten");
			assert(aM26[0][2]()->Data()=="Nebenkosten");
			assert(*aM26[1][0]()==Quantity<Sum>{525});
			assert(*aM26[1][1]()==Quantity<Sum>{0});
			assert(*aM26[1][2]()==Quantity<Sum>{210});
			
			assert(propM24.AdvancePayment().Equals(Quantity<Sum>{2520},0.01));
			assert(propM24.Result(Year{2024}).Equals(Quantity<Sum>{-900.80},0.01));
			assert(mMS[0][0]().Equals(Quantity<Sum>{-167.40},0.01));
			assert(mMS[0][1]().Equals(Quantity<Sum>{-1515.98},0.01));
			assert(mMS[0][2]().Equals(Quantity<Sum>{-526.72},0.01));
			assert(mMS[0][3]().Equals(Quantity<Sum>{-616.26},0.01));
			assert(mMS[0][4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(mMS[0][5]().Equals(Quantity<Sum>{-430.11},0.01));
			assert(propM24.Value(Year{2024}).Equals(Quantity<Sum>{-3420.80},0.01));
			
			auto mTS = propT24(Year{2024}, bM). template To<Quantity<Sum>>();
			costs = propT24.Costs(Year{2025})[0]. template To<QS>();
//			assert(propT24.AdvancePayment().Equals(Quantity<Sum>{16200},0.01));
//			assert(propT24.Result(Year{2024}).Equals(Quantity<Sum>{11874.10},0.01));
			assert(mTS[0][0]().Equals(Quantity<Sum>{-55.80},0.01));
			assert(mTS[0][1]().Equals(Quantity<Sum>{-1061.19},0.01));
			assert(mTS[0][2]().Equals(Quantity<Sum>{-1053.43},0.01));
			assert(mTS[0][3]().Equals(Quantity<Sum>{-1848.77},0.01));
			assert(mTS[0][4]().Equals(Quantity<Sum>{-94.36},0.01));
			assert(mTS[0][5]().Equals(Quantity<Sum>{-212.35},0.01));
			assert(propT24.Value(Year{2024}).Equals(Quantity<Sum>{-4325.90},0.01));

			auto hC = std::make_unique<HallCurrentCalculator>(tokenFactory,elementFactory,visitorFactory,path);
			
			auto hC24 = (*hC)(Year(2024),bM). template To<Quantity<Sum>>();
			assert(hC24[0][0]().Equals(Quantity<Sum>{-202.01},0.01));
			
			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
			
			auto e2 = std::chrono::steady_clock::now();
			std::cout << "2024 Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - b1).count() << "[ms]" << std::endl;

			std::cout<<"\nEND 2024\n"<<std::endl;
		   
			return 0;
		}
};
