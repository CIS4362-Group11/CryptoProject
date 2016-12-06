#include "main.h"

/* if you add a module please register it
 * here by adding it to the mapping
 */
void register_modules(map<string,Module*> &m)
{
    // registering a dummy module, do not copy this
    // m.insert(make_pair("module", new Module()));

    // registration, copy this for new modules
    m.insert(make_pair("attacks/example", new Example()));
	m.insert(make_pair("attacks/histogram", new Histogram()));
	m.insert(make_pair("attacks/ngram", new Ngram()));
    m.insert(make_pair("attacks/caesar", new CaesarAttack()));
    m.insert(make_pair("attacks/kasiski", new Kasiski()));
    m.insert(make_pair("attacks/hillatt", new HillAtt()));
    m.insert(make_pair("ciphers/caesar", new Caesar()));
    m.insert(make_pair("ciphers/vigenere", new Vigenere()));
    m.insert(make_pair("ciphers/hill", new Hill()));
}

/* helper function to split strings */
void split(string &s, char delim, vector<string> &e)
{
    stringstream ss;
    ss.str(s);

    string item;
    while (getline(ss, item, delim))
        e.push_back(item);
}

/* main loop prompt */
void prompt(int &alive, map<string,Module*> &m, string &curr_m)
{
    string buff;
    vector<string> tokens;

    // print prompt
    cout << "[" << curr_m << "] > ";

    // get input
    getline(cin, buff);
    split(buff, ' ', tokens);

    // parse tokens
    if (tokens.size() < 1) {
        return;
    } else if (tokens[0] == "use" || tokens[0] == "u") {
        if (tokens.size() < 2)
            cout << "[-] Please specify a module" << endl;
        else {
            if (m.find(tokens[1]) == m.end())
                cout << "[-] Invalid module name" << endl;
            else
                curr_m = tokens[1];
        }
    } else if(tokens[0] == "show" || tokens[0] == "s") {
        if (tokens.size() < 2)
            cout << "[-] Please specify what information to show:\n\tmodules\n\tdescription\n\toptions" << endl;
        else if (tokens[1] == "description" || tokens[1] == "desc" || tokens[1] == "d") {
            if (curr_m.empty())
                cout << "[-] Please select a module first" << endl;
            else
                m[curr_m]->disp_desc();
        } else if (tokens[1] == "options" || tokens[1] == "o") {
            if (curr_m.empty())
                cout << "[-] Please select a module first" << endl;
            else
                m[curr_m]->disp_opts();
        } else if (tokens[1] == "modules" || tokens[1] == "m") {
            cout << "Available Modules:" << endl;
            map<string,Module*>::iterator it = m.begin();
            while (it != m.end())
                cout << "\t" << (it++)->first << endl;
        }
    } else if (tokens[0] == "set") {
        if (tokens.size() < 3)
            cout << "[-] Please specify option and value\n[-] Options can be listed with: show options" << endl;
        else if (curr_m.empty())
            cout << "[-] Please select a module first" << endl;
        else
            m[curr_m]->set_opt_value(tokens[1], tokens[2]);
    } else if (tokens[0] == "run" || tokens[0] == "r") {
        if (curr_m == "")
            cout << "[-] Please select a module first" << endl;
        else {
            try {
                int r = m[curr_m]->run();
                if (r != 0)
                    cout << "[-] Module returned with non-zero error value: " << r << endl;
            } catch (const exception &e) {
                cout << "[-] Caught fatal error from module: " << e.what() << endl;
            }
        }
    } else if (tokens[0] == "clear" || tokens[0] == "c") {
        curr_m.clear();
    } else if (tokens[0] == "help" || tokens[0] == "h") {
        cout << HELPTEXT << endl;
    } else if (tokens[0] == "exit" || tokens[0] == "e" || tokens[0] == "quit" || tokens[0] == "q") {
        alive = 0;
    } else {
        cout << "[-] Invalid command" << endl;
    }
}

/* parse command line args */
int parse_args(int argc, char **argv, string &cm, map<string,Module*> &m)
{
    bool run = false;
    vector<string> tokens;
    for (int i = 1; i < argc; i++) {
        if (strlen(argv[i]) > 2 && memcmp(argv[i], "-m", 2) == 0) {
            if (m.find(&argv[i][2]) == m.end()) {
                cout << "[-] Invalid module name" << endl;
                return 1;
            } else {
                cm = &argv[i][2];
            }
        } else if (strlen(argv[i]) > 2 && memcmp(argv[i], "-o", 2) == 0) {
            string temp = &argv[i][2];
            split(temp, ',', tokens);
        } else if (strcmp(argv[i], "-r") == 0)
            run = true;
    }

    for (unsigned int i = 0; i < tokens.size(); i++) {
        vector<string> pair;
        split(tokens[i], ':', pair);
        if (pair.size() != 2) {
            cout << "[-] Invalid option pair" << endl;
            return 1;
        } else if (cm.empty()) {
            cout << "[-] Cannot use -o without -m, dropping option: " << pair[0] << endl;
        } else {
            m[cm]->set_opt_value(pair[0], pair[1]);
        }
    }

    if (run && !cm.empty()) {
        m[cm]->run();
        return 1;
    } else
        return 0;
}

int main(int argc, char **argv)
{
    int alive = 1;
    map<string,Module*> mods;
    string current_module = "";

    // load modules
    register_modules(mods);

    if (parse_args(argc, argv, current_module, mods)) return 0;

    // welcome information and version
    cout << WELCOME << endl;
    cout << "[*] Version " << VERSION << endl;

    // number of loaded modules
    // modules loaded earlier so parse_args can use it
    cout << "[*] Loaded " << mods.size() << " modules" << endl;

    // help
    cout << "[*] Type 'help' to see available commands" << endl;

    // main loop
    while (alive)
        prompt(alive, mods, current_module);
    return 0;
}
