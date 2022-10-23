#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include "Raiba.hpp"
#include "Repository.hpp"
#include "FileSystem.hpp"
#include "../Home/House.hpp"
#include "../Common/DateTimes.hpp"

using namespace FS;

//----------------------------------------------------------------------------TEST----------------------------------------------------------


int main()
{
	std::locale::global( std::locale( "" ) );
	std::ofstream outFile2 ("//home//markus//Downloads//Out.txt");
// 	std::ofstream inFile ("//home//markus//Downloads//In.txt");
	auto keyFileN = std::string("//home//markus//Downloads//Keys.txt");
	std::ofstream keyFile ("//home//markus//Downloads//Keys.txt");
	std::string fileName = "Unit.h";
	std::string custom = "RaibaKonten2021_1.csv";
	std::string comdirect = "Umsaetze_1026947527.csv";
	std::string raiba = "Umsaetze_DE19660623660009232702.csv";

	//~ House<HouseConfiguration>::Instance();	
	std::string csv = "/home/markus/Downloads/CSV_Files";
    CSV::Repository::Map(csv);
	CSV::Repository::Register();
    CSV::Repository::ParseAll();

	//~ auto c = CSV::Repository::Read(csv);	
	
	auto outFile = std::ofstream("out.txt");
	auto inFile = std::ofstream("//home//markus//Downloads//in.txt");
//     CSV::Repository::Register();
//     CSV::Repository::ParseAll();
//     CSV::Repository::Display(outFile);
// 	

	//~ std::cout<<"\n TEST ID"<<Bank::Raiba<0>::Identifier<<std::endl;
	//~ CSV::Get<Bank::Raiba<0>>();
	std::cout<<"\n TEST Transfers"<<std::endl;
	
	std::cout<<"\n-------------------------------------------------------- Transfers OUT --------------------------------------------------------------"<<std::endl;

	//~ auto out = Transfers<Bank::Raiba<0>>();
	//~ auto com1 = out();
	//~ com1.Display(outFile);
	//~ com1.DisplayKeys(keyFile);
	
	
	//~ std::cout<<"\n-------------------------------------------------------- Transfers IN --------------------------------------------------------------"<<std::endl;
	//~ auto in = Transfers<Bank::Raiba<0>, Bank::In>();
	//~ auto com2 = in();
	//~ com2.Display(inFile);

	//~ std::cout<<"\n TEST GET Gemeindekasse Dettenheim"<<std::endl;
	//~ std::cout<<"\n-------------------------------------------------------- Transfers OUT --------------------------------------------------------------"<<std::endl;
	//~ auto g = Get<Bank::Raiba<0>, Bank::Out>();
	//~ auto gr = g(Key("Gemeindekasse Dettenheim"));
	//~ gr.Display(std::cout);
	//~ auto water = gr.GetCause();
	//~ auto t = gr.Transactions().Begin();
	//~ Logger::Log()<<"T: "<<*(t)<<std::endl;
//~ // 	Logger::Log()<<*(gr.Transactions().Begin())<<std::endl;
	//~ auto s = Quantity<Sum>(0); 
	//~ for(auto w : water)
		//~ s = s + w->GetQuantity();
	
	//~ Logger::Log()<<s<<std::endl;
	
	//~ std::cout<<"\n-------------------------------------------------------- Transfers IN --------------------------------------------------------------"<<std::endl;
	//~ auto raibaOut = Transfers<Bank::Raiba<0>, Bank::Out>();
	//~ auto rbo = raibaOut();
	//~ auto keys = FileSystem::ReadLines(keyFileN);
	//~ rbo.Display(inFile,keys);
	
	//~ auto tr = gr.Transactions();
//~ // 	auto trIt = tr[5];
	//~ auto y2020 = std::make_shared<DateTimes::Year>(2020);
//~ // 	auto trIt2 = trIt[5];
//~ // 	auto trIt3 = trIt2[keyFileN];
	//~ auto tM = tr[DateTimes::May];
	//~ auto tY = tr[y2020];
	//~ auto tD = tr[DateTimes::Date(15,5,2020)];
	//~ auto tD2 = tr[std::make_shared<DateTimes::Date>(15,5,2020)];
	
	//~ Logger::Log()<<"CONTAINER Begin"<<std::endl;
//~ // 	Logger::Log()<<*trIt.Begin()<<std::endl;
//~ // 	Logger::Log()<<*trIt2.Begin()<<std::endl;
	//~ Logger::Log()<<(tM->Size())<<std::endl;
	//~ Logger::Log()<<"Month May\n"<<std::endl;
	//~ tM->Display();
	//~ Logger::Log()<<"Year 2020\n"<<std::endl;
	//~ tY->Display();
	//~ Logger::Log()<<"Date\n"<<std::endl;
	//~ tD->Display();
	//~ Logger::Log()<<"Date shared\n"<<std::endl;
	//~ tD2->Display();
//~ // 	for(auto i = tM->Begin(); tM->Begin() != tM->End(); ++i)
//~ // 		Logger::Log()<<*i<<std::endl;
	
	//~ Logger::Log()<<"CONTAINER Size"<<std::endl;
//~ // 	Logger::Log()<<trIt3.Size()<<std::endl;
//~ // 	Logger::Log()<<trIt2.Size()<<std::endl;
	
    return 0;
};
