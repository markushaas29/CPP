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
		inline static constexpr IBAN iban{"DE19660623660009232702"};
		//~ struct House
		//~ {
		//~ };
		
		struct Private
		{
			inline static constexpr IBAN iban{"DE05660623660009331409"};
		};
	};
	
	struct Comdirect
	{
		inline static constexpr IBAN iban{"DE83200411330694752700"};
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
