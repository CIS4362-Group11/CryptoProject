#include "main.h"

int main(int argc, char **argv)
{
    vector<string> t;
    t.push_back("testing");
    t.push_back("1");
    t.push_back("2");
    Module m(t);
    m.run();
    return 0;
}
