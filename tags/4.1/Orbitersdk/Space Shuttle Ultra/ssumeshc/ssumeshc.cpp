// ssumeshc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <ctime>
#include <set>

const std::string SSUMESHC_VERSION = "0.2";

std::wstring input_file_name = L"";
std::wstring output_file_name = L".\\meshres.h";
std::string constant_prefix = "GRP_";
std::string constant_suffix = "";


struct LABEL_GROUP {
	unsigned int group_num;
	std::string label;
};

std::vector<LABEL_GROUP> labels;

const std::string& ToAlphaCode(unsigned long number)
{
	static std::string __result;
	std::string a;
	__result = "";

	if(number == 0)
		return __result;
	
	while(number>0)
	{
		if(number > 26)
		{
			a = ((char)('A' + (number%26)));
			number /= 26;
		} else {
			a = ((char)('A' + (number - 1)));
			number = 0;
		}
		
		__result = a + __result;
	}
	return __result;
}

void ParseMesh()
{
	unsigned int current_group_index = 0;
	std::ifstream mshfile;
	std::string line;
	char buffer[400];
	labels.clear();
	mshfile.open(input_file_name.c_str());
	while(mshfile)
	{
		mshfile.getline(buffer, 400);
		line.assign(buffer);
		if(line.substr(0, 5) == "LABEL") {
			LABEL_GROUP lg;
			lg.group_num = current_group_index;
			lg.label = line.substr(6);
			labels.push_back(lg);
		}
		else if(line.substr(0, 4) == "GEOM")
		{
			current_group_index++;
		}
	}
	mshfile.close();
}

void WriteHeaderFile() 
{
	
	char date_buf[400];
	time_t time_buf = time(NULL);
	ctime_s(date_buf, 400, &time_buf);

	std::ofstream hfile;
	std::set<std::string> existing_symbols;
	existing_symbols.clear();
	hfile.open(output_file_name.c_str());
	hfile << "// ======================================================" << std::endl;
	hfile << "// Created by ssumeshc " << SSUMESHC_VERSION << std::endl;
	hfile << "// Date of conversion : " << date_buf << std::endl;
	hfile << "// Input file: " << std::string(input_file_name.begin(), input_file_name.end()) << std::endl;
	hfile << "// ======================================================" << std::endl;
	hfile << std::endl;
	hfile << "#pragma once" << std::endl;
	hfile << std::endl;
	hfile << "const unsigned int NUMGRP" << constant_suffix 
		<< " = " << labels.size() << ";" << std::endl
		<< std::endl;
	for(unsigned int i = 0; i<labels.size(); i++) {
		
		std::string tmp;
		std::string::iterator iter = labels[i].label.begin();
		tmp = constant_prefix;
		while(iter != labels[i].label.end())
		{
			if(isspace(*iter))
			{
				tmp += '_';
			}
			else if(isalnum(*iter)) {
				tmp += toupper(*iter);
			} else if(*iter == '_') {
				tmp += '_';
			} else {
				//ignore
			}
			iter++;
		}

		if(existing_symbols.find(tmp) != existing_symbols.end()) {
			std::string temp;
			unsigned long attempt = 0;
			do{
				temp = tmp + ToAlphaCode(attempt);
				attempt++;
			} while(existing_symbols.find(temp) != existing_symbols.end());
			tmp = temp;
		}
		existing_symbols.insert(tmp);

		tmp.append(constant_suffix);
		hfile << "const unsigned int " << tmp << "\t=\t" << labels[i].group_num << ";"; 
		hfile << "\t//" << labels[i].label;
		hfile << std::endl;
	}
	hfile << std::endl;
	hfile.close();
}

void printhelp()
{
	std::wstring temp;
	temp.assign(SSUMESHC_VERSION.begin(), SSUMESHC_VERSION.end());
	std::wcout << "SSU Mesh labels to C++ converter. Version " 
		<< temp << "." << std::endl;
	std::wcout << "Syntax: ssumeshc [OPTIONS] FILE" << std::endl;
	std::wcout << std::endl;
	std::wcout << "OPTIONS:" << std::endl;
	std::wcout << "   -o FILE     Set output file" << std::endl; 
	std::wcout << "   -p PREFIX   Set symbol prefix" << std::endl; 
	std::wcout << "   -s SUFFIX   Set symbol suffix" << std::endl; 
	std::wcout << "   -? -h       Display this help" << std::endl;

}

int _tmain(int argc, _TCHAR* argv[])
{
	bool bValidInput = false;
	if(argc < 1)
	{
		std::wcerr << L"No mesh file given as input." << std::endl;
	}

	for(int i = 1; i<argc; i++)
	{
		if(argv[i][0] == L'-') {
			std::wstring temp;
			//Option
			switch(argv[i][1])
			{
			case L'p':
				temp.assign(argv[i]+2);
				constant_prefix = std::string(temp.begin(), temp.end());
				break;
			case L's':
				temp.assign(argv[i]+2);
				constant_suffix = std::string(temp.begin(), temp.end());
				break;
			case L'o':
				output_file_name.assign(argv[i]+2);

				break;
			case L'?':
			case L'h':
				printhelp();
				return 0;
			default:
				std::wcerr << L"ERROR: Illegal parameter : " << argv[i] << std::endl;
				printhelp();
				return 1;
			}
		} else {
			//std::wcout << "input=" << argv[i] << std::endl;
			input_file_name.assign(argv[i]);
			bValidInput = true;
		}
	}

	if(bValidInput)
	{
		std::wcout << L"Input File name: " << input_file_name.c_str() << std::endl;
		std::wcout << L"Output File name: " << output_file_name.c_str() << std::endl;
		std::wcout << L"Constant prefix: " << std::wstring(constant_prefix.begin(), constant_prefix.end()) << std::endl;
		std::wcout << L"Constant suffix: " << std::wstring(constant_suffix.begin(), constant_suffix.end()) << std::endl;

		std::wcout << L"Parse mesh file..." << std::endl;
		ParseMesh();

		WriteHeaderFile();
		return 0;
	}
	else {
		std::wcerr << L"ERROR: No input file specified." << std::endl;
		printhelp();
		return 1;
	}
	
}

