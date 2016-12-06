#include "main.h"
#include "Eigen/Dense"

using namespace Eigen;
/* example constructor, sets some options */
Hill::Hill()
{
    // declare options, keep options as uppercase
    vector<string> temp;
    temp.push_back("INPUTFILE");
    temp.push_back("OUTPUTFILE");
	temp.push_back("KEY");
	temp.push_back("DECRYPT");
	temp.push_back("MATRIX_ROWS");
	temp.push_back("MATRIX_COLUMNS");
    set_opts(temp);

    // set default values, option must exist or error will printed
    set_opt_value("OUTPUTFILE", "encrypted.txt");
	set_opt_value("DECRYPT", "0");
	

}

/* I am overriding the default module function
 * for displaying the description text
 */
void Hill::disp_desc()
{
    cout << "Module: ciphers/Hill\n\tEncrypts (or decrypts) a text file with the hill cipher.\n\tInput text should be lowercase, symbols are ignored.\n\tCipher text is all uppercase.\n\tPlease define the following required options:\n\t\tINPUTFILE\tinput filename\n\t\tKEY\t\tlowercase string key" << endl;
   // disp_opts();
    cout << endl;
}

/* overrides the virtual function from Module
 * this is where the real meaty stuff happens
 */
int Hill::run()
{
	ifstream in;
    ofstream out;
    string ibuff, obuff;
    bool decrypt = false;

    // perform error checking on options first
    if (options["INPUTFILE"].empty()) {
        cout << "[-] Please specify an input file" << endl;
        return 1;
    }

    if (options["OUTPUTFILE"].empty()) {
        cout << "[-] Please specify an output file" << endl;
        return 2;
    }

    if (options["KEY"].empty()) {
        cout << "[-] Please specify a key" << endl;
        return 3;
    }
    string tkey = options["KEY"];

    if (options["DECRYPT"] == "1")
        decrypt = true;
    else if (options["DECRYPT"] == "0")
        decrypt = false;
    else {
        cout << "[-] Invalid DECRYPT value (must be 0 or 1)" << endl;
        return 4;
    }
	
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
	
	if (!decrypt) cout << "[*] Encrypting..." << endl;
    else cout << "[*] Decrypting..." << endl;
	int rows = stoi(options["MATRIX_ROWS"]);
	int columns = stoi(options["MATRIX_COLUMNS"]);
	vector< vector<int> > vec;

	int keytrack = 0;
	int tksize = tkey.size();
	for(int i = 0;i < rows;i++)
	{	
		vector<int> row;//putting into the row
		for(int j = 0;j < columns;j++)
		{
			if(tksize <=0)
			{
				row.push_back(abs(tksize));
				tksize--;
				if(tksize%26==0)
				{
					tksize = 0;
				}
			
			}
			else if (isalpha(tkey[keytrack]))
			{
					row.push_back( ((int) tolower(tkey[keytrack])) - 97 );
					tksize--;
					keytrack++;
			}
			else
			{
				cout << "[-] Invalid key" << endl;
				return 7;
			}	
		}
		vec.push_back(row);
	}
			MatrixXf a(vec[0].size(),vec.size());
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
				cout << "The key is not invertible, can encrypt, but can't decrypt." << endl;
			}
	
	string passbuff = "";
	while (getline(in, ibuff)) {
		passbuff += ibuff;
        encrypt(passbuff, obuff, vec, decrypt);
		out << obuff << endl;
    }
	if(passbuff.length() > 0)//add random character if not a multiple
	{
		for(int i = passbuff.length(); i < columns;i++)
		{
			passbuff+="x";
		}
		encrypt(passbuff, obuff, vec, decrypt);
		out << obuff << endl;
	}
	cout << endl;
	for(unsigned int i = 0;i< vec.size();i++)
	{
		for(unsigned int j = 0;j < vec[i].size();j++)
		{
			cout << vec[i][j] << " ";
		}
		cout << endl;
	}
    in.close();
    out.close();
	return 0;
}


/* implementation of  cipher
 * made static so anyone can use, helpful for attacks
 * input string must be lowercase
 * output string will be uppercase
 */
void Hill::encrypt(string& in, string& out , vector <vector<int>> &vec, bool decrypt)
{
	out.clear();
	const char *ti = in.c_str();
	vector<int> text;
	int writeout = 0;
	for (unsigned int i = 0; i < in.size(); i++) {
        char c;
        if (!decrypt) {//encrypt
            c = tolower(ti[i]);
            if (isalpha(c)) {
                text.push_back( ((int) c) - 97);
				writeout++;
            }
			
			if(writeout>0 && (writeout)%vec[0].size()==0)
			{
				int sum = 0;
				for(unsigned int r =0; r < vec.size();r++)
				{
					for(unsigned int c = 0; c < vec[0].size();c++)
					{
						sum +=vec[r][c]*text[c];
					}
					sum%=26;
					out += toupper(sum+97);
					sum = 0;
					writeout = 0;
				}
				text.clear();
			}
        } 
		else //decrypt
		{
			
			MatrixXf a(vec[0].size(),vec.size());
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
			
			cout << a << endl;
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
        }
		
    }
	in = "";
	if(text.size() > 0)
	{
		if(!decrypt)
		{
			for(unsigned int i = 0; i < text.size();i++)
			{
				in+=text[i]+97;
			}
		}

	}
	
	
	
}
