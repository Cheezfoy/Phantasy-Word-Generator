#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <map>
#include "gen.h"
#include "rand.h"
#include "phonoids.h"
#include "gentypes.h"

const std::string IPROMPT = " > ";
std::vector<std::string> COMMAND_HELP = {
    "g: generate",
    "G: random generation suite",
    "r: modify RIPs",
    "R: make RIPs ultimately uniform",
    "s: modify spelling exchanges",
    "o: options",
    "x: exit",
    "for best experience, use Fira Code Medium fontface at size 20" };

bool columnar_output = true;

const short WINDOW_CHAR_WIDTH = 134;

void printString(const std::string &message, bool suppress_break = false) 
{ 
	std::cout << message;
    if (!suppress_break) std::cout << std::endl;
}

void hsplit(short length = WINDOW_CHAR_WIDTH / 2)
{
    while (length-- > 0) std::cout << '=';
    std::cout << std::endl;
}
void printHelp() { for (auto s : COMMAND_HELP) printString(s); }
void promptI(const std::string &prompt_string = IPROMPT) { printString(prompt_string, true); }

void columnsPrint(const std::vector<std::string>& tokens, short max_size, short rbuffer = 3)
{
    short columns = WINDOW_CHAR_WIDTH / (max_size + rbuffer);
    for (int i = 0; i < tokens.size(); ++i)
    {
        std::cout << tokens[i];
        for (short fill = rbuffer + max_size - tokens[i].size(); 
             fill > 0; --fill) std::cout << ' ';
        if ((i + 1) % columns == 0 || i + 1 == tokens.size()) 
            std::cout << std::endl;
    }
}

void flushI()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int intInput(int minimum = INT_MIN, int maximum = INT_MAX, bool flush_after = false)
{
    while (true)
    {
        int i;
        std::cin >> i;
        if (std::cin.fail() || i < minimum || i > maximum)
        {
            printString("Invalid input; retry: ");
            flushI();
        }
        else 
        {
            if (flush_after) flushI();
            return i;
        }
    }
}

bool booleanInput(bool flush_after = false) { return intInput(0, 1, flush_after); }

std::string stringInput()
{
	std::string input_string;
	std::cin >> input_string;
    flushI();
	return input_string;
}

void generatorInterface()
{
	promptI("# of words and # of phonoids?  ");
	int words = intInput(1, INT_MAX),
		length = intInput(1, INT_MAX, true);

    promptI("Use spelling exchange? (0/1)  ");
    bool use_spex = booleanInput(true);
    std::string spelling = "default";
    if (use_spex)
    {
        std::vector<std::string> names = getExchangeNames();
        printString("Spelling exchange names:");
        for (std::string n : names) printString(n);
        promptI("Enter name of exchange:  ");
        while (true)
        {
            spelling = stringInput();
            if (isExchange(spelling)) break; // followed by implicit else
            printString("That is not a valid exchange name.");
        }
    }

    if (columnar_output)
    {
        // this seems like a memory hog...refactor approach?
        // option: split up batches; make the splitting toggleable
        std::vector<std::string> output;

        int tenth = (words < 10) ? 1 : words / 10;

        // there should be 20 spaces between the " and the | in the following string
        printString("                    |\nLoading...", true);
        for (int i = 0; i < words; ++i) 
        {
            output.push_back(generateWord(length, spelling));
            if (i % (tenth) == 0)
                printString(".", true);
        }
        printString(""); // newline after the loading bar is done
        unsigned short max_length = length * maxPhonoidLength(spelling);
        hsplit();
        columnsPrint(output, max_length);
        hsplit();        
    }
    else
    {
        hsplit();
        while (words-- > 0) printString(generateWord(length, spelling));
        hsplit();
    }

}

