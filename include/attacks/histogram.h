#ifndef HISTOGRAM_H
#define HISTOGRAM_H

class Histogram : public Module {
    public:
        Histogram();

        // here is a helper func that I will use
        void test();

        // here are the funcs I need to override
        void disp_desc();
        int run();
};

#endif
