#ifndef KASISKI_H
#define KASISKI_H

class Kasiski : public Module {
    public:
        Kasiski();

        // here are the funcs I need to override
        void disp_desc();
        int run();

    private:
        static float english[];
        vector<int> find_key(string&, int);
        string key_str(vector<int>&);
        void attack();
};

#endif
