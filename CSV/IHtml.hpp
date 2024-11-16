#pragma once

class IHtmlElement;
class ICss;

class IHtml
{
public:
	auto Html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr) const { return html(std::move(v), std::move(css)); }
	auto Html(std::unique_ptr<ICss> css) const { return cssHtml(std::move(css)); }
private:
	virtual std::unique_ptr<IHtmlElement> html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr) const = 0;
	virtual std::unique_ptr<IHtmlElement> cssHtml(std::unique_ptr<ICss> css = nullptr) const = 0;
};
