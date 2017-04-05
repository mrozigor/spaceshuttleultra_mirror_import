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

const std::string SSUMESHC_VERSION = "0.3";

std::wstring input_file_name = L"";
std::wstring output_file_name = L".\\meshres.h";
std::string group_prefix = "GRP_";
std::string group_suffix = "";
std::string material_prefix = "MAT_";
std::string material_suffix = "";
std::string texture_prefix = "TEX_";
std::string texture_suffix = "";


struct ITEM {
	unsigned int num;
	std::string name;
};

std::vector<ITEM> groups;
std::vector<ITEM> materials;
std::vector<ITEM> textures;

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

void ParseMesh( bool mat, bool tex )
{
	unsigned int current_group_index = 0;
	std::ifstream mshfile;
	std::string line;
	char buffer[400];
	groups.clear();
	mshfile.open(input_file_name.c_str());
	while(mshfile)
	{
		mshfile.getline(buffer, 400);
		line.assign(buffer);
		if(line.substr(0, 5) == "LABEL") {
			ITEM lg;
			lg.num = current_group_index;
			lg.name = line.substr(6);
			groups.push_back(lg);
		}
		else if(line.substr(0, 4) == "GEOM")
		{
			current_group_index++;
		}
		else if (mat && (line.substr( 0, 9 ) == "MATERIALS"))
		{
			unsigned int count = stoi( line.substr( 10 ) );
			for (unsigned int i = 0; mshfile && (i < count); i++)
			{
				mshfile.getline( buffer, 400 );
				line.assign( buffer );

				materials.push_back( ITEM{ i, line } );
			}
		}
		else if (tex && (line.substr( 0, 8 ) == "TEXTURES"))
		{
			unsigned int count = stoi( line.substr( 9 ) );
			for (unsigned int i = 1; mshfile && (i <= count); i++)
			{
				mshfile.getline( buffer, 400 );
				line.assign( buffer );

				textures.push_back( ITEM{ i, line } );
			}
		}
	}
	mshfile.close();
}

void WriteHeaderFile( bool mat, bool tex ) 
{
	
	char date_buf[400];
	time_t time_buf = time(NULL);
	ctime_s(date_buf, 400, &time_buf);

	std::ofstream hfile;
	std::set<std::string> existing_symbols_groups;
	std::set<std::string> existing_symbols_materials;
	std::set<std::string> existing_symbols_textures;
	existing_symbols_groups.clear();
	existing_symbols_materials.clear();
	existing_symbols_textures.clear();
	hfile.open(output_file_name.c_str());
	hfile << "// ======================================================" << std::endl;
	hfile << "// Created by ssumeshc " << SSUMESHC_VERSION << std::endl;
	hfile << "// Date of conversion : " << date_buf << std::endl;
	hfile << "// Input file: " << std::string(input_file_name.begin(), input_file_name.end()) << std::endl;
	hfile << "// ======================================================" << std::endl;
	hfile << std::endl;
	hfile << "#pragma once" << std::endl;
	hfile << std::endl;
	hfile << "const unsigned int NUMGRP" << group_suffix 
		<< " = " << groups.size() << ";" << std::endl;
	if (mat) hfile << "const unsigned int NUMMAT" << material_suffix 
		<< " = " << materials.size() << ";" << std::endl;
	if (tex) hfile << "const unsigned int NUMTEX" << texture_suffix 
		<< " = " << textures.size() << ";" << std::endl;
	hfile << std::endl;
	for(unsigned int i = 0; i<groups.size(); i++) {
		
		std::string tmp;
		std::string::iterator iter = groups[i].name.begin();
		tmp = group_prefix;
		while(iter != groups[i].name.end())
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

		if(existing_symbols_groups.find(tmp) != existing_symbols_groups.end()) {
			std::string temp;
			unsigned long attempt = 0;
			do{
				temp = tmp + ToAlphaCode(attempt);
				attempt++;
			} while(existing_symbols_groups.find(temp) != existing_symbols_groups.end());
			tmp = temp;
		}
		existing_symbols_groups.insert(tmp);

		tmp.append(group_suffix);
		hfile << "const unsigned int " << tmp << "\t=\t" << groups[i].num << ";"; 
		hfile << "\t//" << groups[i].name;
		hfile << std::endl;
	}

	if (mat)
	{
		hfile << std::endl;
		for (unsigned int i = 0; i < materials.size(); i++)
		{
			std::string tmp = material_prefix;
			std::string::iterator iter = materials[i].name.begin();
			while (iter != materials[i].name.end())
			{
				if (isspace( *iter )) tmp += '_';
				else if (isalnum( *iter )) tmp += toupper( *iter );
				else if (*iter == '_') tmp += '_';

				iter++;
			}

			if (existing_symbols_materials.find( tmp ) != existing_symbols_materials.end())
			{
				std::string temp;
				unsigned long attempt = 0;
				do
				{
					temp = tmp + ToAlphaCode( attempt );
					attempt++;
				} while (existing_symbols_materials.find( temp ) != existing_symbols_materials.end() );
				tmp = temp;
			}
			existing_symbols_materials.insert(tmp);

			tmp.append( material_suffix );
			hfile << "const unsigned int " << tmp << "\t=\t" << materials[i].num << ";"; 
			hfile << "\t//" << materials[i].name;
			hfile << std::endl;
		}
	}

	if (tex)
	{
		hfile << std::endl;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			std::string tmp = texture_prefix;
			std::string tmp2 = textures[i].name;
			tmp2 = tmp2.substr( 0, tmp2.find_last_of( '.' ) );

			std::string::iterator iter = tmp2.begin();
			while (iter != tmp2.end())
			{
				if (isspace( *iter )) tmp += '_';
				else if (isalnum( *iter )) tmp += toupper( *iter );
				else if (*iter == '_') tmp += '_';
				else if (*iter == '/') tmp += '_';
				else if (*iter == '\\') tmp += '_';

				iter++;
			}

			if (existing_symbols_textures.find( tmp ) != existing_symbols_textures.end())
			{
				std::string temp;
				unsigned long attempt = 0;
				do
				{
					temp = tmp + ToAlphaCode( attempt );
					attempt++;
				} while (existing_symbols_textures.find( temp ) != existing_symbols_textures.end() );
				tmp = temp;
			}
			existing_symbols_textures.insert(tmp);

			tmp.append( texture_suffix );
			hfile << "const unsigned int " << tmp << "\t=\t" << textures[i].num << ";"; 
			hfile << "\t//" << textures[i].name;
			hfile << std::endl;
		}
	}

	//hfile << std::endl;
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
	std::wcout << "   -p PREFIX   Set group symbol prefix" << std::endl; 
	std::wcout << "   -s SUFFIX   Set group symbol suffix" << std::endl; 
	std::wcout << "   -m	      Enable material list output" << std::endl;
	std::wcout << "   -b PREFIX   Set material symbol prefix" << std::endl;
	std::wcout << "   -n SUFFIX   Set material symbol suffix" << std::endl;
	std::wcout << "   -t	      Enable texture list output" << std::endl;
	std::wcout << "   -e PREFIX   Set texture symbol prefix" << std::endl;
	std::wcout << "   -r SUFFIX   Set texture symbol suffix" << std::endl;
	std::wcout << "   -? -h       Display this help" << std::endl;

}

