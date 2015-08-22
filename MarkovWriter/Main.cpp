#include <iostream>
#include <fstream>

#include <string>
#include <map>
#include <vector>
#include <deque>
#include <ctime>
using namespace std;

const int PREFIX_LENGTH = 2; //Number of consecutive prefixes to decide suffix. [1 too random, 3 needs greater input than navigilabs]
const char NONWORD[] = "\n"; //For matching with single-word prefixes.
const int MAXWORDS= 10000; // maximum words generated, failsafe for infinite word-loops

typedef deque<string> Prefix; //Similar to vector, but constant-time add/removal at beginning
map<Prefix, vector<string> > dictionary; //Table to hold states based on prefix

// Reads in whole input file, and puts it into memory dictionary
void BuildDictionary(Prefix& prefix, istream& in);
// Adds the entry into dictionary, and shifts the prefix by one element (to get all posible prefixes without much hassle)
void AddEntry(Prefix& prefix, const string& s);
//Generates the result doc, starts with the first words of the input document, then random takes over.
void GenerateText(int nwords, ofstream& outFile);
/*TODO:
	1.Dict storage
	2.Multiple input files
	3.User interface to allow re-generating w/o reopening program
*/

int main(void)
{
	ifstream inFile;
	inFile.open("in.txt");
	ofstream outFile;
	outFile.open("out.txt");

	int wordCount = MAXWORDS;
	Prefix prefix;
	for (int i = 0; i < PREFIX_LENGTH; i++){//Adding nonwords at beginning of dictionary(not to lose first entries)
		AddEntry(prefix, NONWORD);
	}
		 
	clock_t beginTime = clock();
	cout << "Bulding dict, be patient :) " << endl;
	BuildDictionary(prefix, inFile); //Read in and generate state table
	clock_t endTime = clock();

	
	double elapsed_secs = double(endTime - beginTime) / CLOCKS_PER_SEC;
	cout << "Dict built! It took " << elapsed_secs << " seconds" << endl;
	AddEntry(prefix, NONWORD); //Adding nonword at the end to keep the last entry.

	// initialize random seed
	srand(time(NULL));
	GenerateText(wordCount, outFile); 
	outFile << endl << "-----------------------------------------" << endl;

	inFile.close();
	outFile.close();

	cout << "Output generated! Input something and press enter to close" << endl;
	int a;
	cin >> a;
	return 0;
}


void BuildDictionary(Prefix& prefix, istream& in)
{
	string buf;
	while (in >> buf){
		AddEntry(prefix, buf);
	}
}


void AddEntry(Prefix& prefix, const string& suffix)
{
	if (prefix.size() == PREFIX_LENGTH) {
		dictionary[prefix].push_back(suffix);
		prefix.pop_front();
	}
	prefix.push_back(suffix);
}


void GenerateText(int wordCount, ofstream& outFile)
{
	Prefix prefix;
	int i;
	for (i = 0; i < PREFIX_LENGTH; i++){
		AddEntry(prefix, NONWORD);
	}
		
	for (i = 0; i < wordCount; i++) {
		vector<string>& suffix = dictionary[prefix]; 
		const string& word = suffix[rand() % suffix.size()];
		if (word == NONWORD){
			break;
		}
			
		outFile << word << " ";
		//shift the prefix by a word, to keep generating.
		prefix.pop_front();
		prefix.push_back(word);
	}
}