#include "main.h"
#include "../ciphers/Eigen/Dense"

using namespace Eigen;
/* example constructor, sets some options */
HillAtt::HillAtt()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
	temp.push_back("PLAINTEXT");
	temp.push_back("CIPHERTEXT");
	temp.push_back("MATRIX_ROWS");
	temp.push_back("MATRIX_COLUMNS");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "broken.txt");
    set_opt_value("INPUTFILE", "encrypted.txt");
	set_opt_value("MATRIX_ROWS", "2");
	set_opt_value("MATRIX_COLUMNS", "2");
	set_opt_value("PLAINTEXT", "upto");
	set_opt_value("CIPHERTEXT", "RERZ");
	

}

/* I am overriding the default module function
 * for displaying the description text
 */
void HillAtt::disp_desc()
{
    cout << "Module: attacks/HillAtt\n\tUses known plaintext and cipher text to try to crack the key. \n\tBoth the plaintext must be invertible and have a modulus of 26,\n\t all of this is checked in the program.\n\tInput text should be lowercase, symbols are ignored.\n\tCipher text is all uppercase.\n\tPlease define the following required options:\n\t\tPLAINTEXT\tfound in original file\n\t\tCIPHERTEXT\tuppercase corresponds to plaintext" << endl;
   // disp_opts();
    cout << endl;
}

/* overrides the virtual function from Module
 * this is where the real meaty stuff happens
 */
int HillAtt::run()
{
	ifstream in;
    ofstream out;
    string ibuff, obuff;

    // perform error checking on options first
    if (options["INPUTFILE"].empty()) {
        cout << "[-] Please specify an input file" << endl;
        return 1;
    }

    if (options["OUTPUTFILE"].empty()) {
        cout << "[-] Please specify an output file" << endl;
        return 2;
    }

    if (options["PLAINTEXT"].empty()) {
        cout << "[-] Please specify known plaintext" << endl;
        return 3;
    }
	string tkey = options["PLAINTEXT"]; 
	if (options["CIPHERTEXT"].empty()) {
        cout << "[-] Please specify corresponding ciphertext" << endl;
        return 4;
    }
	string ckey = options["CIPHERTEXT"];
    
	
	if(options["MATRIX_ROWS"].empty() || options["MATRIX_COLUMNS"].empty())
	{
		cout << "[-] Please specify matrix dimensions" << endl;
		return 5;
	}
    cout << "[*] Opening input file: " << options["INPUTFILE"] << endl;
    in.open(options["INPUTFILE"]);

    if (!in.good()) {
        cout << "[-] Input file error" << endl;
        return 6;
    }

    cout << "[*] Opening output file: " << options["OUTPUTFILE"] << endl;
    out.open(options["OUTPUTFILE"]);
	
	cout << "Attacking hill cipher!" << endl;
	//int rows = stoi(options["MATRIX_ROWS"]);
	int columns = stoi(options["MATRIX_COLUMNS"]);
	vector < vector<int>> vec;
	if(attack(tkey,ckey, vec)==9)
	{
		return 9;
	}

			/*MatrixXf a(vec[0].size(),vec.size());
			for(unsigned int r = 0 ; r < vec.size();r++)
			{
				for(unsigned int c = 0; c <vec[0].size();c++)
				{
					a(r,c) = vec[r][c];
				}
			}*/
			
	
	string passbuff = "";
	while (getline(in, ibuff)) {
		passbuff += ibuff;
		decrypt(passbuff,obuff,vec);
       // encrypt(passbuff, obuff, vec, decrypt);
		out << obuff << endl;
    }
	if(passbuff.length() > 0)//add random character if not a multiple
	{
		for(int i = passbuff.length(); i < columns;i++)
		{
			passbuff+="x";
		}
		//encrypt(passbuff, obuff, vec);
		out << obuff << endl;
	}
	cout << endl;
	/*for(unsigned int i = 0;i< vec.size();i++)
	{
		for(unsigned int j = 0;j < vec[i].size();j++)
		{
			cout << vec[i][j] << " ";
		}
		cout << endl;
	}*/
    in.close();
    out.close();
	return 0;
}


