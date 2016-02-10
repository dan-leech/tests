#include <fstream>
#include <iostream>
#include <string>

#define ALPHABET_SIZE 26

class Problem {
	struct node {
		bool endOfWord;
		int depth;
		node *children[ALPHABET_SIZE];

		node() {
			depth = 0;
			endOfWord = false;
			for (int i = 0; i < ALPHABET_SIZE; i++)
			{
				children[i] = NULL;
			}
		}
	} *root;

public:

	Problem() {
		root = new node();
	}

	bool loadDictionary(std::string file) {
		std::ifstream dictionaryFile(file);
		std::string word;
		
		if (dictionaryFile.is_open()) {
			int numLines = 0;
			try
			{
				dictionaryFile >> numLines;
				while (numLines > 0)
				{
					if (dictionaryFile >> word) {
						if (word.length() > 0) {
							node *current = root;
							for (unsigned int i = 0; i < word.length(); i++)
							{
								int letter = (int)word[i] - (int)'a';
								if (current->children[letter] == NULL) {
									current->children[letter] = new node();
									current->children[letter]->depth = current->depth + 1;
								}

								if (i == word.length() - 1)
									current->children[letter]->endOfWord = true;
								current = current->children[letter];
							}
						}
						numLines--;
					}
					else {
						dictionaryFile.close();
						return false;
					}
				}
			}
			catch (const std::exception&)
			{
				dictionaryFile.close();
				return false;
			}

			dictionaryFile.close();

			return true;
		}

		return false;
	}

	int findWords(std::string prefix, int maxLength) {
		int letter = 0;
		node *current = root;

		for (unsigned int i = 0; i < prefix.length(); i++)
		{
			letter = (int)prefix[i] - (int)'a';
			if (current->children[letter] == NULL)
				break;
			current = current->children[letter];
		}

		return checkChild(current, maxLength);
	}	
	
protected:
	int checkChild(node *leaf, int &maxDepth) {
		if (leaf == NULL) return 0;

		unsigned int i = 0;
		int numWords = 0;
		
		while (i < ALPHABET_SIZE && leaf->depth < maxDepth)
		{
			if (leaf->children[i] != NULL)
				numWords += checkChild(leaf->children[i], maxDepth);
			i++;
		} 
		
		if (leaf->endOfWord)
			numWords++;

		return numWords;
	}

};



int main(int argc, char *argv[])
{
	Problem *solve = new Problem();
	std::string file;

	switch (argc) {
	case 1:
		std::cout << "enter filename\n";
		std::cin >> file;

	case 2:
		if(argc == 2)
			file = argv[1];

		std::cout << "loading dictionary\n";
		if (solve->loadDictionary(file))
			std::cout << "dictionary loaded\n";
		else
			std::cout << "wrong file name or file corupted\n";

		break;
	default:
		std::cout << "wrong parameters!\n";
		break;
	}

	std::cout << "Enter \"\\q\" for exit.\n";
	std::string line, word, number;
	int separatorPos = 0;
	while (std::cin >> line && line != "\\q") {
		separatorPos = line.find_first_of(',');
		if (separatorPos == 0) {
			std::cout << "Correct input is \"word,number\"\n";
			continue;
		}
		word = line.substr(0, separatorPos);
		number = line.substr(separatorPos + 1);
		std::cout << solve->findWords(word, std::stoi(number)) << "\n";
	}


    return 0;
}

