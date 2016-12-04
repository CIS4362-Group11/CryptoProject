#ifndef HILL_H
#define HILL_H

class Hill : public Module {
    public:
        Hill();

        // here is a helper func that I will use
        static void encrypt(string&, string&, vector <vector<int>>&, bool);

        // here are the funcs I need to override
        void disp_desc();
        int run();
};

#endif
