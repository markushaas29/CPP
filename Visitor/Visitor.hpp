#ifndef VISITOR_HPP
#define VISITOR_HPP

	class BaseVisitor
	{
	public:
		virtual ~BaseVisitor(){}
	};
	
	template<class T, typename R = void>
	class Visitor
	{
	public:
		using ReturnType = R;
		virtual ReturnType Visit(T&) = 0;
	};
	
	template<class T, typename R = void>
	class ConstVisitor
	{
	public:
		using ReturnType = R;
		virtual ReturnType Visit(const T*) = 0;
	};
		
	template<typename R = void>
	class BaseVisitable
	{
	public:
		using ReturnType = R;
		virtual ~BaseVisitable(){}
		virtual ReturnType Accept(BaseVisitor&) const = 0;
	protected:
		template<class T>
		static ReturnType AcceptImpl(T& visited, BaseVisitor& visitor) 
		{
			if(Visitor<T,R>* p = dynamic_cast<Visitor<T,R>*>(&visitor))
				return p->Visit(visited);
			
			return ReturnType();
		}		
		
		template<class T>
		static ReturnType AcceptConstImpl(const T* visited, BaseVisitor& visitor) 
		{
			std::cout<<"Accept"<<std::endl;
			
			if(ConstVisitor<T,R>* p = dynamic_cast<ConstVisitor<T,R>*>(&visitor))
				std::cout<<"Accept Const"<<std::endl;
			
			if(ConstVisitor<T,R>* p = dynamic_cast<ConstVisitor<T,R>*>(&visitor))
				return p->Visit(visited);
			
			return ReturnType();
		}		
	};
	
#define DEFINE_VISITABLE() virtual ReturnType Accept(BaseVisitor& visitor) const { return AcceptImpl(*this, visitor); }
#define DEFINE_CONSTVISITABLE() virtual ReturnType AcceptConst(BaseVisitor& visitor) const { return AcceptConstImpl(this, visitor); }

#endif
