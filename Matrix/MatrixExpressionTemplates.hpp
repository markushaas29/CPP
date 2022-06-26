#include "../Logger/Logger.hpp"
#include "Generator.hpp"
#include "MatrixCache.hpp"

#ifndef MATRIXEXPRESSIONTEMPLATES_H
#define MATRIXEXPRESSIONTEMPLATES_H

template<class CheckedMatrix>
class Matrix;

template<class ExpressionType>
class BinaryExpression; 

using GeneratorT = Generator<int,int>;

//----------------------------------------------------------------------------------------------------AdditionExpression--------------------------------------

template<class A, class B>
struct ADD_RESULT_TYPE
{
	using RET = GeneratorT;
};

struct RectAssignment
{
	template<class Res, class M>
	static void assign(Res* res, M* m)
	{
		using IndexType = Res::Config::IndexType;
		for(IndexType i = 0;i < m->Rows() ; ++i)
			for(IndexType j = 0; j < m->Cols() ; ++j)
				res->Set(i,j,m->Get(i,j));
	}
};


template<class A>
struct MATRIX_ASSIGMENT
{
	using RET = RectAssignment;
};

struct RectAddGetElement
{
	template<class IndexType, class ResultType, class LeftType, class RightType>
	static typename ResultType::ElementType Get(const IndexType& i, const IndexType& j, const ResultType* res, const LeftType& leftType, const RightType& rightType)
	{
		return leftType.Get(i,j) + rightType.Get(i,j);
	}
	
};

template<class A, class B>
struct MATRIX_ADD_GET_ELEMENT
{
	using RET = RectAddGetElement;
};

template<class A, class B>
class AdditionExpression
{
public: 
	using LeftType = A;
	using RightType = B;
	
	using Config = ADD_RESULT_TYPE<LeftType,RightType>::RET::Config;
	using ElementType = Config::ElementType;
	using IndexType = Config::IndexType;
	
private:
	const LeftType& left_;
	const RightType& right_;
	
protected:
	const IndexType rows_, cols_;
	
public:
	AdditionExpression(const LeftType& m1, const RightType& m2): left_(m1), right_(m2), rows_(m1.Rows()), cols_(m1.Cols())
	{
		if(m1.Cols() != m2.Cols() || m1.Rows() != m2.Rows()) 
			throw "argument matrices are incompatible";
	}
	
	template<class C, class D>
	AdditionExpression(const AdditionExpression<C,D>& expr){ 	}
	
	template<class C, class D>
	AdditionExpression<C,D> operator=(const AdditionExpression<C,D> expr)	{ 	return *this; 	}
	
	ElementType Get(const IndexType& i, const IndexType& j) const {	return MATRIX_ADD_GET_ELEMENT<LeftType, RightType>::RET::Get(i, j, this, left_, right_);	}
	auto& operator()(IndexType r, IndexType c) const {	return this->Get(r,c); }
	
	constexpr IndexType Rows() const { return rows_ ;}
	constexpr IndexType Cols() const { return cols_ ;}
	
	~AdditionExpression(){	}
};

//----------------------------------------------------------------------------------------------------MultiplicationExpression--------------------------------------

template<class A, class B>
struct MULTIPLY_RESULT_TYPE
{
	using RET = GeneratorT;
};

struct RectMultiplyGetElement
{
	template<class IndexType_, class ResultType, class LeftType, class RightType>
	static typename ResultType::ElementType Get(const IndexType_& i, const IndexType_& j, const ResultType* res, const LeftType& leftType, const RightType& rightType)
	{
		using Config = ResultType::Config;
		using ElementType = Config::ElementType;
		using IndexType = Config::IndexType;
		ElementType result = ElementType(0);
		
		for(IndexType k = 0; k < leftType.Cols(); ++k)
			result += getElement(i,k,leftType) * getElement(k,j,rightType);
		
		return result;
	}
	
private:
	template<class IndexType_, class MatrixType>
	static typename MatrixType::ElementType getElement(const IndexType_& i, const IndexType_& j, const MatrixType& matrix){	return matrix.Get(i,j); }	
};

template<class A, class B>
struct MATRIX_MULTIPLY_GET_ELEMENT{	using RET = RectMultiplyGetElement; };

template<class A, class B>
class MultiplicationExpression
{
public: 
	using LeftType = A;
	using RightType = B;
	
	using Config = MULTIPLY_RESULT_TYPE<LeftType,RightType>::RET::Config;
	using ElementType = Config::ElementType;
	using IndexType = Config::IndexType;	
private:
	const LeftType& left_;
	const RightType& right_;
	const int scalar_ = 0;
	
