#include "cwuShell.h"
#include<unistd.h> 

/*Initializes global class variables of promptString and lastCommand.*/
std::string cwuShell::promptString = "cwushell>";
int cwuShell::lastCommand = 0;

/*Prints the prompt in a blue color. Waits for a valid input from the user. Removes doubles spaces in the user's input.
Removes any leading or trailing whitespace. Tokenizes user's inputs and verifys they are acceptable. If so then runs the given
command if available*/
cwuShell::cwuShell()
{
	while(1)
	{	//Prints the prompt with a light blue color.
		std::cout << "\033[1;36m" << cwuShell::getPrompt() << "\033[0m";
	
		//Gets user's input and trims it.
		std::string userInputString;
		std::getline(std::cin, userInputString);
		cwuShell::removeDoubleSpaces(userInputString);
		cwuShell::trim(userInputString);

		//Allows user to enter nothing but does nothing. It just starts the loop again.
		if (userInputString.empty()) continue;
		
		//Splits the user's input at a space and stores in a vector that holds strings.
		std::vector<std::string> userInputVector = cwuShell::split(userInputString);
		
		//If the input is "exit" then enter statement.		
		if (userInputVector.at(0).compare("exit") == 0)
		{
			//Enter statement if no parameter was given.
			if (userInputVector.size() == 1)
			{
				//Exits shell.
				cwuShell::exitShell(lastCommand);
			}
			//If there was a parameter then enter statement.
			else if (userInputVector.size() == 2)
			{

				try
				{	
					//If the parameter is "-h" or "-help" then enter statement.
					if (userInputVector.at(1) == "-h" || userInputVector.at(1) == "-help")
					{
						//Calls manual for exit.
						cwuShell::exitHelp();
					}
					//Enters statement if not "-h" or "-help".
					else
					{
						//Exits based on the entered exit code.
						cwuShell::exitShell(stoi(userInputVector.at(1)));
					}
				}
				//Catches any exception thrown like a string entered instead of a number for a parameter.
				catch(...)
				{
					//Saves the exit code and displays error in orange to console.
					lastCommand = 132;
					std::cout << "\033[91merror: unknown parameter type.\033[0m" << std::endl;
				}
			}
			//Enter statement if more than one parameter entered.
			else
			{
				//Save exit code and throw error.
				lastCommand = 132;
				std::cout << "\033[91merror: too many parameters.\033[0m" << std::endl;
			}
		}
		//Enter statement if "prompt" was entered by the user.
		else if (userInputVector.at(0).compare("prompt") == 0)
		{
			//Sets exit code to 0.
			lastCommand = 0;
			
			//Enters if no paramters.
			if (userInputVector.size() == 1)
			{
				//Sets prompt to default.
				cwuShell::setPrompt();
			}
			//Enters if a parameter is given.
			else if (userInputVector.size() == 2)
			{
				//Enters statement if user entered "-h" or "-help" as parameters.
				if (userInputVector.at(1) == "-h" || userInputVector.at(1) == "-help")
				{
					//Prints prompt manual.
					cwuShell::promptHelp();
				}
				//Enters statement if parameter is not "-h" or "-help".
				else
				{
					//Changes the prompt to user's input.
					cwuShell::setPrompt(userInputVector.at(1));
				}
			}
			//Enter if more than one parameter was entered.
			else
			{
				//Set exit code and throw error in orange,
				lastCommand = 132;
				std::cout << "\033[91merror: too many parameters\033[0m" << std::endl;
			}
		}
		//Enters statement if user entered "cputinfo".
		else if (userInputVector.at(0).compare("cpuinfo") == 0)
		{
			//Flags for switches or unknown switches.
			bool switchCBool = false;
			bool switchTBool = false;
			bool switchNBool = false;
			bool falseStatement = false;
			lastCommand = 0;
			
			//If no parameter then print manual.
			if (userInputVector.size() == 1) cwuShell::cpuinfoHelp();
			
			//Loop through vector to find switches.
			for (unsigned long int index = 1; index < userInputVector.size(); ++index)
			{
				//Prints manual if "-h" or "-help" is entered.
				if (userInputVector.at(index) == "-h" || userInputVector.at(index) == "-help")
				{
					cwuShell::cpuinfoHelp();
				}
				//Flips booleans to true if their respective switch is entered.
				else if (userInputVector.at(index) == "-c") switchCBool = true;
				else if (userInputVector.at(index) == "-t") switchTBool = true;
				else if (userInputVector.at(index) == "-n") switchNBool = true;
				//Enter if multiple switches are in one dash.
				else if (userInputVector.at(index).length() >= 3 && userInputVector.at(index)[0] == '-')
				{
					std::vector<char> switchVector = cwuShell::splitToCharacters(userInputVector.at(index));
					
					//Loops through to find all possible switches.
					for (unsigned long int switchIndex = 1; switchIndex < switchVector.size(); ++switchIndex)
					{
						if (switchVector.at(switchIndex) == 'c') switchCBool = true;
						else if (switchVector.at(switchIndex) == 't') switchTBool = true;
						else if (switchVector.at(switchIndex) == 'n') switchNBool = true;
						else if (switchVector.at(switchIndex) == 'h') cwuShell::cpuinfoHelp();
						//Enters if no switch exits.
						else
						{
							std::cout << "\033[91m'" + userInputVector.at(index) + "'" + " is an invalid command.\033[0m" << std::endl;
							falseStatement = true;
							break;
						}
					}
				}
				else if (falseStatement) break;
				//Enter if invalid command.
				else
				{
					//Sets falseStatement to true and prints error.
					std::cout << "\033[91m'" + userInputVector.at(index) + "'" + " is an invalid command.\033[0m" << std::endl;
					falseStatement = true;
					break;
				}
			}
			//Prints info if falseStatement is false.
			if (falseStatement != true)
			{
				cwuShell::cpuinfo(switchCBool, switchTBool, switchNBool);
			}
		}
		//Enters if user's input starts with "meminfo".
		else if (userInputVector.at(0).compare("meminfo") == 0)
		{
			//Flags for switches or unknown switches.
			bool switchTBool = false;
			bool switchUBool = false;
			bool switchCBool = false;
			bool falseStatement = false;
			lastCommand = 0;
			
			//If no parameter then print manual.
			if (userInputVector.size() == 1) cwuShell::meminfoHelp();
			
			//Loop through vector to find switches.
			for (unsigned long int index = 1; index < userInputVector.size(); ++index)
			{
				//Prints manual if "-h" or "-help" is entered.
				if (userInputVector.at(index) == "-h" || userInputVector.at(index) == "-help")
				{
					cwuShell::meminfoHelp();
				}
				//Flips booleans to true if their respective switch is entered.
				else if (userInputVector.at(index) == "-t") switchTBool = true;
				else if (userInputVector.at(index) == "-u") switchUBool = true;
				else if (userInputVector.at(index) == "-c") switchCBool = true;
				//Enter if multiple switches are in one dash.
				else if (userInputVector.at(index).length() >= 3 && userInputVector.at(index)[0] == '-')
				{
					std::vector<char> switchVector = cwuShell::splitToCharacters(userInputVector.at(index));
					
					//Loops through to find all possible switches.
					for (unsigned long int switchIndex = 1; switchIndex < switchVector.size(); ++switchIndex)
					{
						if (switchVector.at(switchIndex) == 't') switchTBool = true;
						else if (switchVector.at(switchIndex) == 'u') switchUBool = true;
						else if (switchVector.at(switchIndex) == 'c') switchCBool = true;
						else if (switchVector.at(switchIndex) == 'h') cwuShell::meminfoHelp();
						//Enters if no switch exits.
						else
						{
							std::cout << "\033[91m'" + userInputVector.at(index) + "'" + " is an invalid command.\033[0m" << std::endl;
							falseStatement = true;
							break;
						}
					}
				}
				else if (falseStatement) break;
				//Enter if invalid command.
				else
				{
					//Sets falseStatement to true and prints error.
					lastCommand = 127;
					std::cout << "\033[91m'" + userInputVector.at(index) + "'" + " is an invalid command.\033[0m" << std::endl;
					falseStatement = true;
					break;
				}
			}
			
			//Prints info if falseStatement is false.
			if (falseStatement != true)
			{
				cwuShell::meminfo(switchTBool, switchUBool, switchCBool);
			}
		}
		//Prints lastCommand if user enters, "echo $?".
		else if (userInputString.compare("echo $?") == 0)
		{
			std::cout << lastCommand << std::endl;
			lastCommand = 0;
		}
		//Prints whole manual if user enters, "manual".
		else if	(userInputString.compare("manual") == 0) 
		{
			cwuShell::printManual();
		}
		//Tries to execute Linux commands if not present in this shell.
		else
		{
			cwuShell::linuxCommands(userInputString);
		}
	}
}

