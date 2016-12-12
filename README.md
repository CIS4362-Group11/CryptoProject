# CryptoProject
Cryptanalysis suite for classical ciphers

### Description
This project aims to be both extensible and full-featured. It implements many classic crytographic ciphers such as the Hill cipher, the Rail Fence cipher, the Caesar cipher, and the repeated key Vigenere cipher. Along with this, it implements multiple attacks on these ciphers such as histogram generation, n-gram generation, an attack on the Hill cipher, an attack on the Caesar cipher, and Kasiski examination.

### Usage

To run the project, simply invoke the executable `./cryptoproject` and then interact with the menu based system. Example usage is shown below. This is a typical interaction with an example module.

![Example module usage image](/example.png?raw=true "Example module usage")

An example usage command sequence is shown below:
```
$ ./cryptoproject
   ____ _               _         ____                  _
  / ___| | __ _ ___ ___(_) ___   / ___|_ __ _   _ _ __ | |_ ___
 | |   | |/ _` / __/ __| |/ __| | |   | '__| | | | '_ \| __/ _ \
 | |___| | (_| \__ \__ \ | (__  | |___| |  | |_| | |_) | || (_) |
  \____|_|\__,_|___/___/_|\___|  \____|_|   \__, | .__/ \__\___/
                                            |___/|_|             
                 _   _   _             _             
                / \ | |_| |_ __ _  ___| | _____ _ __ 
               / _ \| __| __/ _` |/ __| |/ / _ \ '__|
              / ___ \ |_| || (_| | (__|   <  __/ |   
             /_/   \_\__|\__\__,_|\___|_|\_\___|_|   
                                                     
[*] Version 0.1
[*] Loaded 10 modules
[*] Type 'help' to see available commands
[] > help
Available Commands:
	use <module>		select a module to use
	show <choice>		display information (modules, description, options)
	set <option> <value>	set the option to the desired value
	run			run the currently selected module
	clear			clear the currently selected module
	help			show this help text
	quit			quit the program
[] > show modules
Available Modules:
	attacks/caesar
	attacks/example
	attacks/hillatt
	attacks/histogram
	attacks/kasiski
	attacks/ngram
	ciphers/caesar
	ciphers/hill
	ciphers/rail
	ciphers/vigenere
[] > use attacks/example
[attacks/example] > show description
Module: attacks/example
	This is an example module to show the abilities of the system.
	This is some basic description text that is displayed when show description is input.
	This program will just copy the info from one file to another.
	Please define INPUTFILE for this example module to run correctly.
	Options are given below:
	| Option              | Value
	| INPUTFILE           | 
	| OUTPUTFILE          | /tmp/cryptotmpfile

[attacks/example] > set INPUTFILE README.md 
[attacks/example] > run
[*] Opening file: README.md
[*] Opening file: /tmp/cryptotmpfile
[*] Writing...
[*] Closing files
[attacks/example] > quit
```

### Installation

Dependencies:
- make
- g++ (must support c++11)

The program can be downloaded and built by running the commands:
```
git clone https://github.com/CIS4362-Group11/CryptoProject.git
cd CryptoProject
make
```
The executable will be in the current directory and can be run with `./cryptoproject`.

### Contributing

To add a module to this project the following needs to be done:
- Copy and modify `includes/attacks/example.h`
- Copy and modify `src/attacks/example.cpp`
 - It is imperative that the virtual functions defined in `includes/module.h` are overwritten by your module
- Add your new header to `includes/main.h` in the correct section towards the bottom
- Register the module in `src/main.cpp` in the function `register_modules`
 - for example: `m.insert(make_pair("attacks/mynewmodule", new MyNewModule()))`
- Copy and modify the `example.o` statement in the `makefile` to compile your individual module
