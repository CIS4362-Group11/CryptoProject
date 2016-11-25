#ifndef CAESAR_H
#define CAESAR_H

class Caesar : public Module {
    public:
        Caesar();

        static int encrypt(string&, string&, int);

        void disp_desc();
        int run();
};

#endif
