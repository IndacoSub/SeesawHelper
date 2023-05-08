#include <string>
#include <type_traits>

#define CONCAT2(a, b) a ## b
#define CONCAT(a, b) CONCAT2(a, b)
#define TYPE(x) decltype(x)
#define MAKEGETTERFUNCNAME(x) CONCAT(Get, x)
#define MAKESETTERFUNCNAME(x) CONCAT(Set, x) 
#define GET(x) TYPE(x) MAKEGETTERFUNCNAME(x) (void) const {return this->x;}
#define SET(x) void MAKESETTERFUNCNAME(x) (TYPE(x) const& set) {this->x = set;}
#define GETSET(x) GET(x) SET(x)

// Four columns:
	// Letter
	// Unknown character
	// X offset
	// Extra parameters

class DatEntry {
private:
	char Letter = '\0';	// Can also be a number!
	char IsActuallyUsed = '□';	// Used to note which letters are actually used (as discovered from looking at answer011.dat), either □ (U+25A1) or empty
	std::uint64_t OffsetX = 0;
	std::string ExtraParam = "DefaultExtraParam";

public:
	DatEntry() = default;
	virtual compl DatEntry() = default;

	std::string Stringize(void) const {

		std::string const letter_str = std::string{ Letter };
		std::string const actually_used_str = std::string{ IsActuallyUsed };

		return std::string(
			letter_str + "," +
			actually_used_str + "," +
			std::to_string(OffsetX) + "," +
			ExtraParam
		);
	}

	static char GetActuallyUsedDefault(void) {
		// Either □ (U+25A1) or empty

		/*
		// Unicode
		return static_cast<char>(0x25A1);
		*/
		return ' ';
	}

	static std::uint64_t GetNormalDistance(void) {

		// 90 is commonly used in the .dat between two letters
		return 90;
	}

	static std::uint64_t GetSpaceDistance(void) {

		// 120 is communly used after a space
		return 120;
	}

	static std::string GetExtraParamDefault(void) {

		return "";
	}

	GETSET(Letter);
	GETSET(IsActuallyUsed);
	GETSET(OffsetX);
	GETSET(ExtraParam);
};