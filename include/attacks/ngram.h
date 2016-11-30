#ifndef NGRAM_H
#define NGRAM_H

class Ngram : public Module {
    public:
        Ngram();

        // here is a helper func that I will use
        void test();

        // here are the funcs I need to override
        void disp_desc();
        int run();

		int process_input(ifstream* in, vector< int >* charCounts, const int blockSize, const int charSetSize);
		int process_output(ofstream* out, vector<int>* charCounts, const int charSetSize);
		string conv_int_to_hex(int input);
};

#endif
