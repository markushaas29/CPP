#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixOperations.hpp"
#include "../../CSV/Elements.hpp"

class MatrixCalculatorTest
{
	public:
		int Run()
		{
			std::cout<<"START"<<std::endl;

			using Q = Quantity<Sum>;
			using M3 = Matrix<3>;
			using M2 = Matrix<2>;
			using M1 = Matrix<1>;
			using M3DS = MatrixDescriptor<3,double>;
			using M2DS = MatrixDescriptor<2,double>;
			using M1DS = MatrixDescriptor<1,double>;
			using M1PS = MatrixDescriptor<1,Parameter<int>>;
			using M2PS = MatrixDescriptor<2,Parameter<int>>;
			using MS1 = MatrixDescriptor<1,std::string>;
			using MS2 = MatrixDescriptor<2,std::string>;
			using M1S = Matrix<1,MS1>;
			using M2S = Matrix<2,MS2>;
			using M3D = Matrix<3,M3DS>;
			using M2D = Matrix<2,M2DS>;
			using M2P = Matrix<2,M2PS>;
			using M1D = Matrix<1,M1DS>;
			using QS = Quantity<Scalar,Pure,double>;
		
			M1S m1s{
				{std::string("31.10.1986"),std::string("29.09.1986"),std::string("27.03.1986")}	
			};

			M1S m1S{
				{std::string("4"),std::string( "5"),std::string( "6")}
			};
			m1S.ColSum();
			M1 m13{
				{5,3,1}	
			};
			M1 m1{
				{1,2,3,4,5}	
			};
			
			M2 m35 {
				{1, 2, 3, 4, 5},
				{6, 7, 8, 9,10},
				{11, 12, 13, 14, 15},
			};
			auto m35Am35 = m35+m35;
			auto s1 = m1.ColSum();
			assert(s1()==15);
			assert(m35Am35[0][0].To<int>()==2);
			assert(m35Am35[2][4].To<int>()==30);
			auto m35Sm35 = m35Am35-m35;
			
			assert(m35Sm35[0][0].To<int>()==1);
			assert(m35Sm35[2][4].To<int>()==15);
			
			auto m35Mm35 = m1*m1;
			assert(m35Mm35[0][0].To<int>()==1);
			assert(m35Mm35[2][4].To<int>()==15);
			assert(m35Mm35[4][4].To<int>()==25);
			auto p4 = Parameter<int>(4);
			auto mad = m35 - m35 - m35 + m35;// * p4;
		  	auto mop = ((m35 * 2)- m35 - (m35 + 5) + (m35 - 6) -m35 +m35 );
		  	assert((int)mop[0][0]==-10);
		  	int i00 = mop[0][0];
		  	assert(i00==-10);
			auto i00q = mop[0][0].Get();
			assert(i00q.Value()==-10.0);
			bool is = std::is_same_v<QS, decltype(i00q)>;
			assert(is);
			auto m35m1 = m35 * m1;
			
			M2 m33
			{
				{1, 2, 3},
				{4, 5, 6},
				{7, 8, 9}
			};
			
			M2S m33S
			{
				{"1", "2", "3"},
				{"4", "5", "6"},
				{"7", "8", "9"}
			};
		
			M2 m44 
			{
				{1, 2, 3, 4},
				{6, 7, 8, 9},
				{10, 12, 13, 14},
				{16, 17, 18, 19}
			};
			
			M2P m2p {
				{p4, p4, p4, p4},
				{p4, p4, p4, p4},
				{p4, p4, p4, p4},
				{p4, p4, p4, p4}
			};
			
			auto m33m13 = m33*m13;
			assert(m33m13[0]()==14);
			std::cout<<"Matrix M2 * M1\n"<<m33m13<<"\n";
			std::cout<<"Matrix M2 * M2\n"<<m33*m33<<"\n";
			auto m13m33 = m13*m33;
			assert(m13m33[0]()==24);
			std::cout<<"Matrix M1 * M2\n"<<m13m33*m33m13<<"\n";
			std::cout<<"Matrix M2S\n"<<m33S.ColSum()<<"\n";
			std::cout<<"Matrix M2S\n"<<m33S.ColSum(1)<<"\n";
			assert(m33S.ColSum(1)()==15);
			assert(m33S.ColSum()[1]()==15);
			assert((int)m2p[0][0]==4);
			p4.Value() = 8;
			assert((int)m2p[0][0]==8);

			auto m44p = m44 + m2p;
			assert((int)m44p[0][0]==9);
			p4.Value() = 2;
			assert((int)m44p[0][0]==3);
			
			auto m2pI = m2p + 4;
			assert((int)m2pI[0][0]==6);
			p4.Value() = 6;
			assert((int)m2pI[0][0]==10);
			
			auto m2pIM = m2pI + m44;
			assert((int)m2pIM[0][0]==11);
			p4.Value() = 10;
			assert((int)m2pIM[0][0]==15);
		
			M1 m1b{
				{1,2,3,4}	
			};

			auto M33 = m33 * m33;
			assert(M33[0][0].To<int>()==30);
			assert(M33[1][1].To<int>()==81);
			assert(M33[2][2].To<int>()==150);

			auto M13 = m33 * m13;
			assert(M13[0].To<int>()==14);
			assert(M13[1].To<int>()==41);
			assert(M13[2].To<int>()==68);

			auto a5 = m44 + 5;
			assert((int)a5[0][0]==6);
			assert((int)a5[3][3]==24);
		
			auto s5 = m44 - 5;
			assert((int)s5[0][3]==-1);
			
			auto m5 = m44 * 5;
			assert((int)m5[0][3]==20);
			assert((int)m5[0][1]==10);
			
			auto d2 = m44  / 2;
			assert((int)d2[0][3]==2);
			assert((int)d2[1][2]==4);
			
			auto p = Parameter<int>(2);
			auto ap = m44 + p;
			assert((int)ap[0][0]==3);
			assert((int)ap[0][3]==6);
			p.Value() = 4;
			//assert((int)ap[0][0]==5);
			//assert((int)ap[0][3]==8);

			auto m44m44 = m44 * m44;
			std::cout<<"Matrix m2pI "<<m44m44<<"\n";
			assert(m44m44[0][0].To<int>()==107);
			assert(m44m44[1][0].To<int>()==272);

			auto m44m1 = m44 * m1b;
			assert(m44m1[0].To<int>()==30);
			assert(m44m1[1].To<int>()==80);
			auto m44Dm1 = m44 / m1b;
			
			assert(m44Dm1[0][0].To<int>()==1);
			assert(m44Dm1[1][0].To<double>()==6);
			assert(m44Dm1[2][2].To<double>()==(13/3));

			auto s44 = m44m44.Cols(0,1);
			assert((int)s44[0][0]==107);
			assert((int)m44m44[0][0]==107);
			auto aA = m44.Apply([&](const auto& e1){ return *e1 + 10; });
			auto m1m1 = m1*m1;
			
			assert((int)(m44m1[0])==30);
			
			M1D m1d{
				{1.1,2.2,3.3,4.4}	
			};
			auto m1dq = m1d[0].Get();
			assert(m1dq.Value()==1.1);
			auto m2SA = a5 - m44 + d2 ;
			assert((int)m2SA[0][3]==7);
			
			M2D m2d {
				{1.1, 2.2, 3.3, 4.4},
				{1.1, 2.2, 3.3, 4.4},
				{1.1, 2.2, 3.3, 4.4},
				{1.1, 2.2, 3.3, 5.4}
			};
		
		
			M3 m3 {
				{
					{1,2},
					{3,4}
				},
				{
					{6,7},
					{8,9}
				},
			};
		
			auto m1Am1 = m1 + m1; 
			std::cout<<"ColSum:\n "<<m1Am1<<"\n";
			auto m2C = m44.ColSum(); 
			//assert((int)m2C[0]==14);
			//assert((int)m2C[2]==22);
			auto m2dC = m2d.ColSum();
			assert((double)m2dC[0]==4.4);
			std::cout<<"ColSum:\n "<<m2dC[2]<<"\n";
			assert((double)m2dC[2]()==13.2);
			auto m1C = m1.ColSum(); 
			assert(m1C()==15);
			auto m35C = m35.ColSum(); 
			assert((int)m35C[0]==18);
			assert((int)m35C[1]==21);
			
			auto m2R = m44.RowSum(); 
			std::cout<<"RowSum:\n "<<m2R<<"\n";
			assert((int)m2R[0]==10);
			assert((int)m2R[1]==30);
			
			auto m35R = m35.RowSum(); 
			assert((int)m35R[0]==15);
			assert((int)m35R[1]==40);
			
			auto m1R = m1.RowSum(); 
			assert((int)m1R==15);
			
			auto m2dR = m2d.RowSum();
			assert((double)m2dR[0]==11);
			assert((double)m2dR[3]==12);
			auto s = m2R.Cols(0);
		
			auto m2DC = m2d.ColSum();
	      	assert((double)m2DC[0]==4.4);
	      	assert((double)m2DC[3]==18.6);
	
	      	M1D hmc{
	              {(104.5 * 1.19), (143 * 1.19), (450 *1.19)} 
	      	};
	
	      	M2D hms {
	              {1, 2, 1},
	              {1, 1, 0},
	              {1, 1, 0}
	      	};
	
	      	auto hmsAll = hms.ColSum();
			auto hmRatios = hms / hmsAll;
			std::cout<<"HmsCostRatio"<<hmRatios<<std::endl;
			auto hmsCosts = hmRatios * hmc;
	
			for(auto i = 0; i < hmsCosts.Rows(); ++i)
				std::cout<<"Cost"<<(double)hmsCosts[i]<<std::endl;

			std::cout<<"HmsCotst"<<hmsCosts<<std::endl;

			std::cout<<"END M1 * M1"<<(m1 * m1)<<std::endl;

			auto md = m44.Calc<Diff>();
			assert(m44.Size()==md.Size()+1);
			assert((int)md[0]==-1);
			std::cout<<m1s<<std::endl;

			auto mdat = m1s.To<Date>();	
			auto mdd = mdat.Calc<Diff>();
			auto mq13 = m13.To<Q>();
			auto md13 = mq13.Calc<Diff>();

			std::cout<<(m1s[0].To<Date>()-mdat[1].To<Date>()).Value()<<std::endl;
			std::cout<<mdd<<std::endl;
			std::cout<<mdd[0]<<std::endl;
			std::cout<<mdd[0].Value()<<std::endl;

			auto m1Div = mdd / md13;
//			assert((int)m1Div[0]()==16);
//			assert((int)m1Div[1]()==93);
			
//			std::cout<<"END M1 * M1"<<(mf1[0][0].To<int>() * mf1[1][2].To<int>())<<std::endl;
//			std::cout<<"END M1 * M1"<<(mf1[0][0].Value() * mf1[1][2].Value())<<std::endl;
		//   
			return 0;
		}
};
