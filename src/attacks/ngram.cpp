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
	temp.push_back("MIN_OCCURENCES");
    set_opts(temp);

    // set default values, option must exist or error will printed
	set_opt_value("INPUTFILE", "makefile");
    set_opt_value("OUTPUTFILE", "ngram");
	set_opt_value("N-SIZE", "2");
	set_opt_value("CHARSETSIZE", "256");
	set_opt_value("MIN_OCCURENCES", "20");
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
	cout << "Module: attacks/ngram\n\tThis module aids in the generation of N-gram counts to identify likely.\n\tOutput is generated in a CSV format of \"VALUE,OCCURENCES;\"\n\tThe N-SIZE option is used to determine the number of contiguous letters to consider. Space requirement grows quadratically in the n-gram length.\n\tThe CHARSETSIZE option is used to set the maximum range of the n-gram analysis.\n\t" << endl;
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
	
	//process_output(&out, &charCounts, &charTotal, &maxLength);

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
	int bufferIndex = 0, blockIndex = 0, charIndex;
	vector<int> charBuffer(nSize);
	while (!in->eof()) {
		in->get(inChar);
		charIndex = ((inChar < 0) ? (charSetSize + (int)inChar) : (int)inChar);
		if (inChar < 0 || inChar > charSetSize) {
			cout << "[-] Character set size option is smaller than actual size." << endl;
			return 4;
		}

		charBuffer[bufferIndex] = inChar;
		for (int i = 0; i < nSize; i++) {
			cout << (char)charBuffer[(bufferIndex + (i + 1)) % nSize];
		}
		cout << endl;

		//(*charCounts)[]


		bufferIndex = ((bufferIndex + 1) % nSize);


		//(*charCounts)[blockIndex][charIndex]++;
		//blockIndex = (blockIndex + 1) % nSize;
	}

	return 0;
}

int Ngram::analyze_input(vector< vector<int> >* charCounts, vector<int>* charTotal, vector<int>* maxLength, vector<int>* maxCount) {
	cout << "[*] Analyzing input..." << endl;

	for (unsigned int blockIndex = 0; blockIndex < charCounts->size(); blockIndex++) {
		for (unsigned int charIndex = 0; charIndex < (*charCounts)[blockIndex].size(); charIndex++) {
			int temp = (*charCounts)[blockIndex][charIndex];
			(*charTotal)[blockIndex] += temp;
			if (temp > (*maxCount)[blockIndex]) (*maxCount)[blockIndex] = temp;
		}

		int maxCtCopy = (*maxCount)[blockIndex];
		while (maxCtCopy /= 10) (*maxLength)[blockIndex]++;
	}

	return 0;
}

int Ngram::process_output(ofstream* out, vector< vector<int> >* charCounts, vector<int>* charTotal, vector<int>* maxLength) {
	vector<int> charIndexRef;
	charIndexRef.resize((*charCounts)[0].size());
	for (unsigned int charIndex = 0; charIndex < charIndexRef.size(); charIndex++) {
		charIndexRef[charIndex] = charIndex;
	}

	cout << "[*] Opening file: " << options["OUTPUTFILE"] << endl;
	out->open(options["OUTPUTFILE"]);

	cout << "[*] Writing output..." << endl;

	for (unsigned int blockIndex = 0; blockIndex < (*charCounts).size(); blockIndex++) {
		vector<int> charIndexTemp = charIndexRef;
		if (stoi(options["SORTED"]) != 0) {
			sort_arrays(&((*charCounts)[blockIndex]), &charIndexTemp);
		}

		(*out) << "Ngram for block index " << blockIndex << ": " << endl;
		
		for (unsigned int charIndex = 0; charIndex < (*charCounts)[blockIndex].size(); charIndex++) {
			if (stoi(options["SHOWFREQUENCY"]) == 0) {
				int numSpaces = 0, temp = (*charCounts)[blockIndex][charIndex];
				while (temp /= 10) numSpaces++;
				numSpaces = (*maxLength)[blockIndex] - numSpaces;
				(*out) << charIndexTemp[charIndex] << "," << string(numSpaces + 1, ' ') << (*charCounts)[blockIndex][charIndex] << "; \n";
			}
			else {
				float percent = ((float)(*charCounts)[blockIndex][charIndex] / (float)(*charTotal)[blockIndex]) * 100;
				(*out) << fixed << charIndexTemp[charIndex] << ", "<< percent << "; \n";
			}
		}
	}

	return 0;
}

int Ngram::sort_arrays(vector<int>* charCounts, vector<int>* charIndexArray) {
	for (unsigned int mainIndex = 0; mainIndex < (*charCounts).size(); mainIndex++) {
		int countValue = (*charCounts)[mainIndex],
			countIndex = (*charIndexArray)[mainIndex],
			swapIndex = mainIndex - 1;

		while (swapIndex >= 0 && (*charCounts)[swapIndex] < countValue) {
			(*charCounts)[swapIndex + 1] = (*charCounts)[swapIndex];
			(*charIndexArray)[swapIndex + 1] = (*charIndexArray)[swapIndex];
			swapIndex--;
		}

		(*charCounts)[swapIndex + 1] = countValue;
		(*charIndexArray)[swapIndex + 1] = countIndex;
	}

	return 0;
}
