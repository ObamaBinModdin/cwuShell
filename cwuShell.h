#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <fstream>

class cwuShell
{
	public:
		cwuShell();
		
	private:
		static std::string promptString;
		static int lastCommand;

		static void exitShell(int parameter = 0);
		static std::vector<std::string> split(std::string str, char delim = ' ');
		static std::vector<char> splitToCharacters(std::string str);
		static void removeDoubleSpaces(std::string& str);
		static void trim(std::string& str);
		static void setPrompt(std::string newPrompt = std::string());
		static std::string getPrompt();
		static void cpuinfo(bool switchCBool = false, bool switchTBool = false, bool switchNBool = false);
		static void printCPUClock();
		static void printCPUType();
		static void printCPUCores();
		static void meminfo(bool switchTBool = false, bool switchUBool = false, bool switchCBool = false);
		static void printRAMAvailable();
		static void printRAMUsed();
		static void printL2Size();
		static void linuxCommands(std::string command);
		static void exitHelp();
		static void promptHelp();
		static void cpuinfoHelp();
		static void meminfoHelp();
		static void printManual();
};

