#ifndef MAIN_H
#define MAIN_H

/* define standard library inclusions here */
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <ctype.h>
#include <string.h>

using namespace std;

/* all modules need these files
 * no created files should go above these
 * following includes
 */
#include "const.h"
#include "module.h"

/* define inclusions of modules here */
#include "attacks/example.h"
#include "attacks/histogram.h"
#include "attacks/ngram.h"
#include "attacks/caesar_attack.h"
#include "attacks/kasiski.h"
#include "ciphers/caesar.h"
#include "ciphers/vigenere.h"

#endif