	template<class C, class D>
	MultiplicationExpression(const MultiplicationExpression<C,D>& expr)	{ 	}
	
	template<class C, class D>
	MultiplicationExpression<C,D> operator=(const MultiplicationExpression<C,D> expr)	{	return *this; 	}	
protected:
	const IndexType rows_, cols_;
	
public:
	~MultiplicationExpression()	{	}
	
	template<class M1, class M2>
	MultiplicationExpression(const Matrix<M1>& m1, const Matrix<M2>& m2): left_(m1),right_(m2), rows_(m1.Rows()), cols_(m2.Cols()){ ParameterCheck(m1, m2); }
	
	template<class S1, class S2>
	MultiplicationExpression(const S1& m1, const S2& m2): left_(m1),right_(m2), rows_(1), cols_(1){  }
	
	template<class S, class M2>
	MultiplicationExpression(const S& s, const Matrix<M2>& m2): scalar_(s.Get()), left_(s),right_(m2), rows_(m2.Rows()), cols_(m2.Cols()){  }

	template<class S, class Expr>
	MultiplicationExpression(const S& s, const BinaryExpression<Expr>& expr): left_(s), right_(expr), rows_(expr.Rows()), cols_(expr.Cols()){  }
	
	template<class M1, class S>
	MultiplicationExpression(const Matrix<M1>& m1, const S& s): scalar_(s.Get()), left_(m1),right_(s), rows_(m1.Rows()), cols_(m1.Cols()){  }
	
	template<class Expr, class M2>
	MultiplicationExpression(const BinaryExpression<Expr>& expr, const Matrix<M2>& m2): left_(expr), right_(m2), rows_(expr.Rows()), cols_(m2.Cols()){ ParameterCheck(expr, m2); }
	
	template<class Expr, class S>
	MultiplicationExpression(const BinaryExpression<Expr>& expr, const S& m2): left_(expr), right_(m2), rows_(expr.Rows()), cols_(expr.Cols()){  }

	template<class M1, class Expr>
	MultiplicationExpression(const Matrix<M1>& m1, const BinaryExpression<Expr> expr): left_(m1), right_(expr), rows_(m1.Rows()), cols_(expr.Cols()){ ParameterCheck(m1, expr); }
	
	template<class Expr1, class Expr2>
	MultiplicationExpression(const BinaryExpression<Expr1>& expr1, const BinaryExpression<Expr2> expr2): left_(expr1), right_(expr2), rows_(expr1.Rows()), cols_(expr2.Cols()) {		}
	
	ElementType Get(const IndexType& i, const IndexType& j) const 
	{	
		if (left_.Rows() > 1 && left_.Cols() > 1 && right_.Rows() > 1 && right_.Cols() > 1)
			return MATRIX_MULTIPLY_GET_ELEMENT<LeftType, RightType>::RET::Get(i, j, this, left_, right_);	
		if (left_.Rows() == 1 && left_.Cols() == 1)
			return left_.Get(0,0) * right_.Get(i,j);
		if (right_.Rows() == 1 && right_.Cols() == 1)
			return left_.Get(i,j) * right_.Get(0,0);

		return left_.Get(0,0) * right_.Get(0,0);
	}
	auto& operator()(IndexType r, IndexType c) const {	return this->Get(r,c); }
	
	constexpr IndexType Rows() const { return rows_ ;}
	constexpr IndexType Cols() const { return cols_ ;}
	
private:
	void ParameterCheck(const A& m1, const B& m2)
	{
		if(m1.Cols() != m2.Rows())
			throw "argument matrices are incompatible!";
	}
};

//----------------------------------------------------------------------------------------------------BinaryExpression--------------------------------------

template<class ExpressionType>
class BinaryExpression : public ExpressionType
{
	template<class Expr>
	BinaryExpression(BinaryExpression<Expr>& expr) {};
	template<class Expr>
	BinaryExpression(BinaryExpression<Expr>&& expr) {};
public:
	using LeftType = ExpressionType::LeftType;
	using RightType = ExpressionType::RightType;
	using MatrixType = ExpressionType::Config::MatrixType;
	using IndexType = ExpressionType::IndexType;
	
 	~BinaryExpression(){ 	};
	BinaryExpression(const LeftType& op1, const RightType& op2): ExpressionType(op1, op2){	};
	
	template<class Res>
	Matrix<Res>* Assign(Matrix<Res>* const result) const
	{
		MATRIX_ASSIGMENT<MatrixType>::RET::assign(result, this);
		return result;
	}
	
	template<class Expr>
	BinaryExpression<Expr> operator=(const BinaryExpression<Expr>& expr)
	{ 
		expr.Assign(this);
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
};

#endif
