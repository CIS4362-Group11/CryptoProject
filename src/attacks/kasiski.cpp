#include "main.h"

/* example constructor, sets some options */
Kasiski::Kasiski()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
    temp.push_back("ASSUMEYES");
    temp.push_back("MAXBUFF");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "/tmp/kasiskiresults.txt");
    set_opt_value("ASSUMEYES", "0");
    set_opt_value("MAXBUFF", "8192");
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

    if (options["ASSUMEYES"].empty() || stoi(options["ASSUMEYES"]) < 0 || stoi(options["ASSUMEYES"]) > 1) {
        cout << "[-] Please specify an ASSUMEYES value of 0 or 1" << endl;
        return 3;
    }

    if (options["MAXBUFF"].empty() || stoi(options["MAXBUFF"]) <= 0) {
        cout << "[-] Option MAXBUFF must be greater than 0" << endl;
        return 4;
    }

    cout << "[*] Beginning attack..." << endl;
    begin_attack();

    return 0;
}

/* implements the kasiski attack */
void Kasiski::begin_attack()
{

}
