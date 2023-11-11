#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Matrix.hpp"
#include "MatrixDescriptor.hpp"
#include "MatrixProjectorTest/Test.cpp"
#include "MatrixInitializerTest/Test.cpp"
#include "MatrixCalculatorTest/Test.cpp"
#include "MatrixElementTest/Test.cpp"
#include "MatrixDescriptorTest/Test.cpp"
#include "MatrixSliceAccessTest/Test.cpp"
#include "MatrixQueryTest/Test.cpp"
#include "MatrixAnalyzerTest/Test.cpp"
#include "M3Test/Test.cpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"

using M3 = Matrix<3>;
using M2 = Matrix<2>;
using MS3 = MatrixDescriptor<3,double>;
using MS2 = MatrixDescriptor<2,std::string>;
using ME2D = MatrixDescriptor<2,std::shared_ptr<IElement>>;
using MS1 = MatrixDescriptor<1,std::string>;
using MQ1DS = MatrixDescriptor<1,double>;
using M3D = Matrix<3,MS3>;
using M1S = Matrix<1,MS1>;
using M1Q = Matrix<1,MQ1DS>;
using M2E = Matrix<2,ME2D>;
using M2M = Matrix<2,MS2>;
using M1 = Matrix<1>;
