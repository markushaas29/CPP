#include <memory>
#include "../Logger/Logger.hpp"

#ifndef REPOSITORYVALUE_HPP
#define REPOSITORYVALUE_HPP

namespace FS
{
	//~ template<typename Iterator = std::vector<std::string>::const_iterator>
	class RepositoryValue
	{
	public:
		//~ RepositoryValue() = delete;
		RepositoryValue() = default;
		RepositoryValue(const std::string n): _name{n} {};
		virtual const std::string& Name() { return _name ; }
		virtual RepositoryValue*  Cast() { return  new RepositoryValue(); }
	protected:
		const std::string& name() { return _name; };
	private:
		const std::string _name;
	};
	
	template<typename T = int>
	class CounterValue: public RepositoryValue
	{
	public:
		CounterValue(): RepositoryValue{"CoutnterValue"} {};
		CounterValue<T>*  Cast() override{ return this; }
		const std::string& Name() { return name();  }
	private:
		T value = T{};
	};
	
	template<typename T>
	class AccountValue: public RepositoryValue
	{
	public:
		AccountValue(): RepositoryValue{"AccountValue"}{};
		AccountValue<T>*  Cast() override { return this; }
		const std::string& Name() override { return name();  }
	private:
		T value = T{};
	};
}
#endif
