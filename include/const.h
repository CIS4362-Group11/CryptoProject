#ifndef CONST_H
#define CONST_H

#define MAXOPTNAME 20

#define WELCOME "   ____ _               _         ____                  _\n  / ___| | __ _ ___ ___(_) ___   / ___|_ __ _   _ _ __ | |_ ___\n | |   | |/ _` / __/ __| |/ __| | |   | '__| | | | '_ \\| __/ _ \\\n | |___| | (_| \\__ \\__ \\ | (__  | |___| |  | |_| | |_) | || (_) |\n  \\____|_|\\__,_|___/___/_|\\___|  \\____|_|   \\__, | .__/ \\__\\___/\n                                            |___/|_|             \n                 _   _   _             _             \n                / \\ | |_| |_ __ _  ___| | _____ _ __ \n               / _ \\| __| __/ _` |/ __| |/ / _ \\ '__|\n              / ___ \\ |_| || (_| | (__|   <  __/ |   \n             /_/   \\_\\__|\\__\\__,_|\\___|_|\\_\\___|_|   \n                                                     "

// in case it isn't passed in makefile
#ifndef VERSION
#define VERSION 0.0
#endif

#define HELPTEXT "Available Commands:\n\tuse <module>\t\tselect a module to use\n\tshow <choice>\t\tdisplay information (modules, description, options)\n\tset <option> <value>\tset the option to the desired value\n\trun\t\t\trun the currently selected module\n\tclear\t\t\tclear the currently selected module\n\thelp\t\t\tshow this help text\n\tquit\t\t\tquit the program"

#endif
