#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <filesystem>

#include "DatEntry.h"

int Start(std::string const& file_str);
std::vector<std::string> ReadFile(std::string const& file_str);
std::vector<DatEntry> ReadEntries(std::vector<std::string> const& read_strings);
void ViewEntries(std::vector<DatEntry> const& entries);
std::vector<DatEntry> CreateEntries(std::vector<DatEntry> const& old_entries);

int main(int argc, char* argv[]) {

	if (argc < 1) {
		std::cout << "Usage: SeesawHelper CSV_FILE_DUMPED_FROM_MINIGAME (/minigame/anagram/anagram_US, answer*.dat)" << std::endl;
		return EXIT_FAILURE;
	}

	if (argv[1] == nullptr) {
		std::cout << "Usage: SeesawHelper CSV_FILE_DUMPED_FROM_MINIGAME (/minigame/anagram/anagram_US, answer*.dat)" << std::endl;
		return EXIT_FAILURE;
	}

	std::string const file_str = argv[1];

	int const ret = Start(file_str);
	return ret;
}

int Start(std::string const& file_str) {

	std::vector<std::string> const read_strings = ReadFile(file_str);

	if (read_strings.size() <= 0) {
		// Should not happen(?)
		return EXIT_FAILURE;
	}

	std::vector<DatEntry> const entries = ReadEntries(read_strings);

	ViewEntries(entries);

	std::vector<DatEntry> const new_entries = CreateEntries(entries);

	std::string const out_str = std::filesystem::path(file_str).filename().string() + "_out.txt";
	if (std::filesystem::exists(out_str)) {
		try {
			std::filesystem::remove(out_str);
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
	std::ofstream out(out_str, std::ios::out);
	out << "" << std::endl;
	for (auto const& p : new_entries) {
		out << p.Stringize() << std::endl;
	}

	out.close();

	// □ (U+25A1)
	// 文字 (UTF16),表示前 (UTF16),X_Offset (u16),パラメータ (ASCII)
	std::cout << "Done (saved to " << out_str << ")!" << std::endl;
	std::cout << "Remember to add squares(U + 25A1) and japanese (always the same, on top)!" << std::endl;

	return EXIT_SUCCESS;
}

std::vector<std::string> ReadFile(std::string const& file_str) {

	std::vector<std::string> read_strings{};
	std::ifstream file(file_str, std::ios::in);
	if (!std::filesystem::exists(file_str)) {
		return {};
	}

	if (!file.is_open()) {
		// Shouldn't happen...?
		return {};
	}

	std::string temp_line{};

	while (std::getline(file, temp_line)) {
		char const zero_ch = 0;
		std::string const zero_str = std::string{ zero_ch };
#ifdef _WIN32
		temp_line = std::regex_replace(temp_line, std::regex(zero_str), "");
		temp_line = std::regex_replace(temp_line, std::regex("\""), "");
#else
		temp_line = std::regex_replace(temp_line, std::regex("\""), "");
		// ??? What? Commented out "for now"
		/*
		for (int j = 0; j < temp_line.length(); j++) {
			temp_line.erase(temp_line.begin() + j);
		}
		*/
#endif
		read_strings.push_back(temp_line);
	}

	file.close();

	return read_strings;
}

std::vector<DatEntry> ReadEntries(std::vector<std::string> const& read_strings) {

	std::vector<DatEntry> entries{};
	std::uint64_t cont = 0;	// Mostly for debug

	// Copy is intentional here although we don't care too much about read_strings after this point
	for (auto string : read_strings) {
		cont++;
		if (cont == 1) {
			// The first line contains japanese characters with info
			// but it's always the same, like a header
			continue;
		}

		if (string == "\0") {	// C null terminator... is it even possible?
			continue;
		}

		DatEntry entry{};

		entry.SetLetter(string[0]);	// Set "letter" as the first character from the string
		std::cout << "Letter: " << string[0] << std::endl;

		string = string.substr(1);
		string = string.substr(1);	// ,

		// Set as used even if it isn't, for convenience
		entry.SetIsActuallyUsed(DatEntry::GetActuallyUsedDefault());

		string = string.substr(2);
		string = string.substr(1);	// ,

		std::size_t const find_comma = string.find(",");
		if (find_comma == std::string::npos) {
			// Should never happen (?)
			continue;
		}

		std::string const offset_str = string.substr(0, find_comma);
		std::cout << "Offset_str: \"" << offset_str << "\"" << std::endl;
		std::uint64_t const offset = std::stoull(offset_str);
		entry.SetOffsetX(offset);

		string = string.substr(find_comma);
		string = string.substr(1); // ,

		std::string const argument = string;
		entry.SetExtraParam(argument);
		std::cout << "Param: \"" << argument << "\"" << std::endl;

		entries.push_back(entry);
		std::cout << std::endl;
	}

	return entries;
}

void ViewEntries(std::vector<DatEntry> const& entries) {

	std::cout << "Parameters:" << std::endl;
	std::cout << std::endl;
	std::cout << "(PUT JAPANESE IN MANUALLY)" << std::endl;
	for (auto const& p : entries) {
		std::cout << p.Stringize() << std::endl;
	}
}

std::vector<DatEntry> CreateEntries(std::vector<DatEntry> const& old_entries) {

	static constexpr bool random_rotates = true;	// Why would one disable it?

	std::string new_string{};
	std::cout << "Insert new string" << std::endl;
	std::getline(std::cin >> std::ws, new_string);	// cin >> ws is required for spaces
	std::vector<DatEntry> new_entries{};
	bool last_was_space = false;
	std::uint64_t distance = 0;
	bool first_dist = true;

	for (std::uint64_t j = 0; j < new_string.length(); j++) {

		// Also useful to avoid rotating a space
		if (new_string[j] == ' ') {
			last_was_space = true;
			continue;
		}

		DatEntry param;
		param.SetLetter(new_string[j]);

		param.SetIsActuallyUsed(DatEntry::GetActuallyUsedDefault());

		if (!first_dist) {
			if (last_was_space) {
				distance += DatEntry::GetSpaceDistance();
				last_was_space = false;
			}
			else {
				distance += DatEntry::GetNormalDistance();
			}
		}
		else {
			distance = 0;
			first_dist = false;
		}

		param.SetOffsetX(distance);

		// Just to be sure?
		// Also, this may lock you out from obtaining Monokuma... uhhh... thingies!
		// Pay attention!
		if (j < old_entries.size() && random_rotates) {
			param.SetExtraParam(old_entries[j].GetExtraParam());
		}
		else {
			param.SetExtraParam(DatEntry::GetExtraParamDefault());
		}

		new_entries.push_back(param);
	}

	return new_entries;
}
