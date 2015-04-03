#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <sstream>

#include "morse.h"

int Dictionary::max(int a, int b) {
	return a-((a-b)&((a-b)>>(sizeof(int)*CHAR_BIT-1)));
}

void Dictionary::deleteTree(DictionaryNode* node) {
	if (node == NULL) {
		return;
	}
	deleteTree(node->dot);
	deleteTree(node->dash);
	delete node;
}

void Dictionary::build(std::string filename) {
	build(std::ifstream(filename));
}

void Dictionary::build(std::istream& in) {
	deleteTree(root);
	root = new DictionaryNode();
	std::string line;
	// loop processing lines
	while (std::getline(in, line)) {
		std::stringstream lineStream(line); // use line as a stream
		char symbol = toupper(lineStream.get()); // case insensitive
		lineStream.get(); // space
		DictionaryNode* currentNode = root;
		char direction;
		int codeWordLength;
		// loop processing codeword characters
		for (codeWordLength = 0; (direction = lineStream.get()) >= 0; ++codeWordLength) {
			DictionaryNode** target = NULL;
			switch (direction) {
				case '.' : target = &currentNode->dot;  break;
				case '-' : target = &currentNode->dash; break;
			}
			if (*target == NULL) *target = new DictionaryNode();
			currentNode = *target;
		}
		currentNode->c = symbol;
		depth = max(depth, codeWordLength);
	}
}

/**
 * Return value indicates match.
 * (justifiable backtracking, it is)
 */
bool Dictionary::search(DictionaryNode* node, char c, std::string &code) {
    if (node == NULL) return false;
    if (node->c == c) return true;
	// guess for 'dot direction'
	code.push_back('.');
    if (search(node->dot, c, code)) {
		return true; // match
    } else {
		code.erase(code.length()-1); // pop_back
    }
	// guess for 'dash direction'
	code.push_back('-');
    if (search(node->dash, c, code)) {
        return 1;
    } else {
		code.erase(code.length()-1); // pop_back
	}
	// no match in either direction
    return false;
}

/**
 * Wrapper able to reserve and return.
 */
std::string Dictionary::search(char c) {
	std::string code;
	code.reserve(depth);
	search(root, c, code);
	return code;
}

void Dictionary::encode(std::istream& in, std::ostream& out) {
	std::string line;
	while (std::getline(in, line)) {
		for (unsigned i=0; i<line.length(); ++i) {
			switch (line[i]) {
				case ' ': out << '\n'; break;
				default : out << search(toupper(line[i])) << ' '; break;
			}
		}
		// encode newline the same way as a space
		out << '\n';
	}
}

void Dictionary::decode(std::istream& in, std::ostream& out) {
	std::string line;
	// one line, originally one word, probably many codewords (characters)
	while (std::getline(in, line)) {
		std::stringstream lineStream(line);
		std::string codeword;
		// one codeword per iteration
		while (lineStream >> codeword) {
			DictionaryNode* currentNode = root;
			// process codeword characters (binary search)
			for (unsigned i=0; i<codeword.length(); ++i) {
				switch (codeword[i]) {
					case '.': currentNode = currentNode->dot;  break;
					case '-': currentNode = currentNode->dash; break;
					default : /* Invalid code */ break;
				}
			}
			// output decoded character
			out << currentNode->c;
		}
		// separate decoded words with spaces
		out << ' ';
	}
}