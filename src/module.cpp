#include "main.h"

/* Module constructor */
Module::Module(vector<string> & _options)
{
    for (vector<string>::iterator it = _options.begin(); it != _options.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}

/* This function is virtual and should be overwritten */
void Module::show_help()
{
    cout << "[-] Help functionality has not been implemented." << endl;
}

/* This function is virtual and should be overwritten */
int Module::run()
{
    cout << "[-] Run functionality has not been implemented." << endl;
    return -1;
}
