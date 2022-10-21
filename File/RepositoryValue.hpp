#include "../Logger/Logger.hpp"

#ifndef REPOSITORYVALUE_HPP
#define REPOSITORYVALUE_HPP

namespace FS
{
	//~ template<typename Iterator = std::vector<std::string>::const_iterator>
	class RepositoryValue
	{
	public:
		RepositoryValue() = default;
		virtual void Get(const std::string& s) { Logger::Log("RepositoryValue"); }
	private:
	};
	
	template<typename T = int>
	class CounterValue: public RepositoryValue
	{
	public:
		CounterValue() = default;
		virtual void Get(const std::string& s) { Logger::Log("CoutnterValue"); }
	private:
		T value = T{};
	};
	
	template<typename T>
	class AccountValue: public RepositoryValue
	{
	public:
		AccountValue() = default;
		virtual void Get(const std::string& s) { Logger::Log("AccountValue"); }
	private:
		T value = T{};
	};
}
#endif
