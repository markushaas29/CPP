#include <memory>

#pragma once

class IModel
{
public:
	virtual std::unique_ptr<IModel> Model() const = 0;
	std::string Out(uint i = 0) const  	
	{
		std::string intent;
		intent.insert(0, i, '\t');	
		return out(intent, i);
	}	
private:
	virtual std::string out(const std::string& intent, uint i = 0) const = 0;	
};
