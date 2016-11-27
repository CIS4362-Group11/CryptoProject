#ifndef CAESAR_H
#define CAESAR_H

class Caesar : public Module {
    public:
        Caesar();

        static void encrypt(string&, string&, int, bool);

        void disp_desc();
        int run();
};

#endif
