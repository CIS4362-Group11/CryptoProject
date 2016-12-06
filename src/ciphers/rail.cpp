#include "main.h"

/* example constructor, sets some options */
Rail::Rail()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
    temp.push_back("NUMRAILS");
    temp.push_back("DECRYPT");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "encr_rail.txt");
    set_opt_value("DECRYPT", "0");
	set_opt_value("NUMRAILS", "5");
}

/* I am overriding the default module function
 * for displaying the description text
 */
void Rail::disp_desc()
{
    cout << "Module: ciphers/rail\n\tEncrypts (or decrypts, if number of rails known) a text file with the rail fence cipher.\n\tPlease define the following required options:\n\t\tINPUTFILE\tinput filename" << endl;
    cout << endl;
}

/* overrides the virtual function from Module
 * this is where the real meaty stuff happens
 */
int Rail::run()
{
	ifstream in;
	ofstream out;
	string ibuff, obuff;
	bool shouldDecrypt = false;

	// perform error checking on options first
	if (options["INPUTFILE"].empty()) {
		cout << "[-] Please specify an input file" << endl;
		return 1;
	}

	if (options["OUTPUTFILE"].empty()) {
		cout << "[-] Please specify an output file" << endl;
		return 2;
	}

	if (options["NUMRAILS"].empty()) {
		cout << "[-] Please specify the number of rails" << endl;
		return 3;
	}

	if (options["DECRYPT"] == "1"){
		shouldDecrypt = true;
	}

    cout << "[*] Opening input file: " << options["INPUTFILE"] << endl;
    in.open(options["INPUTFILE"]);

    if (!in.good()) {
        cout << "[-] Input file error" << endl;
        return 5;
    }

    cout << "[*] Opening output file: " << options["OUTPUTFILE"] << endl;
    out.open(options["OUTPUTFILE"]);

	if (!shouldDecrypt) {
		cout << "[*] Encrypting..." << endl;
		encrypt(&in, &out, stoi(options["NUMRAILS"]));
	}
	else {
		cout << "[*] Decrypting..." << endl;
		decrypt(&in, &out, stoi(options["NUMRAILS"]));
	}

    cout << "[*] Closing files" << endl;
    in.close();
    out.close();

    return 0;
}

void Rail::encrypt(ifstream* in, ostream* out, int numRails)
{
	vector< vector<char> > rails;
	rails.resize(numRails);
	char inChar;
	int currRail = 0;
	bool ascend = true;
	while (in->get(inChar)) {
		rails[currRail].push_back(inChar);
		if (ascend) { 
			currRail++;
			if (currRail == numRails-1)
				ascend = false;
		}
		else {
			currRail--;
			if (currRail == 0) 
				ascend = true;
		}
	}

	//Just pad for easier decryption honestly
	if (!ascend) {
		while (currRail > 0) {
			rails[currRail].push_back('\n');
			if (currRail > 0) currRail--;
		}
	}
	while (currRail < numRails) {
		rails[currRail].push_back('\n');
		currRail++;
	}

	for (vector<char> vec : rails) {
		for (char output : vec) {
			(*out) << output;
		}
	}
}


void Rail::decrypt(ifstream* in, ostream* out, int numRails)
{
	vector< vector<char> > rails;
	rails.resize(numRails);
	char inChar;

	string inLine;
	int length = 0;
	while (getline(*in, inLine)) {
		length += inLine.length();
	}
	(*in).clear();
	(*in).seekg(0, ios::beg);

	int numOnEdges = length / ((numRails * 2) - 2);
	int numInside = numOnEdges * 2 - 1;

	for (int numInLine = 0; numInLine < numOnEdges; numInLine++) {
		if (in->get(inChar)) {
			rails[0].push_back(inChar);
		}
	}
	for (int rail = 1; rail < numRails - 1; rail++) {
		for (int numInLine = 0; numInLine < numInside; numInLine++) {
			if (in->get(inChar)) {
				rails[rail].push_back(inChar);
			}
		}
	}
	for (int numInLine = 0; numInLine < numOnEdges; numInLine++) {
		if (in->get(inChar)) {
			rails[numRails-1].push_back(inChar);
		}
	}

	int edgeIndex = 0;
	int middleIndex = 0;
	int railIndex = 0;
	bool ascend = true;

	while (edgeIndex < numOnEdges) {
		if (railIndex == 0 || railIndex == numRails - 1) {
			(*out) << rails[edgeIndex][railIndex];
		}
		else {
			(*out) << rails[middleIndex][railIndex];
		}

		if (ascend) {
			railIndex++;
			if (railIndex == numRails - 1) {
				ascend = false;
				middleIndex++;
			}
		}
		else {
			railIndex--;
			if (railIndex == 0) {
				ascend = true;
				edgeIndex++;
				middleIndex++;
			}
		}
	}
}