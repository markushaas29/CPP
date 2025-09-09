#pragma once

class IHtmlElement;
class ICss;

class IHtml
{
public:
	auto Html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const { return html(std::move(v), std::move(css),n,id); }
	auto Html(std::unique_ptr<ICss> css, const std::string& n="", const std::string& id="") const { return cssHtml(std::move(css),n,id); }
	auto HtmlOut(std::unique_ptr<IHtmlElement> v = nullptr, uint i = 0, std::unique_ptr<ICss> css = nullptr,const std::string& n="", const std::string& id="") const { return html(std::move(v), std::move(css),n,id); }
private:
	virtual std::unique_ptr<IHtmlElement> html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const = 0;
	virtual std::unique_ptr<IHtmlElement> cssHtml(std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const = 0;
};
