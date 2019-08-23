#ifndef PHONOIDS_H_INCLUDED
#define PHONOIDS_H_INCLUDED
#include <vector>
#include <string>
#include <map>
#include "gentypes.h"

void initializePhonoids();
void modifySpellingExchange(const std::string& exchange_name, const std::map<std::string, std::string>& exchanges);
std::string exchangeSpelling(const std::vector<std::string>& word, const std::string& exchange_name);
std::vector<std::string>* getSubGroup(SUBPHONF code);
std::vector<std::string> concatAllPhonoids();
bool isPhonoid(const std::string& candidate);
std::vector<std::string> getExchangeNames();
bool isExchange(const std::string& candidate);
unsigned short maxPhonoidLength(const std::string& spelling = "default");

#endif // PHONOIDS_H_INCLUDED
