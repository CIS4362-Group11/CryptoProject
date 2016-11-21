#ifndef MODULE_H
#define MODULE_H

class Module {
    protected:
        map<string,string> options;
        void set_opts(vector<string>&);

    public:
        Module();
        void disp_opts();
        void set_opt_value(string, string);
        virtual void disp_desc();
        virtual int run();
};

#endif