/*Passes in an exit code and tries to exit based on that parameter.*/
void cwuShell::exitShell(int parameter)
{
	try
	{
		exit(parameter);
	}
	catch(...)
	{
		lastCommand = 132;
		exit(132);
	}
}

/*Splits a string into a vector based on the passed in char as a delimitter.*/
std::vector<std::string> cwuShell::split(std::string str, char delim) 
{
	std::vector<std::string> returnVector;

	try
	{
		std::stringstream ss(str);
		std::string token;
	
		while (std::getline(ss, token, delim))
		{
			returnVector.push_back(token);
		}
	}
	catch(...)
	{
		lastCommand = 1;
		std::cout << "\033[91mAn error has occured\033[0m" << std::endl;
	}
	
	return returnVector;
}

/*Splits a string to a vector of characters and returns the vector.*/
std::vector<char> cwuShell::splitToCharacters(std::string str)
{
	std::vector<char> returnVector(str.begin(), str.end());
	
	return returnVector;
}

/*Removes all doubleSpaces from a string.*/
void cwuShell::removeDoubleSpaces(std::string& str)
{
	std::size_t doubleSpace = str.find("  ");
	
	while (doubleSpace != std::string::npos)
	{
		str.erase(doubleSpace, 1);
		doubleSpace = str.find("  ");
	}
}

