#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <optional>
#include <regex>

#include "helpers.h"

int main(int argc, char* argv[]) {

	if (argc < 1) {
		std::cout << "Usage: SeesawHelper CSV_FILE_DUMPED_FROM_MINIGAME" << std::endl;
		return EXIT_FAILURE;
	}

	if (argv[1] == nullptr) {
		std::cout << "Usage: SeesawHelper CSV_FILE_DUMPED_FROM_MINIGAME" << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<std::string> read_strings{};
	std::ifstream file(argv[1], std::ios::in);

	if (!file.good()) {
		return EXIT_FAILURE;
	}

	if (file.is_open()) {

		std::string line{};

		while (std::getline(file, line)) {
			char zero_ch = 0;
			std::string zero_str = std::string{ zero_ch };
#ifdef _WIN32
			line = std::regex_replace(line, std::regex(zero_str), "");
#else
			for (int j = 0; j < line.length(); j++) {
				line.erase(line.begin() + j);
			}
#endif
			read_strings.push_back(line);
		}

		if (line.length() <= 0) {
			// (?)
		}

		file.close();
	}

	std::string first_line = read_strings[0];
	
	// Four stages:
	// Letter
	// Unknown character
	// X offset
	// Extra parameters

	class DatParameters {
	private:
		char Letter = '\0';
		char Unknown = '□';
		std::uint64_t OffsetX = 0;
		std::string ExtraParam = "DefaultExtraParam";

	public:
		DatParameters() = default;
		virtual compl DatParameters() = default;

		std::string Stringize(void) const {

			std::string const letter_str = std::string{ Letter };
			std::string const unknown_str = std::string{ Unknown };

			return std::string(
				letter_str + "," +
				unknown_str + "," +
				std::to_string(OffsetX) + "," +
				ExtraParam
			);
		}

		static char GetUnknownDefault(void) {
			/*
			// Unicode
			return static_cast<char>(0x25A1);
			*/
			return ' ';
		}

		static std::uint64_t GetNormalDistance(void) {

			return 90;
		}

		static std::uint64_t GetSpaceDistance(void) {

			return 120;
		}

		static std::string GetExtraParamDefault(void) {

			return "";
		}
		
		GETSET(Letter);
		GETSET(Unknown);
		GETSET(OffsetX);
		GETSET(ExtraParam);
	};

	std::vector<DatParameters> parameters{};

	bool first = true;
	for (auto& string : read_strings) {

		if (first) {
			// The first line contains japanese characters and IDK what they mean
			// so let's just skip them as they don't seem to contain
			// any useful info
			first = false;
			continue;
		}

		if (string == "\0") {
			continue;
		}

		/*
		for (int j = 0; j < string.length(); j++) {
			std::cout << (int)string[j] << std::endl;
		}
		*/
		
		std::cout << "Full string □: \"" << string << "\"" << std::endl;

		DatParameters param{};

		std::cout << "Letter" << std::endl;

		param.SetLetter(string[0]);
		std::cout << "Letter: " << string[0] << std::endl;

		std::cout << "Unknown" << std::endl;
		param.SetUnknown(DatParameters::GetUnknownDefault());

		std::cout << "Offset" << std::endl;

		auto find_first_numeric = [&](std::string const& str) -> std::size_t {

			for (std::uint64_t j = 0; j < str.length(); j++) {
				if (std::isdigit(str[j])) {
					return j;
				}
			}
			return UINT64_MAX;
		};

		auto find_last_numeric = [&](std::uint64_t min, std::string const& str) -> std::size_t {

			for (std::uint64_t j = min; j < str.length(); j++) {
				if (!std::isdigit(str[j])) {
					return j;
				}
			}
			return UINT64_MAX;
		};
		std::uint64_t const find_first_number = find_first_numeric(string);
		std::uint64_t const find_last_number = find_last_numeric(find_first_number, string);
		std::string const offset_str = string.substr(find_first_number, find_last_number - find_first_number);
		std::cout << "Offset_str: \"" << offset_str << "\"" << std::endl;
		std::uint64_t const offset = std::stoull(offset_str);
		param.SetOffsetX(offset);

		std::cout << "Param" << std::endl;

		std::string const argument = string.substr(find_last_number + 1);
		param.SetExtraParam(argument);
		std::cout << "Param: \"" << argument << "\"" << std::endl;

		parameters.push_back(param);
	}

	std::uint64_t cont = 0;
	std::cout << "Parameters:" << std::endl;
	std::cout << std::endl;
	std::cout << "(PUT JAPANESE IN MANUALLY)" << std::endl;
	for (auto const& p : parameters) {
		std::cout << p.Stringize() << std::endl;
		cont++;
	}

	constexpr bool random_rotates = false;

	std::string new_string{};
	std::cout << "Insert new string" << std::endl;
	std::getline(std::cin >> std::ws, new_string);
	std::vector<DatParameters> new_parameters{};
	bool last_was_space = false;
	std::uint64_t distance = 0;
	bool first_dist = true;
	for (std::uint64_t j = 0; j < new_string.length(); j++) {
		
		if (new_string[j] == ' ') {
			last_was_space = true;
			continue;
		}

		DatParameters param;
		param.SetLetter(new_string[j]);

		param.SetUnknown(DatParameters::GetUnknownDefault());

		if (!first_dist) {
			if (last_was_space) {
				distance += DatParameters::GetSpaceDistance();
				last_was_space = false;
			}
			else {
				distance += DatParameters::GetNormalDistance();
			}
		}
		else {
			distance = 0;
			first_dist = false;
		}

		param.SetOffsetX(distance);

		if (j < parameters.size() && random_rotates) {
			param.SetExtraParam(parameters[j].GetExtraParam());
		}
		else {
			param.SetExtraParam(DatParameters::GetExtraParamDefault());
		}

		new_parameters.push_back(param);
	}

	std::ofstream out("out.txt", std::ios::out);
	if (!out.good()) {
		out << "" << std::endl;
	}

	for (auto const& p : new_parameters) {
		out << p.Stringize() << std::endl;
	}

	out.close();

	// □ (U+25A1)
	// 文字 (UTF16),表示前 (UTF16),X_Offset (u16),パラメータ (ASCII)
	std::cout << "Done (saved to out.txt)!" << std::endl;
	std::cout << "Remember to add squares(U + 25A1) and japanese (on top)!" << std::endl;
}