#include "main.h"

/* example constructor, sets some options */
Example::Example()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "/tmp/cryptotmpfile");
}

/* helper function that doesn't really do anything */
void Example::test()
{
    cout << "example test" << endl;
}

/* I am overriding the default module function
 * for displaying the description text
 */
void Example::disp_desc()
{
    cout << "Module: attacks/example\n\tThis is an example module to show the abilities of the system.\n\tThis is some basic description text that is displayed when show description is input.\n\tThis program will just copy the info from one file to another.\n\tPlease define INPUTFILE for this example module to run correctly.\n\tOptions are given below:" << endl;
    disp_opts();
    cout << endl;
}

/* overrides the virtual function from Module
 * this is where the real meaty stuff happens
 */
int Example::run()
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
