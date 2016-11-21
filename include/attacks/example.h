#ifndef EXAMPLE_H
#define EXAMPLE_H

class Example : public Module {
    public:
        Example();

        // here is a helper func that I will use
        void test();

        // here are the funcs I need to override
        void disp_desc();
        int run();
};

#endif
