#include <vector>
#include <random>
#include <map>
#include <string>
#include <chrono>
#include "gentypes.h"

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

int randomInteger(int minimum, int maximum)
{
    std::uniform_int_distribution<int> uni(minimum,maximum);
    return uni(rng);
}

// uses a RIP set to randomly select an index which will then retrieve the corresponding choice where this function is invoked
int randomIndex(const std::vector<int>& RIP_table)
{
    int sum = 0;
    for (int i : RIP_table) sum += i;

    int selector = randomInteger(0, sum - 1);
    int ri = 0;
    while (true)
    {
        if (RIP_table[ri] <= selector)
        {
            selector -= RIP_table[ri];
            ++ri;
        }

        else return ri;
    }
}

std::vector<int> simplifyRIP(const std::vector<int>& raw_table)
{
    std::vector<int> output;
    // maximum value and thus maximum denominator
    int max = 1;
    for (int x : raw_table) if (x > max) max = x;
    // determine greatest common denominator
    int GCD = 1;
    for (int di = 2; di <= max; ++di)
        for (std::vector<int>::const_iterator i = raw_table.cbegin(); i != raw_table.cend(); ++i)
        {
            if (*i % di != 0) break; // if current value is not divisible by di then go to next di
            else if (i + 1 == raw_table.cend()) // if otherwise and its the last element then change GCD
                GCD = di;
        }

    for (int x : raw_table) output.push_back(x / GCD);
    return output;
} 

std::vector<GENSITU> RIP_names = {START, ENDPOSTV, MIDPOSTV, MIDPOSTDV, MIDPOSTR};
const int RIPn = RIP_names.size();
std::vector<std::string> situation_names = {
    "start","end post v","middle post v","middle post d and v","middle post r"};
std::map<GENSITU, std::vector<PHONETIC_FUNCTION>> RIP_options = {
    {START,     {V,I,D}},
    {ENDPOSTV,  {R,D}},
    {MIDPOSTV,  {I,D,R}},
    {MIDPOSTDV, {V,I,D}},
    {MIDPOSTR,  {V,I,D}}
};

std::map<GENSITU, std::vector<int>> RIPs;

std::vector<PHONETIC_FUNCTION> subRIP_names = {V, I, D, R};
std::vector<std::string> subsituation_names = {
    "choosing a v", "choosing a i", "choosing a d", "choosing a r"};
std::map<PHONETIC_FUNCTION, std::vector<SUBPHONF>> subRIP_options = {
    {V, {VU, VB}},
    {I, {ISG, ILP, IDS, INS, ITS, ISS}},
    {D, {DSG, DSS, DTS, DHP}},
    {R, {RKS, RRS, RMS, RPS, RNS, RLS, RSP}}};
std::map<PHONETIC_FUNCTION, std::vector<int>> subRIPs;

std::map<SUBPHONF, std::string> subRIP_option_text = 
{
    {VU, "unary v"},{VB, "binary v"},{ISG, "single i"},{ILP, "i lrwy pre"},{IDS, "i d suf"},
    {INS, "i n suf"},{ITS, "i t suf"},{ISS, "i s suf"},{DSG, "single d"},{DSS, "d s suf"},
    {DTS, "d t suf"},{DHP, "d h pre"},{RKS, "r k suf"},{RRS, "r r suf"},{RMS, "r m suf"},
    {RPS, "r p suf"},{RNS, "r n suf"},{RLS, "r l suf"},{RSP, "r s pre"}
};

std::vector<std::string>::const_iterator GSNbegin() { return situation_names.cbegin(); }
std::vector<std::string>::const_iterator GSNend() { return situation_names.cend(); }
std::vector<std::string>::const_iterator SPNbegin() { return subsituation_names.cbegin(); }
std::vector<std::string>::const_iterator SPNend() { return subsituation_names.cend(); }

void addRIP(size_t situation_index, const std::vector<int>& table)
{
    if (situation_index < RIPn)
        RIPs[RIP_names[situation_index]] = simplifyRIP(table);
    else
        subRIPs[subRIP_names[situation_index - RIPn]] = simplifyRIP(table);
}

std::vector<int> getRIP(GENSITU situation)
{
    if (RIPs.find(situation) == RIPs.end())
    {
        std::vector<int> output(RIP_options[situation].size(), 1);
        return output;
    }
    else return RIPs.at(situation);
}

std::vector<int> getSubRIP(PHONETIC_FUNCTION parent_group)
{
    if (subRIPs.find(parent_group) == subRIPs.end())
    {
        std::vector<int> output(subRIP_options[parent_group].size(), 1);
        return output;
    }
    else return subRIPs.at(parent_group);
}

size_t sizeOfRIPs() { return RIP_options.size() + subRIP_options.size(); }

size_t RIPlengthAtIndex(size_t index) 
{ 
    return (
        (index < RIPn)
            ? RIP_options[RIP_names[index]].size()
            : subRIP_options[subRIP_names[index - RIPn]].size()
    ); 
}

PHONETIC_FUNCTION randomPFunc(GENSITU situation)
{
    return (RIP_options[situation])[randomIndex(getRIP(situation))];
}

SUBPHONF randomSPhon(PHONETIC_FUNCTION parent_group)
{
    return (subRIP_options[parent_group])[randomIndex(getSubRIP(parent_group))];
}

std::string indexToRIPoptions(size_t index)
{
    std::string output;
    if (index >= RIPn) 
        for (SUBPHONF sp : subRIP_options[subRIP_names[index - RIPn]]) 
            output += (subRIP_option_text[sp] + ", ");
    else 
        for (PHONETIC_FUNCTION pf : RIP_options[RIP_names[index]]) 
            output += pf;
    return output;
}