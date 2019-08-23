#include <vector>
#include <string>
#include "rand.h"
#include "phonoids.h"
#include "gentypes.h"

// randomly selects a phonoids from the group referred to by 'group'
std::string choosePhonoid(PHONETIC_FUNCTION group)
{
    std::vector<std::string>* choices = getSubGroup(randomSPhon(group));
    int ranPos = randomInteger(0, (*choices).size() - 1);
    return (*choices)[ranPos];
}

// randomly generates a word containing 'phonoids' phonoids
std::string generateWord(int phonoids, const std::string& spellings = "default")
{
    std::vector<std::string> output;
    PHONETIC_FUNCTION prev1; // "prev" being an abbr. of "previous"
    PHONETIC_FUNCTION prev2 = EMPTY;
    for (int i = 0; i < phonoids; ++i)
    {
        if (i == 0)
        { // START of word generation
            PHONETIC_FUNCTION group = randomPFunc(START);
            std::string selection = choosePhonoid(group);
            prev1 = group;
            output.push_back(selection);
        }
        else if (i == phonoids - 1)
        { // end of word
            if (prev1 == V)
                output.push_back(choosePhonoid(randomPFunc(ENDPOSTV)));
            else
                output.push_back(choosePhonoid(V));
        }
        else
        { // middle of word
            PHONETIC_FUNCTION group;
            switch (prev1)
            {
            case V:
                group = randomPFunc(MIDPOSTV);
                break;
            case I:
                group = V;
                break;
            case D:
                if (prev2 == V) group = randomPFunc(MIDPOSTDV);
                else group = V;
                break;
            case R:
                group = randomPFunc(MIDPOSTR);
                break;
            }
            prev2 = prev1, prev1 = group;
            std::string selection = choosePhonoid(group);
            output.push_back(selection);
        }
    }
    return exchangeSpelling(output, spellings);
}
