#ifndef HILLATT_H
#define HILLATT_H

class HillAtt : public Module {
    public:
        HillAtt();

        // here is a helper func that I will use
		static int attack(string&, string&, vector <vector<int>>&);
        static void decrypt(string&, string&, vector <vector<int>>&);

        // here are the funcs I need to override
        void disp_desc();
        int run();
};

#endif
