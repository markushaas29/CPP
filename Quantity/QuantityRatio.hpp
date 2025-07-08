#include <memory>
#include <ratio>
#include "../Math/Math.hpp"
#include "../Unit/Unit.hpp"

#pragma once

template<typename U, typename QR,typename T1> class Quantity;

template<class Type>
struct QuantityRatio
{
	template<typename U, typename QR,typename T1>
	static decltype(auto) Cast(const Quantity<U,QR,T1> q){ return Quantity<U,Type,T1>{q.PureValue() * Type::Factor};}
};

template<int I, typename T>
struct PowUnit
{
	using Type = typename Transform<T, typename PowUnit<I-1,T>::Type, MultiplyPolicy>::Type;
};

template<typename T>
struct PowUnit<0,T>
{
	using Type = T;
};

template<uint Nom, uint D, int Ex, template<int> class Derived, typename U = Scalar>
struct QuantityRatioBase
{
	using Type = Derived<Ex>;
	
	using Unit = U;
//	using Unit = PowUnit<std::abs(std::abs(Ex)-1),U>::Type;
	template<int T> using RatioType = Derived<T>;
	
	static constexpr int N = Ex;
	static constexpr uint BaseNum = Nom;
	static constexpr uint BaseDenom = D;
	static constexpr std::ratio<BaseNum, BaseDenom> RatioBase = std::ratio<BaseNum, BaseDenom>();
	static constexpr long Num = BaseNum >= 1 ? (Ex > 0 ? Math::Pow<BaseNum,Ex>::Result : Math::Pow<BaseDenom,-Ex>::Result) : 1;
	static constexpr long Denom = BaseDenom >= 1 ? (Ex > 0 ? Math::Pow<BaseDenom,Ex>::Result : Math::Pow<BaseNum,-Ex>::Result) : 1;
	static constexpr std::ratio<Num, Denom> Ratio = std::ratio<Num, Denom>();
	static inline const std::string Sign = Derived<Ex>::Sign;
	static inline const std::string Name;
	static constexpr double Factor = ((double)Num / Denom);	
	static constexpr double BaseFactor = ((double)BaseNum / BaseDenom);	
	
	static auto Out() { return Sign + Unit::Sign(); };
	
	template<typename U2>
	static auto TransformUnit() { return Sign + Transform<Unit, U2, MultiplyPolicy>::Type::Sign(); };
	
	template<int Fac>
	struct PowBy
	{
		static constexpr int Factor = Fac;
		using Type = Derived<Fac>;
	};

	static std::string UnitRatio() 
	{ 
		if constexpr (N == 0) 
			return "";
		if constexpr (N == 1) 
			return std::string(Type::Sign);
		if constexpr (N == 2) 
			return std::string(Type::Sign)+ "²";
		if constexpr (N == 3) 
			return std::string(Type::Sign)+ "³";
		return std::string(Type::Sign) + "^" + std::to_string(N); 
	};
};

template<int Ex>
struct MinutesBase: public QuantityRatioBase<60, 1, Ex, MinutesBase,Unit<0,0,0,-Ex>> 
{	
	inline static constexpr const char* Sign = "min"; 
	inline static constexpr const char* Name = "Minuttes"; 
};

template<int Ex>
struct HoursBase: public QuantityRatioBase<3600, 1,Ex, HoursBase,Unit<0,0,0,-Ex>> 
{	
	inline static constexpr const char* Sign = "h"; 
	inline static constexpr const char* Name = "Hours"; 
};

template<int Ex>
struct DaysBase: public QuantityRatioBase<86400, 1, Ex,DaysBase,Unit<0,0,0,-Ex>> 
{	
	inline static constexpr const char* Sign = "ds"; 
	inline static constexpr const char* Name = "Days"; 
};

template<int Ex>
struct InchBase: public QuantityRatioBase<254, 10000, Ex, InchBase, Unit<0,-Ex>> 
{	
inline static constexpr const char* Sign = "\""; 
inline static constexpr const char* Name = "Inch"; 
};

template<int Ex>
struct MileBase: public QuantityRatioBase<621371, 1000000000, Ex, MileBase, Unit<0,-Ex>> 
{	
inline static constexpr const char* Sign = "mi"; 
inline static constexpr const char* Name = "Mile"; 
};

template<int Ex>
struct PoundBase: public QuantityRatioBase<453592, 10000000, Ex, PoundBase, Unit<0,-Ex>> 
{	
inline static constexpr const char* Sign = "lbs"; 
inline static constexpr const char* Name = "Pound"; 
};

template<int Ex>
struct MilliBase: public QuantityRatioBase<1, 1000, Ex, MilliBase> 
{	
inline static constexpr const char* Sign = "m"; 
inline static constexpr const char* Name = "Milli"; 
};

template<int Ex>
struct CentiBase: public QuantityRatioBase<1, 100, Ex, CentiBase> 
{	
	inline static constexpr const char* Sign = "c"; 
	inline static constexpr const char* Name = "Centi"; 
};

