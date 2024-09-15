#pragma once

class IHtmlElement;

class IHtml
{
public:
	auto Html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr) const { return html(); }
private:
	virtual std::unique_ptr<IHtmlElement> html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr) const = 0;
};
