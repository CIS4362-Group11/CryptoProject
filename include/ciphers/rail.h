#ifndef RAIL_H
#define RAIL_H

class Rail : public Module {
public:
	Rail();
	void disp_desc();
	int run();
	void encrypt(ifstream *in, ostream *out, int length);
	void decrypt(ifstream* in, ostream* out, int length);
};

#endif
