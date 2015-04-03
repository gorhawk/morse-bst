#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "morse.h"

void print_help() {
	std::cout
		<< "Usage:" << '\n'
		<< "    morse [OPTION] [DICTIONARY] [INFILE] [OUTFILE]" << '\n'
		<< "Options:" << '\n'
		<< "    -e, --encode    Encodes INFILE into OUTFILE." << '\n'
		<< "                    Default behavior." << '\n'
		<< "    -d, --decode    Decodes INFILE into OUTFILE." << '\n'
		<< "    -o, --output    Accept one filename as OUTFILE, read from standard input." << '\n'
		<< "                    Normally, if OUTFILE is omitted, the program writes to standard output." << '\n'
		<< "    -s, --standard  Use the ITU standardized, International Morse Code." << "\n"
		<< "                    With this option, DICTIONARY is not needed." << "\n"
		<< "    -h, --help      Display this help and exit." << std::endl;
}

void print_argument_error() {
	std::cout << "Wrong arguments." << std::endl;
}

std::string standard_dictionary() {
	// International Morse Code (ITU standard)
	return
		std::string("A .-\nB -...\nC -.-.\nD -..\nE .\nF ..-.\nG --.\nH ....\nI ..\n") +
		std::string("J .---\nK -.-\nL .-..\nM --\nN -.\nO ---\nP .--.\nQ --.-\nR .-.\n") +
		std::string("S ...\nT -\nU ..-\nV ...-\nW .--\nX -..-\nY -.--\nZ --..\n0 -----\n") +
		std::string("1 .----\n2 ..---\n3 ...--\n4 ....-\n5 .....\n6 -....\n7 --...\n") +
		std::string("8 ---..\n9 ----.\n. .-.-.-\n, --..--\n: ---...\n- -....-\n' .----.\n") +
		std::string("/ -..-.\n? ..--..")
	;
}

int main(int argc, char* argv[]) {

	// no argument
	if (argc == 1) {
		print_help();
		return 0;
	}

	bool encoding = true;
	bool use_stdin = false;
	bool custom_dictionary = true;
	std::string dictionary_name, infile_name, outfile_name;

	// parse arguments
	for(int i=1; i<argc; ++i) {
		std::string argument = argv[i];
		if (argument == "-h" || argument == "--help") {
			if (i != 1) {
				print_argument_error();
			}
			print_help();
			return 0;
		}
		if (argument == "-e" || argument == "--encode") continue;
		if (argument == "-d" || argument == "--decode") {
			encoding = false;
			continue;
		}
		if (argument == "-o" || argument == "--output") {
			use_stdin = true;
			continue;
		}
		if (argument == "-s" || argument == "--standard") {
			custom_dictionary = false;
			continue;
		}
		if (argument.find("-")) {
			if (custom_dictionary && dictionary_name.empty()) {
				dictionary_name = argument;
				continue;
			}
			if (!use_stdin && infile_name.empty()) {
				infile_name = argument;
				continue;
			}
			if (outfile_name.empty()) {
				outfile_name = argument;
				continue;
			}
		}
		print_argument_error();
		print_help();
		return 0;
	}

	Dictionary dictionary;
	if (dictionary_name.empty()) {
		dictionary.build(std::istringstream(standard_dictionary()));
	} else {
		std::ifstream in(infile_name);
		if (!in.is_open()) {
			std::cout << "Cannot open dictionary file." << std::endl;
			return -1;
		}
		dictionary.build(in);
	}

	std::istream *in;
	std::ifstream input_file;
	if (!infile_name.empty()) {
		input_file.open(infile_name);
		if (!input_file.is_open()) {
			std::cout << "Cannot open input file." << std::endl;
			return -1;
		}
		in = &input_file;
	} else {
		in = &std::cin;
	}

	std::ostream *out;
	std::ofstream output_file;
	if (!outfile_name.empty()) {
		output_file.open(outfile_name);
		if (!output_file.is_open()) {
			std::cout << "Cannot open output file." << std::endl;
			return -1;
		}
		out = &output_file;
	} else {
		out = &std::cout;
	}

	if (encoding) {
		dictionary.encode(*in, *out);
	} else {
		dictionary.decode(*in, *out);
	}

	return 0;
}