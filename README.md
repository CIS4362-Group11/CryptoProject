# CryptoProject
Cryptanalysis suite for classical ciphers

### Example Usage

![Example module usage image](/example.png?raw=true "Example module usage")

### Contributing

To add a module to this project the following needs to be done:
- Copy and modify `includes/attacks/example.h`
- Copy and modify `src/attacks/example.cpp`
 - It is imperative that the virtual functions defined in `includes/module.h` are overwritten by your module
- Copy and modify the `example.o` statement in the `makefile` to compile your individual module
- Register the module in `src/main.cpp` in the function `register_modules`
 - for example: `m.insert("attacks/mynewmodule", new MyNewModule())`
