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
    attack();

    return 0;
}

/* implements the kasiski attack */
void Kasiski::attack()
{
    string buff, test;
    ifstream in;
    unsigned int max = stoi(options["MAXBUFF"]);

    cout << "[*] Opening input file: " << options["INPUTFILE"] << endl;
    in.open(options["INPUTFILE"]);

    while (getline(in, buff)) {
        for (unsigned int i = 0; i < buff.size(); i++)
            if (isalpha(buff[i]) && test.size() < max)
                test += tolower(buff[i]);
            else if (test.size() >= max)
                break;
        if (test.size() >= max)
            break;
    }

    // cout << test << endl;

    if (test.size() < max)
        cout << "[-] WARNING: Kasiski attack buffer not full" << endl;

    cout << "[*] Finding possible key lengths..." << endl;
    vector<int> candidates;
    for (unsigned int i = 0; i < (test.size()-2); i++) {
        string pattern = test.substr(i, 3);
        size_t found = test.find(pattern, i+1);
        if (found != string::npos)
            if (find(candidates.begin(), candidates.end(), found-i) == candidates.end())
                candidates.push_back(found-i);
    }

    map<int,int> factors;
    for (unsigned int i = 0; i < candidates.size(); i++) {
        for (int j = 3; j <= candidates[i]; j++) {
            if (candidates[i] % j == 0) {
                auto it = factors.find(j);
                if (it != factors.end())
                    factors[j]++;
                else
                    factors[j] = 1;
            }
        }
    }

    /* insertion sort */
    vector<int> lengths_to_try(factors.size());
    int i = 0;
    for (auto it = factors.begin(); it != factors.end(); it++, i++)
        lengths_to_try[i] = it->first;
    for (unsigned int i = 0; i < factors.size(); i++) {
        int j = i;
        while (j > 0 && factors[lengths_to_try[j-1]] < factors[lengths_to_try[j]]) {
            int temp = lengths_to_try[j-1];
            lengths_to_try[j-1] = lengths_to_try[j];
            lengths_to_try[j] = temp;
            j--;
        }
    }

    // for (int i = 0; i < lengths_to_try.size(); i++)
    //     cout << lengths_to_try[i] << " " << factors[lengths_to_try[i]] << endl;

    cout << "[*] Most likely key length: " << lengths_to_try[0] << endl;
}
