#include <iostream>

#ifndef COLOUR_HPP
#define COLOUR_HPP
struct Color {
	
	template<uint Num>
	struct Code
	{ 
		static  inline constexpr uint FG = Num; 
		static  inline constexpr uint BG = Num + 10; 
	};
	
			
	template<typename T>
	class Modifier {
		static inline constexpr uint code = T::FG;
	public:
		constexpr Modifier() = default;
		friend std::ostream& operator<<(std::ostream& os, const Modifier& mod) {
					return os << "\033[" << mod.code << "m";
				}
	};
};
struct Red: Color::Code<31>{ inline static constexpr const char* Id = "red"; };
struct Green: Color::Code<32>{};
struct Yellow: Color::Code<93>{};
struct Blue: Color::Code<34>{};
struct Default: Color::Code<39>{};
struct White: Color::Code<97>{};
#endif
