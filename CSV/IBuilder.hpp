#pragma once

class IHtmlElement;
class ICss;

template<typename T, typename E = ICss>
class IBuilder
{
public:
	auto With(std::unique_ptr<E> css = nullptr) const { return with(std::move(css)); }
private:
	virtual std::unique_ptr<T> with(std::unique_ptr<E> css = nullptr) const = 0;
};
