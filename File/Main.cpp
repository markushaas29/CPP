#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include "Account.hpp"
#include "Repository.hpp"
#include "FileSystem.hpp"

using namespace FS;
//----------------------------------------------------------------------------PRODUCT----------------------------------------------------------
// int main()
// {
// 	
// 	std::vector<std::string> dirs= 
// 	{
// 		"/home/markus/Dokumente/cpp/Matrix",
// 		"/home/markus/Dokumente/cpp/File",
// 		"/home/markus/Dokumente/cpp/Visitor",
// 		"/home/markus/Dokumente/cpp/Unit",
// 		"/home/markus/Dokumente/cpp/Wrapper",
// 		"/home/markus/Dokumente/cpp/Traits",
// 		"/home/markus/Dokumente/cpp/SFINAE",
// 		"/home/markus/Dokumente/cpp/Quantity",
// 		"/home/markus/Dokumente/cpp/Logger",
// 		"/home/markus/Dokumente/cpp/Fraction",
// 		"/home/markus/Dokumente/cpp/CSV",
// 		"/home/markus/Dokumente/cpp/COW",
// 		"/home/markus/Dokumente/cpp/AbstractFactory",
// 		"/home/markus/Dokumente/cpp/Typelist",
// 		"/home/markus/Dokumente/cpp/String",
// 	};
// 	
// 	std::string to ="/media/markus/8591-1355/CPP/";
// // 	std::string to ="/home/markus/Downloads/";
// 	
// 	for(auto p : dirs)
// 	{
// 		std::string from = p;
// 		Backup::Repository::Backup(from,to);
// 	}
// 		
//     return 0;
// };

//----------------------------------------------------------------------------TEST----------------------------------------------------------


int main()
{
	std::ofstream outputfile ("//home//markus//Downloads//Output.txt");
	auto keyFileN = std::string("//home//markus//Downloads//Keys.txt");
// 	std::ofstream keyFile ("//home//markus//Downloads//Keys.txt");
	std::string fileName = "Unit.h";
	std::string custom = "RaibaKonten2021_1.csv";
	std::string comdirect = "Umsaetze_1026947527.csv";
	std::string raiba = "Umsaetze_DE19660623660009232702.csv";
	
	std::string csv = "/home/markus/Downloads/CSV_Files";
    CSV::Repository::Map(csv);

	auto c = CSV::Repository::Read(comdirect);	
	
	auto outFile = std::ofstream("out.txt");
	auto inFile = std::ofstream("//home//markus//Downloads//in.txt");
    CSV::Repository::Attach();
    CSV::Repository::ParseAll();
    CSV::Repository::Display(outFile);
	
	std::cout<<"\n TEST Transfers"<<std::endl;
	std::cout<<"\n TEST Transfers"<<std::endl;
	
	std::cout<<"\n-------------------------------------------------------- Transfers OUT --------------------------------------------------------------"<<std::endl;

	auto out = Transfers<Bank::Raiba<0>>();
	auto com1 = out();
	com1.Display(outputfile);
// 	com1.DisplayKeys(keyFile);
	
	
	std::cout<<"\n-------------------------------------------------------- Transfers IN --------------------------------------------------------------"<<std::endl;
	auto in = Transfers<Bank::Raiba<0>, Bank::In>();
	auto com2 = in();
	com2.Display(outputfile);

	std::cout<<"\n TEST GET Gemeindekasse Dettenheim"<<std::endl;
	std::cout<<"\n-------------------------------------------------------- Transfers OUT --------------------------------------------------------------"<<std::endl;
	auto g = Get<Bank::Raiba<0>, Bank::Out>();
	auto gr = g(Key("Gemeindekasse Dettenheim"));
	gr.Display(std::cout);
	auto water = gr.GetCause();
	Logger::Log(water.begin(), water.end());
	auto s = Quantity<Sum>(0); 
	for(auto w : water)
		s = s + w.GetQuantity();
	
	Logger::Log()<<s<<std::endl;
	
	std::cout<<"\n-------------------------------------------------------- Transfers IN --------------------------------------------------------------"<<std::endl;
	auto raibaOut = Transfers<Bank::Raiba<0>, Bank::Out>();
	auto rbo = raibaOut();
	auto keys = FileSystem::ReadLines(keyFileN);
	rbo.Display(inFile,keys);
	
// 	for(auto i : com)
// 		std::cout<<*i<<Std::endl;
	
// 	auto ins = com::InTransfers()
// 	std::cout<<ins
	
// 	FS::Comdirect<0>::Transactions.Display(std::cout);
	
    return 0;
};
