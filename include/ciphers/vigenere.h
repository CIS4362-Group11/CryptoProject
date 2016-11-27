#ifndef VIGENERE_H
#define VIGENERE_H

class Vigenere : public Module {
    public:
        Vigenere();

        static void encrypt(string&, string&, int, bool);

        void disp_desc();
        int run();
};

#endif
