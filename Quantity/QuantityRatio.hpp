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

template<uint N, uint D, int Ex, template<int> class Derived, typename U = Scalar>
struct QuantityRatioBase
{
	using Type = Derived<Ex>;
	using DividerUnit = U;
	template<int T> using RatioType = Derived<T>;
	
	static constexpr int Exponent = Ex;
	static constexpr uint BaseNum = N;
	static constexpr uint BaseDenom = D;
	static constexpr std::ratio<BaseNum, BaseDenom> RatioBase = std::ratio<BaseNum, BaseDenom>();
	static constexpr uint Num = BaseNum > 1 ? Math::Pow<BaseNum,Ex>::Result : 1;;
	static constexpr uint Denom = BaseDenom > 1 ? Math::Pow<BaseDenom,Ex>::Result : 1;;
	static constexpr std::ratio<Num, Denom> Ratio = std::ratio<Num, Denom>();
	static inline const std::string Sign() { return Derived<Ex>::Sign(); }
	static inline const std::string Name;
	static constexpr double Factor = ((double)Num / Denom);	
	static constexpr double BaseFactor = ((double)BaseNum / BaseDenom);	
	
	static auto Out() { return Sign()  + ((Ex != 0 && Ex != 1) ? ("^" + std::to_string(Ex)) : ""); };
	
	template<int Fac>
	struct PowBy
	{
		static constexpr int Factor = Fac;
		using Type = Derived<Fac>;
	};
};

template<int Ex>
struct MinutesBase: public QuantityRatioBase<60, 1, Ex, MinutesBase,Unit<0,0,0,Ex>> 
{	
	static inline const std::string Sign() { return  "min"; } 
	inline static constexpr const char* Name = "Minuttes"; 
};

template<int Ex>
struct HoursBase: public QuantityRatioBase<3600, 1,Ex, HoursBase,Unit<0,0,0,Ex>> 
{	
	static inline const std::string Sign() { return "h"; }
	inline static constexpr const char* Name = "Hours"; 
};

template<int Ex>
struct DaysBase: public QuantityRatioBase<86400, 1, Ex,DaysBase,Unit<0,0,0,Ex>> 
{	
	static inline const std::string Sign() { return "d"; }
	inline static constexpr const char* Name = "Days"; 
};

template<int Ex>
struct MilliBase: public QuantityRatioBase<1, 1000, Ex, MilliBase> 
{	
	static inline const std::string Sign() { return "m"; }
	inline static constexpr const char* Name = "Milli"; 
};

template<int Ex>
struct CentiBase: public QuantityRatioBase<1, 100, Ex, CentiBase> 
{	
	static inline const std::string Sign() { return "c"; }
	inline static constexpr const char* Name = "Centi"; 
};

template<int Ex>
struct DeziBase: public QuantityRatioBase<1, 10, Ex, DeziBase> 
{	
	static inline const std::string Sign() { return "d"; }
	inline static constexpr const char* Name = "Dezi"; 
};

template<int Ex>
struct PureBase: public QuantityRatioBase<1,1,Ex,PureBase> 
{	
	static inline const std::string Sign() { return ""; }
	inline static constexpr const char* Name = "Pure"; 
};

template<int Ex>
struct KiloBase: public QuantityRatioBase<1000, 1, Ex, KiloBase> 
{	
	static inline const std::string Sign() { return "k"; }
	inline static constexpr const char* Name = "Kilo"; 
};

template<int Ex>
struct LiterBase: public QuantityRatioBase<1000, 1, Ex, LiterBase> 
{	
	static inline const std::string Sign() { return "l"; }
	inline static constexpr const char* Name = "Liter"; 
};

template<int Ex>
struct KiloHourBase: public QuantityRatioBase<3600, 1, Ex, KiloBase> 
{	
	static inline const std::string Sign() { return "k"; }
	inline static constexpr const char* Name = "KiloHour"; 
};

template<int Ex>
struct MegaBase: public QuantityRatioBase<1000000, 1, Ex, MegaBase> 
{	
	static inline const std::string Sign() { return "M"; }
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
	static inline const std::string Sign() { return "da"; }
	inline static constexpr const char* Name = "Deka"; 
};

using Minutes = MinutesBase<1>;
using Hours = HoursBase<1>;
using Days = DaysBase<1>;
using Milli = MilliBase<1>;
using Centi = CentiBase<1>;
using Dezi = DeziBase<1>;
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
	using DividerUnit = Transform<typename L::QuantityRatioType::DividerUnit, typename R::QuantityRatioType::DividerUnit, P>::Type;
	using qu = typename Transform<typename L::UnitType, typename R::UnitType, P>::Type; 
	using ru = typename Transform<qu,DividerUnit, MultiplyPolicy>::Type;
	using Derived = D<L,R>;
	using RatioType = typename L::QuantityRatioType;
	static constexpr int Exponent = 1;
	static constexpr uint BaseNum = 1;
	static constexpr uint BaseDenom = 1;
//	static constexpr std::ratio<BaseNum, BaseDenom> RatioBase = std::ratio<BaseNum, BaseDenom>();
//	static constexpr uint Num = BaseNum > 1 ? Math::Pow<BaseNum,Ex>::Result : 1;;
//	static constexpr uint Denom = BaseDenom > 1 ? Math::Pow<BaseDenom,Ex>::Result : 1;;
//	static constexpr std::ratio<Num, Denom> Ratio = std::ratio<Num, Denom>();
//	static inline const std::string Sign = Derived<Ex>::Sign;
//	static inline const std::string Name;
	static constexpr double Factor = ((double)L::Factor / R::Factor);	
//	static constexpr double BaseFactor = ((double)BaseNum / BaseDenom);	
//	
	inline static std::string Sign() {	return L::QuantityRatioType::Sign() == R::QuantityRatioType::Sign() ? L::QuantityRatioType::Sign() : L::QuantityRatioType::Sign() + R::QuantityRatioType::Sign(); }
	//inline static std::string Sign() { return L::QuantityRatioType::Sign() + Unit::Sign(); }
//	
	template<int Fac>
	struct PowBy
	{
		//static constexpr int Factor = Fac;
		using Type = Derived;//<Fac>;
	};
};

template<typename L, typename R>
struct QRDiv: public QRBase<L,R, QRDiv, DividePolicy>
{
	using Base = QRBase<L,R, QRDiv, DividePolicy>;
	static constexpr double Factor = ((double)L::QuantityRatioType::Factor / R::QuantityRatioType::Factor);	
	static std::string Out() {		return std::string(L::Sign) +  std::string(R::Sign) + "^-1"+ Base::Unit::Sign(); 	};
};

template<typename L, typename R>
struct QRMul: public QRBase<L,R, QRMul, MultiplyPolicy>
{
	using Base = QRBase<L,R, QRMul, MultiplyPolicy>;
	static constexpr double Factor = ((double)L::QuantityRatioType::Factor * R::QuantityRatioType::Factor);	
	static std::string Out() {		return std::string(L::Sign) +  std::string(R::Sign); 	};
};

template <template <int> class, template<int> class> 
struct IsSameTemplate : std::false_type{};

template <template <int> class T>
struct IsSameTemplate<T,T> : std::true_type{};
