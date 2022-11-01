#include <memory>
#include "../Logger/Logger.hpp"
#include "../Common/UniqueCast.hpp"

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
		std::ostream& Display(std::ostream& strm) const {	return strm<<_name;}
	protected:
		const std::string& name() { return _name; };
	private:
		const std::string _name;
	};
	
	//~ template<typename C, typename S = T::char_<'\t'>>
	std::ostream& operator<<(std::ostream& strm, const RepositoryValue& rv){	return rv.Display(strm); }
	
	template<typename T = int>
	class CounterValue: public RepositoryValue
	{
	public:
		CounterValue(): RepositoryValue{"CoutnterValue"} {};
		CounterValue(const T& t): RepositoryValue{"CoutnterValue"}, value{t} {};
		CounterValue<T>*  Cast() override{ return this; }
		const std::string& Name() { return name();  }
		const std::string& Val1() { return name();  }
		T value = T{};
	private:
	};
	
	template<typename T>
	class AccountValue: public RepositoryValue
	{
	public:
		AccountValue(): RepositoryValue{"AccountValue"}{};
		AccountValue(const T& t): RepositoryValue{"CoutnterValue"}, value{t} {};
		AccountValue<T>*  Cast() override { return this; }
		const std::string& Name() override { return name();  }
		const std::string& Val() { return name();  }
		T value = T{};
	private:
	};
}
#endif
