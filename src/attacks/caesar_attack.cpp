#include "main.h"

/* example constructor, sets some options */
CaesarAttack::CaesarAttack()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "/tmp/caesarattackresults.txt");
}

/* I am overriding the default module function
 * for displaying the description text
 */
void CaesarAttack::disp_desc()
{
    cout << "Module: attacks/caesar_attack\n\tThis module attempts to guess the correct shift for the Caesar cipher.\n\tIt makes use of frequency analysis to determine a proper guess.\n\tMay require user interaction.\n\tPlease define an INPUTFILE." << endl;
    cout << endl;
}

/* overrides the virtual function from Module
 * this is where the real meaty stuff happens
 */
int CaesarAttack::run()
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

    cout << "[*] Opening input file: " << options["INPUTFILE"] << endl;
    in.open(options["INPUTFILE"]);

    cout << "[*] Opening output file: " << options["OUTPUTFILE"] << endl;
    out.open(options["OUTPUTFILE"]);

    cout << "[*] Beginning attack..." << endl;
    begin_attack(in, out);

    cout << "[*] Closing files" << endl;
    in.close();
    out.close();

    return 0;
}

/* handles the main attack sequence */
void CaesarAttack::begin_attack(ifstream &in, ofstream &out)
{
    int freq[26] = {0}, count = 0;
    string buff;

    while (getline(in, buff)) {
        for (unsigned int i = 0; i < buff.size(); i++) {
            if (isalpha(buff[i])) {
                int t = ((int) tolower(buff[i])) - 97;
                freq[t]++; count++;
            }
        }
    }

    for (int i = 0; i < 26; i++)
        cout << freq[i] << endl;
}
