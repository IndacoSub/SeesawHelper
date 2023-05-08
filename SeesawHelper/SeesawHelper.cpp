#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <filesystem>

#include "DatEntry.h"

std::vector<std::string> ReadFile(std::string const& file_str);
std::size_t FindFirstNumeric(std::string const& str);
std::size_t FindLastNumeric(std::uint64_t const& since, std::string const& str);
std::vector<DatEntry> ReadEntries(std::vector<std::string> const& read_strings);
void ViewEntries(std::vector<DatEntry> const& entries);
std::vector<DatEntry> CreateEntries(std::vector<DatEntry> const& old_entries);

int main(int argc, char* argv[]) {

	if (argc < 1) {
		std::cout << "Usage: SeesawHelper CSV_FILE_DUMPED_FROM_MINIGAME" << std::endl;
		return EXIT_FAILURE;
	}

	if (argv[1] == nullptr) {
		std::cout << "Usage: SeesawHelper CSV_FILE_DUMPED_FROM_MINIGAME" << std::endl;
		return EXIT_FAILURE;
	}

	std::string const file_str = argv[1];

	std::vector<std::string> const read_strings = ReadFile(file_str);

	if (read_strings.size() <= 0) {
		// Should not happen(?)
		return EXIT_FAILURE;
	}

	std::vector<DatEntry> const entries = ReadEntries(read_strings);

	ViewEntries(entries);

	std::vector<DatEntry> const new_entries = CreateEntries(entries);

	std::string const out_str = "out.txt";
	std::ofstream out(out_str, std::ios::out);
	if (std::filesystem::exists(out_str)) {
		out << "" << std::endl;
	}

	for (auto const& p : new_entries) {
		out << p.Stringize() << std::endl;
	}

	out.close();

	// □ (U+25A1)
	// 文字 (UTF16),表示前 (UTF16),X_Offset (u16),パラメータ (ASCII)
	std::cout << "Done (saved to out.txt)!" << std::endl;
	std::cout << "Remember to add squares(U + 25A1) and japanese (on top)!" << std::endl;
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
		char zero_ch = 0;
		std::string zero_str = std::string{ zero_ch };
#ifdef _WIN32
		temp_line = std::regex_replace(temp_line, std::regex(zero_str), "");
#else
		for (int j = 0; j < temp_line.length(); j++) {
			temp_line.erase(temp_line.begin() + j);
		}
#endif
		read_strings.push_back(temp_line);
	}

	file.close();

	return read_strings;
}

std::size_t FindFirstNumeric(std::string const& str) {

	return str.find_first_of("0123456789");
}

std::size_t FindLastNumeric(std::uint64_t const& since, std::string const& str) {

	return str.find_last_of("0123456789", since);
}

std::vector<DatEntry> ReadEntries(std::vector<std::string> const& read_strings) {

	std::vector<DatEntry> entries{};
	std::uint64_t cont = 0;
	for (auto& string : read_strings) {
		cont++;
		if (cont == 1) {
			// The first line contains japanese characters
			// so let's just skip them as they don't seem to contain
			// any useful info
			continue;
		}

		if (string == "\0") {	// C null terminator... is it even possible?
			continue;
		}

		std::cout << "Full string □: \"" << string << "\"" << std::endl;

		DatEntry entry{};

		entry.SetLetter(string[0]);	// Set "letter" as the first character from the string... Why though?
		std::cout << "Letter: " << string[0] << std::endl;

		entry.SetUnknown(DatEntry::GetUnknownDefault());

		std::uint64_t const find_first_number = FindFirstNumeric(string);
		std::uint64_t const find_last_number = FindLastNumeric(find_first_number, string);
		std::string const offset_str = string.substr(find_first_number, find_last_number - find_first_number);
		std::cout << "Offset_str: \"" << offset_str << "\"" << std::endl;
		std::uint64_t const offset = std::stoull(offset_str);
		entry.SetOffsetX(offset);

		std::string const argument = string.substr(find_last_number + 1);
		entry.SetExtraParam(argument);
		std::cout << "Param: \"" << argument << "\"" << std::endl;

		entries.push_back(entry);
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

	static constexpr bool random_rotates = false;

	std::string new_string{};
	std::cout << "Insert new string" << std::endl;
	std::getline(std::cin >> std::ws, new_string);
	std::vector<DatEntry> new_entries{};
	bool last_was_space = false;
	std::uint64_t distance = 0;
	bool first_dist = true;
	for (std::uint64_t j = 0; j < new_string.length(); j++) {

		if (new_string[j] == ' ') {
			last_was_space = true;
			continue;
		}

		DatEntry param;
		param.SetLetter(new_string[j]);

		param.SetUnknown(DatEntry::GetUnknownDefault());

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
