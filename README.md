# CryptoProject
Cryptanalysis suite for classical ciphers

### Example Usage

![Example module usage image](/example.png?raw=true "Example module usage")

### Installation

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
