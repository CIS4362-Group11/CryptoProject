#ifndef MODULE_H
#define MODULE_H

class Module {
    protected:
        map<string,string> options;

    public:
        Module();
        void set_opts(vector<string>&);
        void disp_opts();
        void set_opt_value(string, string);
        virtual void disp_desc();
        virtual int run();
};

#endif