int _tmain(int argc, _TCHAR* argv[])
{
	bool bValidInput = false;
	bool doMAT = false;
	bool doTEX = false;
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
				group_prefix = std::string(temp.begin(), temp.end());
				break;
			case L's':
				temp.assign(argv[i]+2);
				group_suffix = std::string(temp.begin(), temp.end());
				break;
			case L'o':
				output_file_name.assign(argv[i]+2);
				break;
			case L'm':
				doMAT = true;
				break;
			case L'b':
				if (!doMAT) break;
				temp.assign( argv[i] + 2 );
				material_prefix = std::string( temp.begin(), temp.end() );
				break;
			case L'n':
				if (!doMAT) break;
				temp.assign( argv[i] + 2 );
				material_suffix = std::string( temp.begin(), temp.end() );
				break;
			case L't':
				doTEX = true;
				break;
			case L'e':
				if (!doTEX) break;
				temp.assign( argv[i] + 2 );
				texture_prefix = std::string( temp.begin(), temp.end() );
				break;
			case L'r':
				if (!doTEX) break;
				temp.assign( argv[i] + 2 );
				texture_suffix = std::string( temp.begin(), temp.end() );
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
		std::wcout << L"Group prefix: " << std::wstring(group_prefix.begin(), group_prefix.end()) << std::endl;
		std::wcout << L"Group suffix: " << std::wstring(group_suffix.begin(), group_suffix.end()) << std::endl;

		if (doMAT)
		{
			std::wcout << L"Material prefix: " << std::wstring(material_prefix.begin(), material_prefix.end()) << std::endl;
			std::wcout << L"Material suffix: " << std::wstring(material_suffix.begin(), material_suffix.end()) << std::endl;
		}

		if (doTEX)
		{
			std::wcout << L"Texture prefix: " << std::wstring(texture_prefix.begin(), texture_prefix.end()) << std::endl;
			std::wcout << L"Texture suffix: " << std::wstring(texture_suffix.begin(), texture_suffix.end()) << std::endl;
		}

		std::wcout << L"Parse mesh file..." << std::endl;
		ParseMesh( doMAT, doTEX );

		WriteHeaderFile( doMAT, doTEX );

		std::wcout << L"Found:" << std::endl;
		std::wcout << L"\t" << groups.size() << L" groups" << std::endl;
		if (doMAT) std::wcout << L"\t" << materials.size() << L" materials" << std::endl;
		if (doTEX) std::wcout << L"\t" << textures.size() << L" textures" << std::endl;

		std::wcout << std::endl << L"Done! Bye..." << std::endl;
		return 0;
	}
	else {
		std::wcerr << L"ERROR: No input file specified." << std::endl;
		printhelp();
		return 1;
	}
	
}

