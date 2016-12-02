#ifndef CAESAR_ATTACK_H
#define CAESAR_ATTACK_H

float chisq(float*, float*);
void left_shift_freq(float*);

class CaesarAttack : public Module {
    public:
        CaesarAttack();

        // here are the funcs I need to override
        void disp_desc();
        int run();

        static void begin_attack(ifstream&, ofstream&, string&, int);
};

#endif
