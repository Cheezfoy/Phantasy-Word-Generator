#include <string>
#include <iostream>
#include "phonoids.h"
#include "IO.h"

int main()
{
    // strings pertaining to the title of the application
    const std::string
        compile_date     = __DATE__,
        compile_time     = __TIME__,
        program_title    = "Phantasy Word Generator",
        version_nickname = "SMS Novara",
        version_number   = "8.3",
        vdiv             = "..";

    // determining size of horizontal split and creating it
    size_t title_width = 26 + program_title.size() + version_nickname.size() + version_number.size();
    std::string hsplit(title_width, '=');

    // creating the text for the title of the application
    const std::string title_text =
        hsplit           + '\n' +
        program_title    + ' '  +
        version_number   + vdiv +
        version_nickname + vdiv +
        compile_date     + "--" + compile_time +
        "\nby R.H.Scriba, programmed in C++\n" +
        "enter \"h\" for help\n" +
        hsplit;
    printString("Generating pronounceable phonoids...");
    initializePhonoids(); // creating phonoids
    printString(title_text); // informing the user what they've executed

    bool continue_input = true;
    while (continue_input)
    {
        promptI(" > ");
        std::string command_input = stringInput();
        switch (command_input[0])
        {
        case 'g':
            generatorInterface(); break;
        case 'G':
            randomGenerationSuite(); break;
        case 'h':
            printHelp(); break;
        case 'x':
            continue_input = false; break;
        case 'r':
            RIPinterface(); break;
        case 'R':
            resetRIPs(); break;
        case 's':
        	spellingInterface(); break;
        case 'o':
            optionsInterface(); break;
        default:
            printString("Invalid command; reenter, buster.");
            continue; break;
        }
    }
}

