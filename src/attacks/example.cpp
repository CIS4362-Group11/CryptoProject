#include "main.h"

Example::Example()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("FILENAME");
    set_opts(temp);

    // set default values
    set_opt_value("FILENAME", "/tmp/cryptotmpfile");
}

void Example::test()
{
    cout << "example test" << endl;
}
