#include "../Unit/Unit.hpp"
#include "../Descriptor/IDescriptor.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"
#include "../Matrix/MatrixParsers.hpp"
#include "../HTMLBuilder/HTMLBuilder.hpp"

#pragma once

template<typename T>
class ICounterIO
{
	friend T;
	using Type = T;
	using DataModel = typename T::DataModel;
	virtual std::unique_ptr<DataModel> read() const = 0;
	virtual std::ostream& display(std::ostream& out) const = 0;
};

template<typename T>
class CounterIO: public ICounterIO<T>
{
public:
	using Type = T;
	using DescriptorType = typename T::DescriptionType;
	using DataModel = typename T::DataModel;
	using Unit = typename T::Unit;
	CounterIO(std::shared_ptr<Factory<IToken>> f, const DescriptorType& d): descriptor{d}, parser{std::make_unique<CounterParser<Type>>(f,descriptor.Filename())} {}
private:	
	DescriptorType descriptor;
    std::unique_ptr<IMatrixParser<2>> parser;   
	virtual std::unique_ptr<DataModel> read() const 
	{ 
		auto dm = std::make_unique<DataModel>((*parser)()); 
		auto elements = dm->Elements();
		std::vector<std::vector<std::shared_ptr<IElement>>> newElements;
		
		std::unique_ptr<BaseVisitor> cvv = std::make_unique<ConsumptionVisitor<Quantity<Unit>>>();
		cvv = dm->Accept(std::move(cvv));
		auto cv = cvv->template As<ConsumptionVisitor<Quantity<Unit>>>();
		auto values = cv.Values();
		auto diffs = cv.Diffs();
		auto spans = cv.Spans();
		auto times = cv.Times()();
		auto avs = cv.Averages();

		size_t rows = elements.size() / 2;
		int j = 0;
		for(int i = 0; i <= rows; i = i + 2)
		{
			std::vector<std::shared_ptr<IElement>> nElements;
			auto nR = elements[i+1];
			nElements.push_back(elements[i]);
			nElements.push_back(elements[i+1]);
			auto oR = elements[i+3];
			nElements.push_back(values[j].Clone());
			nElements.push_back(std::make_unique<Entry>(asString(diffs[j])));
			nElements.push_back(spans[j].Clone());
			nElements.push_back(std::make_unique<Entry>(asString(times[j])));
			nElements.push_back(avs[j++].Clone());

			newElements.push_back(nElements);
		}
		std::vector<std::shared_ptr<IElement>> nElements2;
		nElements2.insert(nElements2.end(), elements.end()-4, elements.end()-2);
		nElements2.push_back((*(values.end()-1)).Clone());
		nElements2.push_back(std::make_unique<Entry>(asString(*(diffs.end()-1))));
		nElements2.push_back((*(spans.end()-1)).Clone());
		nElements2.push_back(std::make_unique<Entry>(asString(*(times.end()-1))));
		nElements2.push_back((*(avs.end()-1)).Clone());
		newElements.push_back(nElements2);
		std::vector<std::shared_ptr<IElement>> nElements;
		nElements.insert(nElements.end(), elements.end()-2, elements.end());
		nElements.push_back(std::make_unique<Empty>());
		nElements.push_back(std::make_unique<Empty>());
		nElements.push_back(std::make_unique<Empty>());
		nElements.push_back(std::make_unique<Empty>());
		nElements.push_back(std::make_unique<Empty>());
		newElements.push_back(nElements);
		auto modell = Init(newElements)();
		auto mf1 = MatrixFormatter(modell);
        auto html = HtmlBuilder(DescriptorType::Identifier +".html",descriptor.Path());

		html.Add(mf1.Html());
		
		return dm;
	}
	virtual std::ostream& display(std::ostream& out) const { 	return out;	}

	auto asString(const auto& val) const
	{
		std::stringstream ss;
		ss<<val;
		return ss.str();
	}
};
