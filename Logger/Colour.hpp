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
	
	using Red = Code<31>;
	using Green = Code<32>;
	using Yellow = Code<93>;
	using Blue = Code<34>;
	using Default = Code<39>;
	using White = Code<97>;
			
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
#endif
