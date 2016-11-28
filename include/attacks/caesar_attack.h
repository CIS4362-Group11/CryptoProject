#ifndef CAESAR_ATTACK_H
#define CAESAR_ATTACK_H

class CaesarAttack : public Module {
    public:
        CaesarAttack();

        // here are the funcs I need to override
        void disp_desc();
        int run();

    private:
        void begin_attack(ifstream&, ofstream&);
};

#endif
