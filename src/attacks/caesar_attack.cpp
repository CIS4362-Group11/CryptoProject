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

    cout << "[*] Beginning attack..." << endl;
    begin_attack(in, out);

    cout << "[*] Closing files" << endl;
    in.close();
    out.close();

    return 0;
}

/* not safe helper function
 * doesn't check array out of bounds
 */
float chisq(float *y, float *ym)
{
    float sq = 0.0f;
    for (int i = 0; i < 26; i++)
        sq += (y[i] - ym[i])*(y[i] - ym[i]);
    return sq;
}

/* shifts each entry to the left one */
void left_shift_freq(float *freq)
{
    float temp = freq[0];
    for (int i = 0; i < 25; i++)
        freq[i] = freq[i+1];
    freq[25] = temp;
}

/* handles the main attack sequence */
void CaesarAttack::begin_attack(ifstream &in, ofstream &out)
{
    /* from analyzing huckleberry finn */
    float english[] = {0.0834423, 0.0169666, 0.0191698, 0.0539923, 0.112034, 0.0180501, 0.0246394, 0.0602159, 0.0646833, 0.00280142, 0.0130094, 0.0398317, 0.0236778, 0.0747964, 0.0835958, 0.0138107, 0.000442449, 0.0464413, 0.057629, 0.0967157, 0.0318857, 0.00675638, 0.03031, 0.0011919, 0.0234859, 0.00042439};

    int freq[26] = {0}, count = 0;
    float ffreq[26] = {0.0f};
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
        ffreq[i] = (float)freq[i]/(float)count;

    int mins[26]; float csq[26];
    for (int i = 0; i < 26; i++) {
        mins[i] = i;
        csq[i] = chisq(ffreq, english);
        left_shift_freq(ffreq);
    }

    /* insertion sort */
    for (int i = 0; i < 26; i++) {
        int j = i;
        while (j > 0 && csq[mins[j-1]] > csq[mins[j]]) {
            int temp = mins[j-1];
            mins[j-1] = mins[j];
            mins[j] = temp;
            j--;
        }
    }

    cout << "[+] Most likely shift: " << mins[0] << endl;

    for (int i = 0; i < 26; i++) {
        cout << "[*] Decrypting file with shift " << mins[i] << " (chi^2 = " << csq[mins[i]] << ")..." << endl;

        out.open(options["OUTPUTFILE"]);

        // reset input
        in.clear();
        in.seekg(0);

        string obuff;
        int samplecount = 0;
        cout << "[*] Sample from decryption: " << endl;
        while (getline(in, buff)) {
            Caesar::encrypt(buff, obuff, mins[i], true);
            out << obuff << endl;
            if (samplecount < 5) {
                cout << obuff << endl;
                samplecount++;
            } else {
                break;
            }
        }

        string ans;
        cout << "[*] Continue decryption? [Y/n]: ";
        getline(cin, ans);
        if (ans == "N" || ans == "n") {
            out.close();
            continue;
        }

        while (getline(in, buff)) {
            Caesar::encrypt(buff, obuff, mins[i], true);
            out << obuff << endl;
        }

        // if we get here we are done
        break;
    }
}