/*Removes all leading and trailing whitespace of the passed in string.*/
void cwuShell::trim(std::string& str)
{
	const char* toDelete = " \t\n\r\f\v";
	
	str.erase(0, str.find_first_not_of(toDelete));
	str.erase(str.find_last_not_of(toDelete) + 1);
}

/*Changes the prompt to the passed in prompt. If none passed in then return to default prompt.*/
void cwuShell::setPrompt(std::string newPrompt)
{
	if (newPrompt.empty())
	{
		promptString = "cwushell>";
	}
	else
	{
		promptString = newPrompt;
	}
}

/*Returns a string of the current prompt.*/
std::string cwuShell::getPrompt()
{
	return promptString;
}

/*Handles what switches to print for cpuinfo.*/
void cwuShell::cpuinfo(bool switchCBool, bool switchTBool, bool switchNBool)
{
	if (switchCBool) cwuShell::printCPUClock();
	
	if (switchTBool) cwuShell::printCPUType();
	
	if (switchNBool) cwuShell::printCPUCores();
}

/*Prints the CPU clock speed in MHz.*/
void cwuShell::printCPUClock()
{
	std::ifstream cpuFile;
	
	try
	{
		cpuFile.open("/proc/cpuinfo", std::ifstream::in);
	
		std::string cpuSpeed;
	
		while (std::getline(cpuFile, cpuSpeed))
		{
			if (cpuSpeed.find("cpu MHz") != std::string::npos)
			{
				std::vector<std::string> speedLineVector = cwuShell::split(cpuSpeed);
				cpuSpeed = speedLineVector.at(speedLineVector.size() - 1);
				break;
			}
		}
	
		cpuFile.close();

		std::cout << "CPU clock speed: " + cpuSpeed + " MHz" << std::endl;
	}
	catch(const std::ifstream::failure& fail)
	{
		std::cerr << "\033[91m" << fail.what() << "\033[0m" << std::endl;
	}
}

/*Prints the CPU type.*/
void cwuShell::printCPUType()
{
	std::ifstream cpuFile;
	
	try
	{
		cpuFile.open("/proc/cpuinfo", std::ifstream::in);
	
		std::string cpuType;
	
		while (std::getline(cpuFile, cpuType))
		{
			if (cpuType.find("model name") != std::string::npos)
			{
				std::vector<std::string> typeLineVector = cwuShell::split(cpuType, ':');
				cpuType = typeLineVector.at(typeLineVector.size() - 1);
				break;
			}
		}
	
		cpuFile.close();
	
		std::cout << "CPU type:" + cpuType << std::endl;
	}
	catch(const std::ifstream::failure& fail)
	{
		std::cerr << "\033[91m" << fail.what() << "\033[0m" << std::endl;
	}
}

