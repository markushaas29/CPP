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
struct Red: Color::Code<31>			{ inline static constexpr const char* Id = "red"; };
struct Green: Color::Code<32>		{ inline static constexpr const char* Id = "green"; };
struct Yellow: Color::Code<93>		{ inline static constexpr const char* Id = "yellow"; };
struct Blue: Color::Code<34>		{ inline static constexpr const char* Id = "blue"; };
struct Black: Color::Code<1>		{ inline static constexpr const char* Id = "black"; };
struct Gray: Color::Code<90>		{ inline static constexpr const char* Id = "gray"; };
struct Magenta: Color::Code<35>		{ inline static constexpr const char* Id = "magenta"; };
struct Default: Color::Code<39>{};
struct White: Color::Code<97>		{inline static constexpr const char* Id = "white";};

//Color 	Foreground Code 	Background Code
//Black 	30 	40
//Red 	31 	41
//Green 	32 	42
//Yellow 	33 	43
//Blue 	34 	44
//Magenta 	35 	45
//Cyan 	36 	46
//Light Gray 	37 	47
//Gray 	90 	100
//Light Red 	91 	101
//Light Green 	92 	102
//Light Yellow 	93 	103
//Light Blue 	94 	104
//Light Magenta 	95 	105
//Light Cyan 	96 	106
//White 	97 	107
#endif