template<int Ex>
struct DeziBase: public QuantityRatioBase<1, 10, Ex, DeziBase> 
{	
	inline static constexpr const char* Sign = "d"; 
	inline static constexpr const char* Name = "Dezi"; 
};

template<int Ex>
struct PureBase: public QuantityRatioBase<1,1,Ex,PureBase> 
{	
	inline static constexpr const char* Sign = ""; 
	inline static constexpr const char* Name = "Pure"; 
};

template<int Ex>
struct KiloBase: public QuantityRatioBase<1000, 1, Ex, KiloBase> 
{	
	inline static constexpr const char* Sign = "k"; 
	inline static constexpr const char* Name = "Kilo"; 
};

template<int Ex>
struct LiterBase: public QuantityRatioBase<1, 1000, Ex, LiterBase, Unit<0,-(3*Ex)>> 
{	
	inline static constexpr const char* Sign = "l"; 
	inline static constexpr const char* Name = "Liter"; 
};

template<int Ex>
struct KiloHourBase: public QuantityRatioBase<3600, 1, Ex, KiloBase> 
{	
	inline static constexpr const char* Sign = "k"; 
	inline static constexpr const char* Name = "KiloHour"; 
};

template<int Ex>
struct MegaBase: public QuantityRatioBase<1000000, 1, Ex, MegaBase> 
{	
	inline static constexpr const char* Sign = "M"; 
	inline static constexpr const char* Name = "Mega"; 
};

//~ template<int Ex>
//~ struct Hekto: public SIPrefix<100, 1> 
//~ {	
	//~ inline static const std::string Sign = "h"; 
	//~ inline static const std::string Name = "Hekto"; 
//~ };

template<int Ex>
struct DekaBase: public QuantityRatioBase<10,1,Ex,DekaBase> 
{	
	inline static constexpr const char* Sign = "da"; 
	inline static constexpr const char* Name = "Deka"; 
};

using Minutes = MinutesBase<1>;
using Hours = HoursBase<1>;
using Days = DaysBase<1>;
using Inch = InchBase<1>;
using Mile = MileBase<1>;
using Pound = PoundBase<1>;
using Milli = MilliBase<1>;
using Centi = CentiBase<1>;
using Dezi = DeziBase<1>;
using Liter = LiterBase<1>;
using Kilo = KiloBase<1>;
using KiloHour = KiloHourBase<1>;
using Mega = MegaBase<1>;
using Pure = PureBase<1>;
using Deka = DekaBase<1>;

template<typename L, typename R, template<typename,typename> class D, template<typename,typename> class P>
struct QRBase
{
//	template<int T> using RatioType = Derived<T>;
	//typename Unit = L::Unit;
	using Unit = Transform<typename L::Unit, typename R::Unit, P>::Type;
	using Derived = D<L,R>;
//	static constexpr int Exponent = Ex;
//	static constexpr uint BaseNum = N;
//	static constexpr uint BaseDenom = D;
//	static constexpr std::ratio<BaseNum, BaseDenom> RatioBase = std::ratio<BaseNum, BaseDenom>();
//	static constexpr uint Num = BaseNum > 1 ? Math::Pow<BaseNum,Ex>::Result : 1;;
//	static constexpr uint Denom = BaseDenom > 1 ? Math::Pow<BaseDenom,Ex>::Result : 1;;
//	static constexpr std::ratio<Num, Denom> Ratio = std::ratio<Num, Denom>();
//	static inline const std::string Sign = Derived<Ex>::Sign;
//	static inline const std::string Name;
	static constexpr double Factor = ((double)L::Factor / R::Factor);	
//	static constexpr double BaseFactor = ((double)BaseNum / BaseDenom);	
//	
	inline static std::string Sign = Derived::Sign;
//	
//	template<int Fac>
//	struct PowBy
//	{
//		static constexpr int Factor = Fac;
//		using Type = Derived<Fac>;
//	};
};

template<typename L, typename R>
struct QRDiv: public QRBase<L,R, QRDiv, DividePolicy>
{
	using Base = QRBase<L,R, QRDiv, DividePolicy>;
	static constexpr double Factor = ((double)L::Factor / R::Factor);	
	static std::string Out() {		return std::string(L::Sign) +  std::string(R::Sign) + "^-1"+ Base::Unit::Sign(); 	};
};

template<typename L, typename R>
struct QRMul: public QRBase<L,R, QRMul, MultiplyPolicy>
{
	using Base = QRBase<L,R, QRDiv, MultiplyPolicy>;
	static constexpr double Factor = ((double)L::Factor * R::Factor);	
	static std::string Out() {		return std::string(L::Sign) +  std::string(R::Sign) + Base::Unit::Sign(); 	};
};

template <template <int> class, template<int> class> 
struct IsSameTemplate : std::false_type{};

template <template <int> class T>
struct IsSameTemplate<T,T> : std::true_type{};