/*Prints the number of cores in the CPU.*/
void cwuShell::printCPUCores()
{
	std::ifstream cpuFile;
	
	try
	{
		cpuFile.open("/proc/cpuinfo", std::ifstream::in);
	
		std::string cpuCores;
	
		while (std::getline(cpuFile, cpuCores))
		{
			if (cpuCores.find("cpu cores") != std::string::npos)
			{
				std::vector<std::string> coresLineVector = cwuShell::split(cpuCores);
				cpuCores = coresLineVector.at(coresLineVector.size() - 1);
				break;
			}
		}
	
		cpuFile.close();
	
		std::cout << "CPU cores: " + cpuCores << std::endl;
	}
	catch(const std::ifstream::failure& fail)
	{
		std::cerr << "\033[91m" << fail.what() << "\033[0m" << std::endl;
	}
}

/*Handles which switches to print from meminfo.*/
void cwuShell::meminfo(bool switchTBool, bool switchUBool, bool switchCBool)
{
	if (switchTBool) cwuShell::printRAMAvailable();
	
	if (switchUBool) cwuShell::printRAMUsed();
	
	if (switchCBool) cwuShell::printL2Size();
}

/*Prints the number of bytes available in RAM in bytes.*/
void cwuShell::printRAMAvailable()
{
	std::ifstream memFile;
	
	try
	{
		memFile.open("/proc/meminfo", std::ifstream::in);
	
		std::string ramAvailable;
	
		while (std::getline(memFile, ramAvailable))
		{
			if (ramAvailable.find("MemAvailable") != std::string::npos)
			{
				std::vector<std::string> ramLineVector = cwuShell::split(ramAvailable);
				ramAvailable = ramLineVector.at(ramLineVector.size() - 2);
				break;
			}
		}
	
		memFile.close();
	
		unsigned long long ram = (std::stoull(ramAvailable, NULL, 0) * 1024);
	
		std::cout << "Memory available: " << ram << " bytes" << std::endl;
	}
	catch(const std::ifstream::failure& fail)
	{
		std::cerr << "\033[91m" << fail.what() << "\033[0m" << std::endl;
	}
}

/*Prints the amount of bytes used in RAM.*/
void cwuShell::printRAMUsed()
{
	std::ifstream memFile;
	
	try
	{
		memFile.open("/proc/meminfo", std::ifstream::in);
	
		std::string ramTotal;
		std::string ramAvailable;
	
		while (std::getline(memFile, ramAvailable))
		{
			if (ramAvailable.find("MemTotal") != std::string::npos)
			{
				std::vector<std::string> ramLineVector = cwuShell::split(ramAvailable);
				ramTotal = ramLineVector.at(ramLineVector.size() - 2);
			}
			else if (ramAvailable.find("MemAvailable") != std::string::npos)
			{
				std::vector<std::string> ramLineVector = cwuShell::split(ramAvailable);
				ramAvailable = ramLineVector.at(ramLineVector.size() - 2);
				break;
			}
		}
	
		memFile.close();
	
		unsigned long long ram = ((std::stoull(ramTotal, NULL, 0) - std::stoull(ramAvailable, NULL, 0)) * 1024);
	
		std::cout << "Memory used: " << ram << " bytes" << std::endl;
	}
	catch(const std::ifstream::failure& fail)
	{
		std::cerr << "\033[91m" << fail.what() << "\033[0m" << std::endl;
	}
}

/*Prints the L2 cache size in bytes.*/
void cwuShell::printL2Size()
{
	std::ifstream memFile;
	
	try
	{
		memFile.open("/sys/devices/system/cpu/cpu0/cache/index2/size", std::ifstream::in);
	
		std::string l2Size;
		std::getline(memFile, l2Size);
	
		memFile.close();
	
		l2Size = l2Size.substr(0, l2Size.length() - 1);
	
		unsigned long long size = std::stoull(l2Size, NULL, 0)  * 1024;
	
		std::cout << "L2 cache size: " << size << " bytes" << std::endl;
	}
	catch(const std::ifstream::failure& fail)
	{
		std::cerr << "\033[91m" << fail.what() << "\033[0m" << std::endl;
	}
}

