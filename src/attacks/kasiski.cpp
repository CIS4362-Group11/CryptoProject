#include "main.h"

float Kasiski::english[] = {0.0834423, 0.0169666, 0.0191698, 0.0539923, 0.112034, 0.0180501, 0.0246394, 0.0602159, 0.0646833, 0.00280142, 0.0130094, 0.0398317, 0.0236778, 0.0747964, 0.0835958, 0.0138107, 0.000442449, 0.0464413, 0.057629, 0.0967157, 0.0318857, 0.00675638, 0.03031, 0.0011919, 0.0234859, 0.00042439};

/* example constructor, sets some options */
Kasiski::Kasiski()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
    temp.push_back("ASSUMEZERO");
    temp.push_back("MAXBUFF");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "/tmp/kasiskiresults.txt");
    set_opt_value("ASSUMEZERO", "0");
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

    if (options["ASSUMEZERO"].empty() || stoi(options["ASSUMEZERO"]) < 0 || stoi(options["ASSUMEZERO"]) > 1) {
        cout << "[-] Please specify an ASSUMEZERO value of 0 or 1" << endl;
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

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
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

    for (int i = 0; i < 5; i++)
        cout << "\t" << lengths_to_try[i] << " " << factors[lengths_to_try[i]] << "\t";
    cout << endl;
    for (int i = 5; i < 10; i++)
        cout << "\t" << lengths_to_try[i] << " " << factors[lengths_to_try[i]] << "\t";
    cout << endl;

    cout << "[*] Finding possible keys... " << endl;

    vector<vector<int>> possible_keys(10);
    float key_chi[10] = {0};
    for (int i = 0; i < 10; i++) {
        possible_keys[i] = find_key(test, lengths_to_try[i]);

        unsigned int keyi = 0;
        Vigenere::encrypt(test, buff, possible_keys[i], keyi, true);

        float freq[26] = {0.0f};
        for (unsigned int j = 0; j < buff.size(); j++) {
            int t = ((int) tolower(buff[j])) - 97;
            freq[t]++;
        }
        for (int j = 0; j < 26; j++)
            freq[j] /= ((float) buff.size());
        key_chi[i] = chisq(freq, english);
        // cout << key_str(possible_keys[i]) << "\t" << key_chi[i] << "\t" << buff.substr(0,70) << endl;
    }

    int mins[10] = {0};
    for (int i = 0; i < 10; i++)
        mins[i] = i;
    for (int i = 0; i < 10; i++) {
        int j = i;
        while (j > 0 && key_chi[mins[j-1]] > key_chi[mins[j]]) {
            int temp = mins[j-1];
            mins[j-1] = mins[j];
            mins[j] = temp;
            j--;
        }
    }

    cout << "[+] Possible keys (sorted by probability):" << endl;
    for (int i = 0; i < 10; i++)
        cout << "\t[" << i << "] " << key_str(possible_keys[mins[i]]) << endl;

    int choice = -1;
    if (stoi(options["ASSUMEZERO"]))
        choice = 0;
    else {
        while (choice < 0 || choice >= 10) {
            cout << "[*] Select key for decryption: ";
            getline(cin, buff);
            if (!is_number(buff)) continue;
            choice = stoi(buff);
        }
    }

    ofstream out;
    cout << "[*] Opening output file: " << options["OUTPUTFILE"] << endl;
    out.open(options["OUTPUTFILE"]);

    in.clear();
    in.seekg(0);
    string obuff;
    unsigned int key_index = 0;
    while (getline(in, buff)) {
        Vigenere::encrypt(buff, obuff, possible_keys[mins[choice]], key_index, true);
        out << obuff << endl;
    }

    cout << "[*] Closing files" << endl;
    in.close();
    out.close();
}

string Kasiski::key_str(vector<int> &key)
{
    string out = "";
    for (unsigned int i = 0; i < key.size(); i++)
        out += ((char) (key[i]+97));
    return out;
}

vector<int> Kasiski::find_key(string &buff, int length)
{
    float *freq = new float[length*26];

    for (int i = 0; i < length; i++)
        for (int j = 0; j < 26; j++)
            freq[i*26 + j] = 0;

    int pos = 0;
    for (unsigned int i = 0; i < buff.size(); i++) {
        int t = ((int) tolower(buff[i])) - 97;
        freq[pos*26 + t]++;
        if ((++pos) >= length)
            pos = 0;
    }

    for (int i = 0; i < length; i++)
        for (int j = 0; j < 26; j++)
            freq[i*26 + j] /= ((float)buff.size()/(float)length);

    // for (int i = 0; i < length; i++) {
    //     for (int j = 0; j < 26; j++)
    //         cout << freq[i*26 + j] << " ";
    //     cout << endl;
    // }

    vector<int> key;
    for (int i = 0; i < length; i++) {
        float chis[26];
        float *ffreq = &freq[i*26];
        for (int j = 0; j < 26; j++) {
            chis[j] = chisq(ffreq, english);
            left_shift_freq(ffreq);
        }

        int min = 0; float val = chis[min];
        for (int x = 1; x < 26; x++) {
            if (chis[x] < val) {
                min = x; val = chis[min];
            }
        }

        key.push_back(min);
    }

    delete[] freq;
    return key;
}
