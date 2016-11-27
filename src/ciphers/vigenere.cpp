#include "main.h"

/* example constructor, sets some options */
Vigenere::Vigenere()
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
void Vigenere::disp_desc()
{
    cout << "Module: ciphers/vigenere\n\tEncrypts (or decrypts) a text file with the repeated-key vigenere cipher.\n\tInput text should be lowercase, symbols are ignored.\n\tCipher text is all uppercase.\n\tPlease define the following required options:\n\t\tINPUTFILE\tinput filename\n\t\tKEY\t\tlowercase string key" << endl;
    cout << endl;
}

/* overrides the virtual function from Module
 * this is where the real meaty stuff happens
 */
int Vigenere::run()
{
    ifstream in;
    ofstream out;
    string ibuff, obuff;
    int key = stoi(options["KEY"]);
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

    if (!decrypt) cout << "[*] Encrypting..." << endl;
    else cout << "[*] Decrypting..." << endl;
    while (getline(in, ibuff)) {
        encrypt(ibuff, obuff, key, decrypt);
        out << obuff << endl;
    }

    cout << "[*] Closing files" << endl;
    in.close();
    out.close();

    return 0;
}

/* implementation of vigenere cipher
 * made static so anyone can use, helpful for attacks
 * input string must be lowercase
 * output string will be uppercase
 */
void Vigenere::encrypt(string &in, string &out, int key, bool decrypt)
{
    // clear output buffer
    out.clear();

    const char *ti = in.c_str();
    for (unsigned int i = 0; i < in.size(); i++) {
        char c;
        if (!decrypt) {
            c = tolower(ti[i]);
            if (isalpha(c)) {
                int tc = ((int) c) - 97;
                tc = ((tc + key) % 26) + 97;
                c = toupper((char) tc);
            }
        } else {
            c = toupper(ti[i]);
            if (isalpha(c)) {
                int tc = ((int) c) - 65;
                tc = (tc - key) + 65;
                if (tc < 65) tc += 26;
                c = tolower((char) tc);
            }
        }
        out += c;
    }
}
