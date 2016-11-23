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

		int process_input(ifstream* in, vector< vector<int> >* charCounts, const int blockSize, const int charSetSize);
		int analyze_input(vector< vector<int> >* charCounts, vector<int>* charTotal, vector<int>* maxLength, vector<int>* maxCount);
		int process_output(ofstream * out, vector<vector<int>>* charCounts, vector<int>* charTotal, vector<int>* maxLength);
		int sort_arrays(vector<int>* charCounts, vector<int>* charIndexArray);
};

#endif
