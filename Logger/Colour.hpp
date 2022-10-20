#include <iostream>

#ifndef COLOUR_HPP
#define COLOUR_HPP
namespace Color {
	struct Code {
		static constexpr uint FG_RED     = 31;
		static constexpr uint FG_GREEN    = 32;
		static constexpr uint FG_YELLOW   = 93;
		static constexpr uint FG_BLUE     = 34;
		static constexpr uint FG_DEFAULT  = 39;
		static constexpr uint BG_RED      = 41;
		static constexpr uint BG_GREEN    = 42;
		static constexpr uint BG_BLUE     = 44;
		static constexpr uint BG_DEFAULT  = 49;
	};
			
	class Modifier {
		uint code;
	public:
		Modifier(uint pCode) : code(pCode) {}
		friend std::ostream&	operator<<(std::ostream& os, const Modifier& mod) {
					return os << "\033[" << mod.code << "m";
				}
		};
}
#endif
