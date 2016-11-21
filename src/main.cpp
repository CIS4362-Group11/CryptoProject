#include "main.h"

int main(int argc, char **argv)
{
    vector<string> t;
    t.push_back("testing1");
    t.push_back("testing2");
    t.push_back("testing3");
    Module m(t);
    m.disp_opts();
    return 0;
}
