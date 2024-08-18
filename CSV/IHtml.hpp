#pragma once

class IHtmlElement;

class IHtml
{
public:
	auto Html() { return html(); }
private:
	virtual std::unique_ptr<IHtmlElement> html() const = 0;
};
