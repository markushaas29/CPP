namespace FS
{
	struct INPUT;
	struct FS;
	struct KEYS;
	struct CPP;
	struct HPP;
	struct H;
	struct CTRV;
	struct CSV;
	struct XLSX;
	struct XLS;
	struct JPG;
	struct TXT;
	struct ODS;
	struct ZIP;
	struct DOCX;
	struct PDF;
}

namespace Bank
{
	struct Direction;
	template<typename> struct Raiffeisenbank;
}

#pragma once

struct Configuration
{
	struct Repository
	{
		inline static constexpr const char* SourcePath = "/home/markus/Downloads/CSV_Files";
		using FileTypes = Typelist<FS::INPUT,FS::KEYS,FS::CPP, FS::HPP, FS::CTRV,FS::CSV>::Type;
	};
	
	struct Account
	{
		using TransferType = std::tuple<IBAN,BIC,Name,DateTimes::Date, Quantity<Sum>, Bank::Direction, Entry>;
	};
	
	struct Raiffeisenbank
	{
		inline static constexpr BIC bic{"GENODE61DET"};

		struct House
		{
			inline static constexpr IBAN Iban{"DE19660623660009232702"};
			inline static constexpr BIC Bic = bic;
		};
		
		struct Private
		{
			inline static constexpr IBAN Iban{"DE05660623660009331409"};
			inline static constexpr BIC Bic{"GENODE61DET"};
		};
	};
	
	struct AncilliaryRentalCosts
	{
		using AccountType = Bank::Raiffeisenbank<Raiffeisenbank::House>;
	};
	
	struct Comdirect
	{
		inline static constexpr IBAN Iban{"DE83200411330694752700"};
		inline static constexpr BIC Bic{"COBADEHD001"};
	};
	
	struct Backup
	{
		using FileTypes = Typelist<FS::XLSX,FS::XLS,FS::JPG,FS::TXT,FS::ODS,FS::ZIP,FS::DOCX,FS::HPP,FS::H,FS::CSV,FS::CPP,FS::PDF>::Type;
	};
};

//~ template<typename Derived>
//~ struct ConfigurationBase
//~ {
	//~ using Type = Derived;
//~ };

//~ template<class T>
//~ struct Configuration: ConfigurationBase<Configuration<T>>
//~ {
	
//~ };

//~ template<typename S> class AncilliaryRentalCostsContainer;
//~ template<typename ConfigT> class Stage;

//~ template<typename T>
//~ struct Configuration<AncilliaryRentalCostsContainer<T>>: ConfigurationBase<Configuration<AncilliaryRentalCostsContainer<T>>>
//~ {
	//~ using Stage = T;
//~ };
