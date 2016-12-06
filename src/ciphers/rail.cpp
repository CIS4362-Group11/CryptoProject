#include "main.h"

/* example constructor, sets some options */
Rail::Rail()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
    temp.push_back("LENGTH");
    temp.push_back("DECRYPT");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "encr_rail.txt");
    set_opt_value("DECRYPT", "0");
	set_opt_value("LENGTH", "5");
	// delete before merge
	set_opt_value("INPUTFILE", "makefile");
}

/* I am overriding the default module function
 * for displaying the description text
 */
void Rail::disp_desc()
{
    cout << "Module: ciphers/rail\n\tEncrypts (or decrypts, if length known) a text file with the rail fence cipher.\n\tPlease define the following required options:\n\t\tINPUTFILE\tinput filename\n\t\tLENGTH\t\tnumber of rails" << endl;
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
	bool decrypt = false;

	// perform error checking on options first
	if (options["INPUTFILE"].empty()) {
		cout << "[-] Please specify an input file" << endl;
		return 1;
	}

	if (options["OUTPUTFILE"].empty()) {
		cout << "[-] Please specify an output file" << endl;
		return 2;
	}

	if (options["LENGTH"].empty()) {
		cout << "[-] Please specify a length" << endl;
		return 3;
	}

	if (options["DECRYPT"] == "1"){
		decrypt = true;
	}

    cout << "[*] Opening input file: " << options["INPUTFILE"] << endl;
    in.open(options["INPUTFILE"]);

    if (!in.good()) {
        cout << "[-] Input file error" << endl;
        return 5;
    }

    cout << "[*] Opening output file: " << options["OUTPUTFILE"] << endl;
    out.open(options["OUTPUTFILE"]);

    if (!decrypt) cout << "[*] Encrypting..." << endl;
    else cout << "[*] Decrypting..." << endl;

	encrypt(&in, &out, stoi(options["LENGTH"]));

    cout << "[*] Closing files" << endl;
    in.close();
    out.close();

    return 0;
}

void Rail::encrypt(ifstream* in, ostream* out, int length)
{
	vector< vector<char> > rails;
	rails.resize(length);
	char inChar;
	int currRail = 0;
	bool ascend = true;
	while (in->get(inChar)) {
		cout << inChar;
		rails[currRail].push_back(inChar);
		if (ascend) { 
			currRail++;
			if (currRail == length-1)
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
		while (currRail >= 0) {
			rails[currRail].push_back('\n');
			if (currRail > 0) currRail--;
		}
	}
	while (currRail < length) {
		rails[currRail].push_back('\n');
		currRail++;
	}

	for (vector<char> vec : rails) {
		for (char output : vec) {
			(*out) << output;
		}
	}
}
