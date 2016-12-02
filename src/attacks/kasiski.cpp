#include "main.h"

/* example constructor, sets some options */
Kasiski::Kasiski()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "/tmp/kasiskiresults.txt");
}

/* I am overriding the default module function
 * for displaying the description text
 */
void Kasiski::disp_desc()
{
    cout << "Module: attacks/kasiski\n\tAttempts to decrypt a ciphertext sample encrypted with\n\tthe repeated key Vigenere cipher.\n\tWill return the decrypted file and key if successful.\n\tPlease define INPUTFILE for this example module to run correctly.\n\tOptions are given below:\n" << endl;
    disp_opts();
    cout << endl;
}

/* overrides the virtual function from Module
 * this is where the real meaty stuff happens
 */
int Kasiski::run()
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