/*Handles all available Linux commands.*/
void cwuShell::linuxCommands(std::string command)
{
	try
	{	
		lastCommand = std::system(command.c_str());
	}
	catch(...)
	{
		std::cout << "\033[91mSomething went wrong...\033[0m" << std::endl;
	}
}

/*Prints Exit manual.*/
void cwuShell::exitHelp()
{
	std::cout << "NAME\n";
	std::cout << "\texit - Exits the cwuShell.\n\n";
	
	std::cout << "SYNOPSIS\n";
	std::cout << "\texit [EXIT_CODE]\n\n";

	std::cout << "DESCRIPTION\n";
	std::cout << "\texits the cwuShell.\n\n";

	std::cout << "\tThe exit code or exit state to exit the cwuShell in.\n\n";
	
	std::cout << "\tIf no parameter is given, then the cwuShell will exit with the exit code of the previous "
		<< "\n\texecuted command or with 0 if no previous commands have been executed.\n\n";
		
	std::cout << "AUTHOR\n";
	std::cout << "\tOscar H. Ford\n";
}

/*Prints Prompt manual.*/
void cwuShell::promptHelp()
{
	std::cout << "NAME\n";
	std::cout << "\tprompt - changes the prompt as it appears in the cwuShell.\n\n";
	
	std::cout << "SYNOPSIS\n";
	std::cout << "\tprompt [NEW_PROMPT]\n\n";
	
	std::cout << "DESCRIPTION\n";
	std::cout << "\tThe new prompt that will be displayed in cwuShell.\n\n";
	
	std::cout << "\tIf no parameter is given, then the cwuShell will default to 'cwuShell>'.\n\n";
	
	std::cout << "AUTHOR\n";
	std::cout << "\tOscar H. Ford\n";
}

/*Prints cpuinfo manual.*/		
void cwuShell::cpuinfoHelp()
{
	std::cout << "NAME\n";
	std::cout << "\tcpuinfo - displays information on the CPU\n\n";	
	
	std::cout << "SYNOPSIS\n";
	std::cout << "\tcpuinfo [OPTION][OPTION][OPTION]...\n\n";

	std::cout << "DESCRIPTION\n";
	std::cout << "\tPrints to console the clock speed, CPU type, and the number of cores\n\tin the CPU.\n\n";
	
	std::cout << "\t-c\tWill print to console the clock speed in MHz.\n";
	std::cout << "\t-t\tWill print to console the CPU type.\n";
	std::cout << "\t-n\tWill print to console the number of cores that reside in the CPU.\n";
	
	std::cout << "\tDefault is to print to console this help statement again when no arguments are given.\n\n";
	
	std::cout << "AUTHOR\n";
	std::cout << "\tOscar H. Ford\n";	
}

/*Prints meminfo manual.*/
void cwuShell::meminfoHelp()
{
	std::cout << "NAME\n";
	std::cout << "\tmeminfo - displays information on the memory and RAM\n\n";	
	
	std::cout << "SYNOPSIS\n";
	std::cout << "\tmeminfo [OPTION][OPTION][OPTION]...\n\n";
	
	std::cout << "DESCRIPTION\n";
	std::cout << "\tPrints to console the available RAM memory in bytes,\n\tthe used RAM memory in bytes, and the size of the \n\tL2 cache in bytes.\n\n";
	
	std::cout << "\t-t\tWill print to console the available RAM memory in bytes.\n\n";
	std::cout << "\t-u\tWill print to console the used RAM memory in bytes.\n\n";
	std::cout << "\t-c\tWill print to console the size of the L2 cache in bytes.\n\n";
	
	std::cout << "\tDefault is to print to console this help statement again when no arguments are given.\n\n";
	
	std::cout << "AUTHOR\n";
	std::cout << "\tOscar H. Ford\n";
}

/*Prints all manuals.*/
void cwuShell::printManual()
{
	cwuShell::exitHelp();
	std::cout << "\n--------------------------------------------------------------------------------------------------\n\n";

	cwuShell::promptHelp();
	std::cout << "\n--------------------------------------------------------------------------------------------------\n\n";

	cwuShell::cpuinfoHelp();
	std::cout << "\n--------------------------------------------------------------------------------------------------\n\n";
	
	cwuShell::meminfoHelp();
	std::cout << "\n--------------------------------------------------------------------------------------------------\n\n";

	std::cout << "More commands are available through Linux...\n";
}
