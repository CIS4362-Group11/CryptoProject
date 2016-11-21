#include "main.h"

/* Module constructor */
Module::Module(vector<string> & _options)
{
    for (vector<string>::iterator it = _options.begin(); it != _options.end(); it++)
        options[*it] = "";
}

void Module::set_opt()
{

}

void Module::disp_opts()
{
    map<string,string>::iterator it = options.begin();
    cout << "\t| Option\t\t| Value" << endl;
    for (; it != options.end(); it++)
        cout << "\t| " << it->first << "\t\t| " << it->second << endl;
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
