#include "main.h"

/* example constructor, sets some options */
Ngram::Ngram()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
	temp.push_back("N-SIZE");
	temp.push_back("CHARSETSIZE");
	temp.push_back("MINOCCURENCES");
	temp.push_back("OUTPUTHEX");
    set_opts(temp);

    // set default values, option must exist or error will printed
	set_opt_value("INPUTFILE", "makefile");
    set_opt_value("OUTPUTFILE", "ngram");
	set_opt_value("N-SIZE", "2");
	set_opt_value("CHARSETSIZE", "256");
	set_opt_value("MINOCCURENCES", "2");
	set_opt_value("OUTPUTHEX", "0");
}

/* helper function that doesn't really do anything */
void Ngram::test()
{
    cout << "example test" << endl;
}

/* I am overriding the default module function
 * for displaying the description text
 */
void Ngram::disp_desc()
{
	cout << "Module: attacks/ngram\n\tThis module aids in the generation of N-gram counts to identify likely.\n\tOutput is generated in a CSV format of \"VALUE,OCCURENCES;\"\n\tThe N-SIZE option is used to determine the number of contiguous letters to consider.\n\tThe CHARSETSIZE option is used to set the maximum range of the n-gram analysis.\n\t" << endl;
    disp_opts();
    cout << endl;
}

int Ngram::run()
{
    // perform error checking on options first
    if (options["INPUTFILE"].empty()) {
        cout << "[-] Please specify an input file" << endl;
        return 1;
    }

    if (options["OUTPUTFILE"].empty()) {
        cout << "[-] Please specify an output file" << endl;
        return 2;
    }

	const int nSize = stoi(options["N-SIZE"]);
	const int charSetSize = stoi(options["CHARSETSIZE"]);

	if (nSize < 1){
		cout << "[-] N-SIZE should be greater than 0" << endl;
		return 3;
	}

	vector<int> charCounts;
	charCounts.resize(pow(charSetSize, nSize));

	ifstream in;
    ofstream out;
    string buff;

	if (process_input(&in, &charCounts, nSize, charSetSize) != 0) {
		return 4;
	}

	//vector<int> charTotal(nSize), maxLength(nSize), maxCount(nSize);
	//analyze_input(&charCounts, &charTotal, &maxLength, &maxCount);
	
	process_output(&out, &charCounts, charSetSize);

    cout << "[*] Closing files" << endl;
    in.close();
    out.close();

    return 0;
}

int Ngram::process_input(ifstream* in, vector< int >* charCounts, const int nSize, const int charSetSize) {
	cout << "[*] Opening file: " << options["INPUTFILE"] << endl;
	in->open(options["INPUTFILE"]);


	cout << "[*] Processing input file..." << endl;
	char inChar;
	int bufferIndex = 0, charIndex;
	vector<int> charBuffer(nSize);
	while (!in->eof()) {
		in->get(inChar);
		charIndex = ((inChar < 0) ? (charSetSize + (int)inChar) : (int)inChar);
		if (charIndex < 0 || charIndex > charSetSize) {
			cout << "[-] Character set size option is smaller than actual size." << endl;
			return 4;
		}

		charBuffer[bufferIndex] = inChar;
		int arrayIndex = 0;
		for (int i = 0; i < nSize; i++) {
			arrayIndex += charBuffer[(bufferIndex + (i + 1)) % nSize] * pow(charSetSize, nSize - (i + 1));
		}

		(*charCounts)[arrayIndex]++;
		bufferIndex = ((bufferIndex + 1) % nSize);
	}

	return 0;
}

int Ngram::process_output(ofstream* out, vector<int>* charCounts, const int charSetSize) {

	cout << "[*] Opening file: " << options["OUTPUTFILE"] << endl;
	out->open(options["OUTPUTFILE"]);

	cout << "[*] Writing output..." << endl;

	for (unsigned int charIndex = 0; charIndex < (*charCounts).size(); charIndex++) {
		if ((*charCounts)[charIndex] >= stoi(options["MINOCCURENCES"])) {
			string ngram = "";
			int charValue = 0, charIndexCopy = charIndex;
			while (charIndexCopy > 0) {
				charValue = charIndexCopy % charSetSize;
				charIndexCopy /= charSetSize;
				if (stoi(options["OUTPUTHEX"]) == 0) {
					ngram = to_string(charValue) + ngram;
				}
				else {
					ngram = conv_int_to_hex(charValue) + ngram;
				}
			}
			(*out) << ngram << "," << (*charCounts)[charIndex] << endl;
		}
	}

	return 0;
}

string Ngram::conv_int_to_hex(int input) {
	string hexOut = (input <= 16) ? "0" : "";
	while (input > 0) {
		int indexVal = input % 16;
		if (indexVal > 9) {
			hexOut += (char)('A' + (indexVal - 10));
		}
		else {
			hexOut = to_string(indexVal) + hexOut;
		}
		input /= 16;
	}
	return hexOut;
}