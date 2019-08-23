#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <string>
#include "gentypes.h"

void generatorInterface();
void randomGenerationSuite();
void RIPinterface();
void resetRIPs();
void spellingInterface();
void printString(const std::string &message, bool suppress_break = false);
void printHelp();
void promptI(const std::string &prompt_string);
void optionsInterface();
std::string stringInput();

#endif // IO_H_INCLUDED
