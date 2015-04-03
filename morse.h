#include <string>
#include <fstream>

struct DictionaryNode {
	char c;
	DictionaryNode* dot;
	DictionaryNode* dash;
	DictionaryNode() {
		c = 0;
		dot = NULL;
		dash = NULL;
	}
};

class Dictionary {
private: // attributes
	DictionaryNode* root;
	int depth; // limits codeword length
private: // methods
	void initialize() {
		root = NULL;
		depth = NULL;
	}
	void deleteTree(DictionaryNode* node);
	bool search(DictionaryNode* node, char c, std::string &code);
public:
	Dictionary() {
		initialize();
	}
	Dictionary(std::string filename) {
		initialize();
		build(filename);
	}
	void build(std::string filename);
	void build(std::istream& in);
	std::string search(char c);
	void encode(std::istream& in, std::ostream& out);
	void encode(std::string inputFilename, std::string outputFilename) {
		encode(std::ifstream(inputFilename), std::ofstream(outputFilename));
	}
	void decode(std::istream& in, std::ostream& out);
	void decode(std::string inputFilename, std::string outputFilename) {
		decode(std::ifstream(inputFilename), std::ofstream(outputFilename));
	}
	~Dictionary() {
		deleteTree(root);
	}
	static int max(int a, int b);
};