void randomGenerationSuite()
{
    printString("Enter the maximum probability integer:");
    promptI(" [1,100]: ");
    int max_precision = intInput(1,100);
    for (short i = 0; i < sizeOfRIPs(); ++i)
    {
        std::vector<int> random_RIP;
        for (short j = 0; j < RIPlengthAtIndex(i); ++j)
            random_RIP.push_back(randomInteger(0, max_precision));
        addRIP(i, random_RIP);
    }
    printString("All RIP tables randomized.");
    generatorInterface();
}

void RIPinterface()
{
	printString("\nEnter the number of the RIP you wish to modify:");
    int gsni = 0;
    printString("-1: Stop input");
    for (auto i = GSNbegin(); i != GSNend(); ++i)
    {
        std::cout << gsni << ":  " << *i << " --- " << indexToRIPoptions(gsni) << std::endl; 
        ++gsni;
    }
    for (auto i = SPNbegin(); i != SPNend(); ++i)
    {
        std::cout << gsni << ":  " << *i << " --- " << indexToRIPoptions(gsni) << std::endl; 
        ++gsni;
    }
    while (true)
    {
        promptI(" R:  ");
        short i_selection = intInput(-1, sizeOfRIPs() - 1, true);
        
        if (i_selection == -1) break;

        printString("\nNow enter the values for the RIP, in sequence:");
        std::vector<int> new_RIP;
        while (true)
        {
            new_RIP = {};
            unsigned int RIP_sum = 0;
            for (int i = 0; i < RIPlengthAtIndex(i_selection); ++i)
            {
                unsigned int iin = intInput(0, INT_MAX);
                new_RIP.push_back(iin);
                RIP_sum += iin;
            }
            flushI();
            if (RIP_sum > 0) break;
            else printString("RIP tables that sum to zero are invalid.");
        }
        addRIP(i_selection, new_RIP);
        printString("\nRIP write completed.");
    }
}

// rtfuu = rip table for ultimate uniformity
std::vector<int> VID_junction_indeces = {0, 3, 4}, // the indeces of junctions with options v, i, d in rand::RIP_options
    VID_RTFUU = {1, 3, 2}, // RTFUU for decision junctions which have the option to choose the v, i, or d phonoid classes
    epv_RTFUU = {4, 7}, // RTFUU for the ENDPOSTV generation situation (or decision junction) (index 1 in rand::RIP_options)
    mpv_RTFUU = {6, 4, 7}; // RTFUU for the MIDPOSTV generation situation (index 2 in rand::RIP_options)

void resetRIPs()
{
    for (int i : VID_junction_indeces) addRIP(i, VID_RTFUU);

    addRIP(1, epv_RTFUU);
    addRIP(2, mpv_RTFUU);

    for (short i = 5; i < sizeOfRIPs(); ++i)
    {
        std::vector<int> uniform(RIPlengthAtIndex(i), 1);
        addRIP(i, uniform);
    }
    printString("all RIP tables are now ultimately uniform");
}

void spellingInterface()
{
    promptI("\nEnter name of exchange set:  ");
    std::string exchange_name = stringInput();

    std::map<std::string, std::string> temp_exchanges;
    columnsPrint(concatAllPhonoids(), maxPhonoidLength(), 1);
    printString("Enter a phonoid to replace, followed by your replacement (enter \"x\" to stop):");
    while (true)
    {
        promptI(" -  ");
        const std::string original = stringInput();
        if (original == "x") break;
        else if (!isPhonoid(original))
        {
            printString("That is not one of the listed phonoids.");
            continue;
        }
        else
        {
            promptI(" =  ");
            const std::string replacement = stringInput();
            temp_exchanges[original] = replacement;
        }
    }
    modifySpellingExchange(exchange_name, temp_exchanges);
    printString("Spelling exchange write completed for ", true);
    printString(exchange_name);
}

void optionsInterface()
{
    printString("Choose an option to modify:\n0: Back\n1: Toggle columnar output");
    promptI(" :  ");
    bool optioni = booleanInput(true);
    if (optioni)
    {
        promptI(" 0/1:  ");
        columnar_output = booleanInput(true);
    }

}