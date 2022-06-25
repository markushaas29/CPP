#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "MatrixExpressionTemplates.hpp"
#include "MatrixProxy.hpp"
#include "Generator.hpp"
#include <cassert>

#ifndef MATRIX_H
#define MATRIX_H

//--------------------------------FORMAT_NON_ZERO------------------------------------------------

template<class Matrix>
struct FORMAT_NON_ZERO
{
	using Type = T::TrueType;
};

//--------------------------------Array------------------------------------------------

template<class Array> class ArrayFormat; 

template<class Generator>
class Array 
{
public:
	using Type = Array<Generator>;
	using Proxy = MatrixProxy<Type>;
	using Config = Generator::Config;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	using Container = Config::Container;
	using Row = Config::Row;
	friend class ArrayFormat<Type>;
protected:
	IndexType row, col;
	Container elements;
	Row rows;

public:
	Array(const IndexType row, const IndexType col): row{row}, col{col} 
	{ 
		assert(row > 0);
		assert(col > 0);
		
		this->elements = Container(row);
		for(IndexType i = 0;i < Rows() ; ++i)
		{
			this->elements.at(i) = Row(Cols());
		}
	}
	
	Proxy operator[](size_t r) const { return Proxy{*this,r}; }
	
	auto& operator()(IndexType r, IndexType c) const
	{ 
		checkBounds(r, c);
		return elements.at(r).at(c); 
	}
	
	template<class A>
	Array(const Array<A>& a){ 	}
	
	template<class A>
	Array& operator=(const Array<A>& a)	{	return this;	}
	
	~Array(){	}
	
	constexpr IndexType Rows() const { return row; }
	constexpr IndexType Cols() const { return col; }
	
	void Set(const IndexType& i, const IndexType& j, const ElementType& v)
	{
		checkBounds(i, j);
		elements.at(i).at(j) = v; 
	}	
	
	void InitElements(const ElementType& v)
	{
		for(IndexType i = 0;i < Rows() ; ++i)
			for(IndexType j = 0; j < Cols() ; ++j)
				Set(i,j,v);	
	}
	
private:
	void checkBounds(const IndexType& i, const IndexType& j) const
	{
		assert(i < Rows());
		assert(j < Cols());
	}
	

};

//--------------------------------Format------------------------------------------------

template<class Array>
class ArrayFormat 
{
	
public:
	using Config = Array::Config;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	using MatrixType = Config::MatrixType;
private:
	Array elements_;
	
protected:
	void checkBounds(const IndexType& i, const IndexType& j) const
	{
		assert(i < rows());
		assert(j < cols());
	}
	
	bool nonZero(const IndexType& i, const IndexType& j) const
	{
		return FORMAT_NON_ZERO<MatrixType>::Type::Value;
	}
	
	ArrayFormat(const IndexType& rows, const IndexType& cols): elements_(rows, cols) { }
	
	const IndexType& rows() const { return elements_.rows(); }
	const IndexType& cols() const { return elements_.cols(); }
	
	void Set(const IndexType& i, const IndexType& j, const ElementType& v)
	{
		checkBounds(i, j);
		if(nonZero(i,j)) elements_.Set(i,j,v);
		else assert(v == ElementType(0));
	}
		
	void InitElements(const ElementType& v){elements_.InitElements(v);}
};

//--------------------------------BoundsChecker------------------------------------------------

template<class OptMatrix>
class BoundsChecker: public OptMatrix
{
public:
	using Type = BoundsChecker<OptMatrix>;
	using Proxy = MatrixProxy<Type>;
	using Config = OptMatrix::Config;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	
	BoundsChecker(const IndexType& rows, const IndexType& cols): OptMatrix(rows, cols) { }
	
	void Set(const IndexType& i, const IndexType& j, const ElementType& v)
	{
		checkBounds(i, j);
		OptMatrix::Set(i,j,v);
	}
	
