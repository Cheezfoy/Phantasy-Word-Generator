#ifndef RAND_H_INCLUDED
#define RAND_H_INCLUDED
#include "gentypes.h"
#include <vector>
#include <string>

int randomInteger(int minimum, int maximum);
int randomIndex(const std::vector<int>& RIP_table);
PHONETIC_FUNCTION randomPFunc(GENSITU situation);
SUBPHONF randomSPhon(PHONETIC_FUNCTION parent_group);
std::vector<std::string>::const_iterator GSNbegin();
std::vector<std::string>::const_iterator GSNend();
std::vector<std::string>::const_iterator SPNbegin();
std::vector<std::string>::const_iterator SPNend();
size_t sizeOfRIPs();
size_t RIPlengthAtIndex(size_t index);
void addRIP(size_t situation_index, const std::vector<int>& table);
std::string indexToRIPoptions(size_t index);
#endif // RAND_H_INCLUDED
