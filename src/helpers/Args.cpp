#include "helpers/Args.hpp"
#include <regex>
#include <iostream>

namespace raymarcher
{
    Arguments::Arguments()
    {
            this->argumentPrecedence = OPTIONAL;
    }
    void Arguments::First(argType type)
    {
            this->argumentPrecedence = type;
    }


    void Arguments::AddFixed(std::string name, std::string format)
    {
            this->fixedArguments.push_back(std::make_pair(name,format));
    }

    void Arguments::AddOpt(std::string name, std::string shortKey, bool hasParam, std::string format)
    {
            auto pair = std::make_pair(name,hasParam);
            this->options[shortKey] = pair;
    }
            
    void Arguments::AddLongOpt(std::string name, std::string key, std::string shortKey, bool hasParam, std::string format)
    {
            this->AddOpt(name,key,hasParam,format);
            this->AddOpt(name,shortKey,hasParam,format);
    }

    int Arguments::Parse(int argc, const char** argv)
    {
            //for(int i = 0; i < argc; ++i)
            //        std::cout << argv[i] << std::endl;

            // Skip program name
            const char ** argumentPointer = argv+1;
            
            enum automata{STEP_FIXED, STEP_OPTIONAL, STEP_END} state;

            state = STEP_OPTIONAL;
            if(this->argumentPrecedence == FIXED)
                    state = STEP_FIXED;

            while(state != STEP_END)
            {
                    switch(state)
                    {
                            case STEP_FIXED:
                            for(auto &fixedArg: this->fixedArguments)
                            {
                                    // expected fixed, got optional ?
                                    if(*argumentPointer == NULL || *argumentPointer[0] == '-')
                                    {
                                            errorMessage = std::string("Expected fixed argument '")+fixedArg.first+"'";
                                            return ARGUMENT_INVALID_FORMAT;
                                    }
                                    if(!HasFormat(fixedArg.second,*argumentPointer))
                                    {
                                            errorMessage = std::string("Format mismatch for '")+fixedArg.first+"'";
                                            return ARGUMENT_INVALID_FORMAT;
                                    }
                                    this->arguments[fixedArg.first] = *argumentPointer;
                                    argumentPointer++;
                            }
                            state = STEP_END;
                            if(this->argumentPrecedence == FIXED)
                                    state = STEP_OPTIONAL;
                            break;

                            case STEP_OPTIONAL:
                            while(*argumentPointer != NULL)
                            {
                                    if(*argumentPointer[0] == '-')
                                    {

                                            const char* omitMinus = (*argumentPointer)+1;
                                            if((*argumentPointer)[1] == '-')
                                                    omitMinus = (*argumentPointer)+2;
                                            const char* param = *(argumentPointer+1);
                                            if(HasOption(omitMinus))
                                            {
                                                    auto val = &arguments[GetOptionName(omitMinus)];
                                                    if(HasOptionParam(omitMinus))
                                                    {
                                                            // expected param
                                                            if(param == NULL)
                                                            {
                                                                    errorMessage = std::string("Expected param for option -")+omitMinus;
                                                                    return ARGUMENT_INVALID_FORMAT;
                                                            }

                                                            *val = param;
                                                            ++argumentPointer;
                                                    }
                                                    else
                                                            *val = "";
                                            } else {
                                                    errorMessage = std::string("Non-defined option '")+omitMinus+"'";
                                                    return ARGUMENT_INVALID_FORMAT;
                                            }
                                            argumentPointer++;
                                            continue;
                                    }
                                    break;
                            }
                            if(*argumentPointer != NULL)
                            {
                                    // FIXED arguments went first, optionals were
                                    // supposed to end command-line
                                    if(this->argumentPrecedence == FIXED)
                                    {
                                            errorMessage = "Expected any optional argument";
                                            return ARGUMENT_INVALID_FORMAT;
                                    }
                                    else {
                                            state = STEP_FIXED;
                                            break;
                                    }
                            } else {
                                    state = STEP_END;
                                    if(this->argumentPrecedence == OPTIONAL)
                                            state = STEP_FIXED;
                            }
                    }
            }
            if(*argumentPointer != NULL)
            {
                    errorMessage = std::string("Expected no argument, got '")+*argumentPointer;
                    return ARGUMENT_INVALID_FORMAT;
            }
            return OK;
    }



    bool Arguments::HasFormat(std::string format, std::string testedWord)
    {
            //std::cout << "Comparing " << format << " and " << testedWord << std::endl;
            // TODO:compare
            std::regex re = std::regex(format);
            std::smatch result;
            return std::regex_match(testedWord,result, re);
    }


    bool Arguments::HasOption(std::string option)
    {
            return (this->options.find(option) != this->options.end());
    }

    bool Arguments::HasOptionParam(std::string longOpt)
    {
            auto it = this->options.find(longOpt);
            if(it != this->options.end())
            {
                    return it->second.second == true;
            }
            return false;
    }

    std::string Arguments::GetOptionName(std::string opt)
    {	
            auto it = this->options.find(opt);
            if(it != this->options.end())
            {
                    return it->second.first;
            }
            return "";
    }

    std::string Arguments::operator[](std::string argument)
    {
            return this->arguments[argument];
    }


    bool Arguments::HasArgument(std::string name)
    {

            auto it = this->arguments.find(name);
            return (it != this->arguments.end());
    }


    bool Arguments::AllowRedefinition(bool allow)
    {
            this->isRedefinitionAllowed = allow;
            return true;
    }

    std::string Arguments::Value_or(std::string argument, std::string defaultValue)
    {
        return (HasArgument(argument) ? (*this)[argument] : defaultValue);
    }
}