	ElementType Get(const IndexType& i = 0, const IndexType& j = 0) const 
	{
		checkBounds(i, j);
		return (*this)(i,j);
	}
	
	//~ Proxy operator[](size_t r) const { return Proxy{*this,r}; }
	
	//~ ElementType& operator()(size_t r, size_t c) const
	//~ { 
		//~ checkBounds(r, c);
		//~ return (*this)(r,c); 
	//~ }
	
protected:
	void checkBounds(const IndexType& i, const IndexType& j) const
	{
		if(i < 0 || i >= OptMatrix::Rows() || j < 0 || j >= OptMatrix::Cols())
			throw "Indices out of bounds";
	}
};

template<class Generator>
class DenseCCommaInitializer
{
public:
	using Config = Generator::Config;
	using MatrixType = Config::MatrixType;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	
private:
	MatrixType& matrix_;
	IndexType i_, j_;
	
public:
	DenseCCommaInitializer(MatrixType& m, const ElementType& first): matrix_(m), i_(0), j_(0) { insert(first);}
	DenseCCommaInitializer& operator,(const ElementType& val)
	{ 
		insert(val);
		return *this;
	}
	
protected:
	void insert(const ElementType& v)
	{
		assert(j_ < matrix_.Rows());
		assert(i_ < matrix_.Cols());
		matrix_.Set(i_, j_, v);
		++j_;
		if(j_==matrix_.Cols())
		{
			j_=0;
			++i_;
		}
	}
};

//--------------------------------Matrix------------------------------------------------

template<class CheckedMatrix>
class Matrix: public CheckedMatrix
{
public:
	using Config = CheckedMatrix::Config;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	using CommaInitializer = Config::CommaInitializer;
	
	Matrix(IndexType rows = 0, IndexType cols = 0, ElementType InitElement = ElementType()): CheckedMatrix(rows, cols) { this->InitElements(InitElement); }
	
	template<class A>
	Matrix(const Matrix<A>& m){ 	}
	
	~Matrix()	{ 	}
	
	template<class Expr>
	Matrix(const BinaryExpression<Expr>& expr): CheckedMatrix(expr.Rows(), expr.Cols())	{ 	expr.Assign(this);}
	
	CommaInitializer operator=(const ElementType& v){	return CommaInitializer(*this,v); }	
	
	template<class A>
	Matrix& operator=(const Matrix<A>& m)
	{ 
		MATRIX_ASSIGMENT<A>::RET::assign(this, &m);
		return *this; 
	}
	
	std::ostream& Display(std::ostream& out) const
	{
		for(IndexType i = 0;i < this->Rows() ; ++i)
		{
			for(IndexType j = 0; j < this->Cols() ; ++j)
				out<<this->Get(i,j)<<" ";
			out<<std::endl;
		}

		return out;
	}
private:	
	template<class Expr>
	Matrix& operator=(const BinaryExpression<Expr>& expr)
	{ 
		expr.Assign(this);
		return *this; 
	}
	
	template<class C, class D>
	Matrix& operator=(const AdditionExpression<C,D> expr) {	return *this; }
};

//--------------------------------Matrix------------------------------------------------

template<class CheckedMatrix>
class Scalar: public CheckedMatrix
{
public:
	using Config = CheckedMatrix::Config;
	using IndexType = Config::IndexType;
	using ElementType = Config::ElementType;
	using CommaInitializer = Config::CommaInitializer;
	
	Scalar(ElementType n_ = 0): CheckedMatrix(1, 1) { this->InitElements(n_); }
	
	template<class A>
	Scalar(const Scalar<A>& m){ 	}
	
	~Scalar()	{ 	}
	
	template<class Expr>
	Scalar(const BinaryExpression<Expr>& expr): CheckedMatrix(1, 1)	{ expr.Assign(this);}
	
	template<class A>
	Scalar& operator=(const Scalar<A>& m)	{	return *this; 	}	
};

#endif
