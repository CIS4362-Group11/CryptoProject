#include "main.h"

/* example constructor, sets some options */
Histogram::Histogram()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
	temp.push_back("BLOCKSIZE");
	temp.push_back("CHARSETSIZE");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "/tmp/histogram");
	set_opt_value("BLOCKSIZE", "1");
	set_opt_value("CHARSETSIZE", "256");
}

/* helper function that doesn't really do anything */
void Histogram::test()
{
    cout << "example test" << endl;
}

/* I am overriding the default module function
 * for displaying the description text
 */
void Histogram::disp_desc()
{
	cout << "Module: attacks/histogram\n\tThis module aids in the generation of character distribution histograms.\n\tOutput is generated in a CSV format of \"VALUE,OCCURENCES;\"\n\tThe BLOCKSIZE option is used to generate a set of histograms, one for each character in a block.\n\tThe CHARSETSIZE option is used to set the maximum range of the histogram(s).\n\t" << endl;
    disp_opts();
    cout << endl;
}

/* overrides the virtual function from Module
 * this is where the real meaty stuff happens
 */
int Histogram::run()
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

    ifstream in;
    ofstream out;
    string buff;

    cout << "[*] Opening file: " << options["INPUTFILE"] << endl;
    in.open(options["INPUTFILE"]);

    cout << "[*] Opening file: " << options["OUTPUTFILE"] << endl;
    out.open(options["OUTPUTFILE"]);

    cout << "[*] Writing..." << endl;
    while (!in.eof()) {
        getline(in, buff);
        out << buff << endl;
    }

    cout << "[*] Closing files" << endl;
    in.close();
    out.close();

    return 0;
}
