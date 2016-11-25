#include "main.h"

/* example constructor, sets some options */
Caesar::Caesar()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
    temp.push_back("KEY");
    temp.push_back("DECRYPT");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "encrypted.txt");
    set_opt_value("DECRYPT", "0");
}

/* I am overriding the default module function
 * for displaying the description text
 */
void Caesar::disp_desc()
{
    cout << "Module: ciphers/caesar\n\tEncrypts (or decrypts) a text file with the caesar cipher.\n\tPlease define the following required options:\n\t\tINPUTFILE\tinput filename\n\t\tKEY\t\tinteger key between 1 and 25" << endl;
    cout << endl;
}

/* overrides the virtual function from Module
 * this is where the real meaty stuff happens
 */
int Caesar::run()
{
    ifstream in;
    ofstream out;
    string ibuff, obuff;
    int key = stoi(options["KEY"]), r = 0;
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

    if (options["KEY"].empty() || stoi(options["KEY"]) <= 0 || stoi(options["KEY"]) > 25 ) {
        cout << "[-] Please specify an integer key value between 1 and 25" << endl;
        return 3;
    }

    if (options["DECRYPT"] == "1")
        decrypt = true;
    else if (options["DECRYPT"] == "0")
        decrypt = false;
    else {
        cout << "[-] Invalid DECRYPT value (must be 0 or 1)" << endl;
        return 4;
    }

    cout << "[*] Opening input file: " << options["INPUTFILE"] << endl;
    in.open(options["INPUTFILE"]);

    if (!in.good()) {
        cout << "[-] Input file error" << endl;
        return 5;
    }

    cout << "[*] Opening output file: " << options["OUTPUTFILE"] << endl;
    out.open(options["OUTPUTFILE"]);

    // not sure about this gotta check
    if (decrypt)
        key = 26 - key;

    cout << "[*] Encrypting..." << endl;
    while (!in.eof()) {
        getline(in, ibuff);
        r = encrypt(ibuff, obuff, key);
        if (r != 0) {
            cout << "[-] Error while processing input file" << endl;
            return 6;
        }
        out << obuff << endl;
    }

    cout << "[*] Closing files" << endl;
    in.close();
    out.close();

    return 0;
}

/* implementation of caesar cipher
 * made static so anyone can use, helpful for attacks
 * input string must be lowercase
 * output string will be uppercase
 */
int Caesar::encrypt(string &in, string &out, int key)
{
    // clear output buffer
    out.clear();

    const char *ti = in.c_str();
    for (unsigned int i = 0; i < in.size(); i++) {
        char c = tolower(ti[i]);
        out += c;
    }
    return 0;
}
