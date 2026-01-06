class Invoice_2022
{
	public:
		int Run(const auto& tokenFactory, const auto& elementFactory,const auto& visitorFactory,const auto& path)
		{
			std::cout<<"START 2022"<<std::endl;
			auto b1 = std::chrono::steady_clock::now();
			
			auto account = std::make_shared<AccountCalculator>(tokenFactory,elementFactory,visitorFactory, path);
			std::chrono::steady_clock::time_point e1 = std::chrono::steady_clock::now();

			std::cout << "Calculation Time Init :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e1 - b1).count() << "[ms]" << std::endl;
			
			auto bM = HtmlBuilder<German>("Middle_T_2003.html");
			
			auto propB23 = ExtraCostsCalculator<Bottom>(account, tokenFactory,elementFactory,visitorFactory,path);			
			auto bV = propB23.Values(Year{2022}, bM).template To<Quantity<Sum>>();
			assert(bV[0]().Equals(Quantity<Sum>{-107.33},0.01));
			assert(bV[1]().Equals(Quantity<Sum>{-758.60},0.01));
			assert(bV[2]().Equals(Quantity<Sum>{-417.83},0.01));
			assert(bV[3]().Equals(Quantity<Sum>{-42.84},0.01));
			assert(bV[4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(bV[5]().Equals(Quantity<Sum>{-288.47},0.01));

			auto propT23 = ExtraCostsCalculator<Top>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto mT22 = propT23.Values(Year{2022}, bM). template To<Quantity<Sum>>();
			assert(mT22[0]().Equals(Quantity<Sum>{-53.67},0.01));
			assert(mT22[1]().Equals(Quantity<Sum>{-531.02},0.01));
			assert(mT22[2]().Equals(Quantity<Sum>{-835.65},0.01));
			//assert(mT22[3]().Equals(Quantity<Sum>{-270.22},0.01));
			assert(mT22[4]().Equals(Quantity<Sum>{-94.36},0.01));
			assert(mT22[5]().Equals(Quantity<Sum>{-292.31},0.01));
			
			e1 = std::chrono::steady_clock::now();
			std::cout << "Calculation Time Bottom :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e1 - b1).count() << "[ms]" << std::endl;

			std::cout<<"END 2022"<<std::endl;
		   
			return 0;
		}
};
