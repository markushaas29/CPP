#pragma once

class IDescriptor
{
	virtual std::ostream& display(std::ostream& out) const = 0;
	friend std::ostream& operator<<(std::ostream& out, const IDescriptor& i) {	return i.display(out); } 
};

class IDescription: public IDescriptor
{
public:
	virtual const std::string& Path() const = 0;
};
