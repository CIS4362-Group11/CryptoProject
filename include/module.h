#ifndef MODULE_H
#define MODULE_H

class Module {
    protected:
        map<string, string> options;

    public:
        Module(vector<string>&);
        void set_opt();
        virtual void show_help();
        virtual int run();
};

#endif
