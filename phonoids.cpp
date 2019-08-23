#include <string>
#include <vector>
#include <map>
#include "gentypes.h"

// checks whether x is a member of vec
bool vIN(const std::vector<std::string>& vec, const std::string& x)
{
    for (std::string i : vec) if (i == x) return true;
    return false;
}

// Cartesian product first x second
std::vector<std::string> CartProd(const std::vector<std::string>& first, const std::vector<std::string>& second)
{
    std::vector<std::string> combinations;
    for (std::string fi : first)
        for (std::string si : second)
            combinations.push_back(fi + si);
    return combinations;
}

// Cartesian product {first} x second
std::vector<std::string> CartProd(const std::string& first, const std::vector<std::string>& second)
{ 
    std::vector<std::string> temp = {first};
    return CartProd(temp, second); 
}

// returns concatenation of two vectors
std::vector<std::string> vOR(const std::vector<std::string>& v1, const std::vector<std::string>& v2)
{
    std::vector<std::string> sum;
    for (std::string member : v1) sum.push_back(member);
    for (std::string member : v2) sum.push_back(member);
    return sum;
}

// returns concatenation of string to vector
std::vector<std::string> vOR(const std::string& v1, const std::vector<std::string>& v2)
{
    std::vector<std::string> temp = {v1};
    return vOR(temp, v2); 
}

// returns vector of strings that are in 'original' but not in 'deletion'
std::vector<std::string> vNAND(const std::vector<std::string>& original, const std::string& deletion)
{
    std::vector<std::string> difference;
    for (std::string oi : original)
        if (oi != deletion) difference.push_back(oi);
    return difference;
}

typedef std::vector<std::string>* strvecptr;

std::vector<std::string> 
    i, d,
    v_un, v_bin,
    i_lrwy_p, i_d_suf, i_n_suf, i_t_suf, i_s_suf,
    d_s_suf, d_t_suf, d_h_pre,
    r_k_suf, r_r_suf, r_m_suf, r_p_suf, r_n_suf, r_l_suf, r_sz_pre;

std::vector<strvecptr> all_phonoids = {&i, &d, &v_un, &v_bin, &i_lrwy_p, &i_d_suf, 
    &i_n_suf, &i_t_suf, &i_s_suf, &d_s_suf, &d_t_suf, &d_h_pre, &r_k_suf, &r_r_suf, &r_m_suf, &r_p_suf,
    &r_n_suf, &r_l_suf, &r_sz_pre};

std::map<SUBPHONF, strvecptr> subphonoids = {
    {VU, &v_un}, {VB, &v_bin},
    {ISG, &i}, {ILP, &i_lrwy_p}, {IDS, &i_d_suf}, {INS, &i_n_suf}, {ITS, &i_t_suf}, {ISS, &i_s_suf},
    {DSG, &d}, {DSS, &d_s_suf}, {DTS, &d_t_suf}, {DHP, &d_h_pre},
    {RKS, &r_k_suf}, {RRS, &r_r_suf}, {RMS, &r_m_suf}, {RPS, &r_p_suf}, {RNS, &r_n_suf}, 
    {RLS, &r_l_suf}, {RSP, &r_sz_pre}};

strvecptr getSubGroup(SUBPHONF code) { return subphonoids[code]; }

// first map is exchange name to spellings, second map is original to replacement
std::map<std::string, std::map<std::string, std::string>> spelling_exchanges;

// checking whether phonoids need be reinitialize
bool initialization_needed = true;
void initializePhonoids()
{
    /**
    !!!!!!!!!!!!!!!!!!!!!!!
    !!!HALLO FROM SWEDEN!!!
    !!!!!!!!!!!!!!!!!!!!!!!
    HEY! make sure to change the default max phonoid length down in 'maxPhonoidLength()'
    if you tell this function to make phonoids of length > 2
    */
    if (!initialization_needed) return;
    spelling_exchanges["default"] = {};
    
    // formative phonoids and parts
    std::vector<std::string>
        rwy = {"r","w","y"},
        sz = {"s","z"},
        r_suffices = {"b","d","f","g","j","l","m","n","p","s","t","v","z"};
    // creates vowels
    v_un = {"a","e","i","o","u"}; // unary vowels
    v_bin = CartProd( v_un, v_un ); // binary vowels
    // creates initiative phonoids
    i = {"h","w","y"}; // singletons
    vOR(sz,rwy); // example
    vOR("t",sz); // example
    i_lrwy_p = CartProd( {"b","f","g","k","m","p","v","z"}, vOR( "l", rwy )); // prefices of lrwy
    i_d_suf = CartProd( "d", vNAND( rwy, "y" )); // d suffices
    i_n_suf = CartProd( "n", vNAND( rwy, "r" )); // n suffices
    i_t_suf = CartProd( "t", rwy ); // t suffices
    i_s_suf = CartProd( "s", {"l","m","n","r","v","w","y"} ); // s suffices
    // creates duplifix phonoids
    d = {"b","d","f","g","j","k","l","m","n","p","r","s","t","v","z"}; // singletons
    d_h_pre = CartProd( vOR( sz, {"c","t"}), {"h"} ); // h prefices
    d_s_suf = CartProd( "s", {"k", "p", "t"} ); // s suffices
    d_t_suf = CartProd( "t", vOR( "l", sz )); // t suffices
    // creates receptive phonoids
    r_r_suf = CartProd( "r", r_suffices); // r suffices
    r_l_suf = CartProd( "l", vNAND( r_suffices, "l" )); // l suffices
    r_n_suf = CartProd( "n", {"d","f","g","j","k","s","t","z"} ); // n suffices
    r_sz_pre = CartProd( {"b","f","d","g"}, sz ); // prefices of sz
    r_k_suf = CartProd( "k", vOR( "t", sz )); // k suffices
    r_m_suf = CartProd( "m", vOR( "p", sz )); // m suffices
    r_p_suf = CartProd( "p", vOR( sz, {"f","t"} )); // p suffices
 
    initialization_needed = false;
}

std::string exchangeSpelling(const std::vector<std::string>& word, const std::string& exchange_name)
{
    std::map<std::string, std::string>& exchange = spelling_exchanges[exchange_name];
    std::string output;
    for (std::string phonoid : word)
    {
        if (exchange.find(phonoid) != exchange.end())
            output += exchange[phonoid];
        else 
            output += phonoid;
    }
    return output;
}

void modifySpellingExchange(const std::string& exchange_name, const std::map<std::string, std::string>& exchanges)
{
    spelling_exchanges[exchange_name] = exchanges;
}

std::vector<std::string> concatAllPhonoids()
{
    std::vector<std::string> output;
    for (auto sub : all_phonoids) for (auto p : *sub) output.push_back(p);
    return output;
}

bool isPhonoid(const std::string& candidate)
{
    for (auto x : all_phonoids)
        if (vIN(*x, candidate)) return true;
    return false;
}

std::vector<std::string> getExchangeNames()
{
    std::vector<std::string> output;
    for (auto i = spelling_exchanges.cbegin(); i != spelling_exchanges.cend(); ++i)
        output.push_back( i->first );
    return output;
}

bool isExchange(const std::string& candidate)
{
    return (spelling_exchanges.find(candidate) != spelling_exchanges.end());
}

unsigned short maxPhonoidLength(const std::string& spelling = "default")
{
    if (spelling == "default") return 2;
    unsigned short maximum = 0;
    for (auto ip : spelling_exchanges[spelling])
        if (ip.second.size() > maximum) maximum = ip.second.size();
    return maximum;
}