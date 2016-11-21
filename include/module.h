#ifndef MODULE_H
#define MODULE_H

class Module {
    protected:
        map<string,string> options;

    public:
        Module(vector<string>&);
        void set_opt();
        void disp_opts();
        virtual void show_help();
        virtual int run();
};

#endif