/* implementation of  cipher
 * made static so anyone can use, helpful for attacks
 * input string must be lowercase
 * output string will be uppercase
 */
 int HillAtt::attack(string& ptext, string&ctext, vector <vector<int>> &vec)
 {
	MatrixXf a(2,2);
	MatrixXf k(2,2);
	a(0,0) = ptext[0]-97;
	a(1,0) = ptext[1]-97;
	a(0,1) = ptext[2]-97;
	a(1,1) = ptext[3]-97;
	
	
	k(0,0) = tolower(ctext[0])-97;
	k(1,0) = tolower(ctext[1])-97;
	k(0,1) = tolower(ctext[2])-97;
	k(1,1) = tolower(ctext[3])-97;
	int y = rint(a.determinant());
	if(y==0)
	{
		cout << "Plaintext provided not invertible" << endl;
		return 9;
	}
	a = a.inverse();
	int z =y;
	int m = 26;
	y = fmod(y,m);
	if(y < 0)
	{
		y = y+m;
	}
	int x;
	int inv = 1;
	for(x = 1; x < m; x++) {
		if(fmod((y*x),m) == 1)
		{
			inv = 0;
			break;
		}
	}
	if(inv ==1)
	{
		cout << "could not apply modulus 26" << endl;
		return 9;
	}
	for(int i = 0; i < a.rows(); i++)
	{
		for(int j = 0; j <a.cols();j++)
		{
			int m=26;
			a(i,j) = a(i,j)*z;
			a(i,j) *=x;
			int temp = fmod(a(i,j),m);
			if(temp < 0)
			{
				temp = temp+m;
			}
			a(i,j) = temp;
		}
	}
	k = k*a;
	for(int i = 0; i < k.rows(); i++)
	{
		vector<int> row;
		for(int j = 0; j <k.cols();j++)
		{
			int m=26;
			int temp = fmod(k(i,j),m);
			if(temp < 0)
			{
				temp = temp+m;
			}
			k(i,j) = temp;
			row.push_back(k(i,j));
		}
		vec.push_back(row);
	}
	cout << "The key found is:" << endl;
	cout << k << endl;
	return 0;
 }

 
void HillAtt::decrypt(string& in, string& out, vector <vector<int>>& vec)
{
	out.clear();
	
	const char *ti = in.c_str();
	vector<int> text;
	int writeout = 0;
	for (unsigned int i = 0; i < in.size(); i++) {
        char c;

			MatrixXf a(vec.size(),vec[0].size());
			for(unsigned int r = 0 ; r < vec.size();r++)
			{
				for(unsigned int c = 0; c <vec[0].size();c++)
				{
					a(r,c) = vec[r][c];
				}
			}
			int y = a.determinant();
			if(y == 0)
			{
				cout << "The key is not invertible, can't decrypt" << endl;
				return;
			}
			a= a.inverse();
			int m = 26;
			int z =y;
			y %= m;
			int x;
			for(x = 1; x < m; x++) {
				if((y*x) % m == 1)
				{
					break;
				}
				
			
			}	
			
			for(int l = 0; l < a.rows();l++)
			{
				for(int k = 0; k < a.cols(); k++)
				{
					a(l,k) = a(l,k)*z;
					a(l,k) *=x;
					int temp = fmod(a(l,k),m);
					if(temp < 0)
					{
						temp = temp+m;
					}
					a(l,k) = temp;
				}
			}
			
			//cout << a << endl;
			c = toupper(ti[i]);
            if (isalpha(c)) {
                text.push_back( ((int) c) - 65);
				writeout++;
            }
			
			if(writeout>0 && (writeout)%vec[0].size()==0)
			{
				int sum = 0;
				for(unsigned int r =0; r < a.rows();r++)
				{
					for(unsigned int c = 0; c < a.cols();c++)
					{
						sum +=a(r,c)*text[c];
					}
					sum%=26;
					out += tolower(sum+65);
					sum = 0;
					writeout = 0;
				}
				text.clear();
			}
        //}
		
    }
	in = "";
	if(text.size() > 0)
	{
		//if(!decrypt)
		// {
			for(unsigned int i = 0; i < text.size();i++)
			{
				in+=text[i]+97;
			}
		//}

	}
	
	
	
}
