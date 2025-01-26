/*
base64.cc - c++ source to a base64 reference encoder and decoder

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/

#include <b64/encode.h>

#include <iostream>
#include <fstream>
#include <string>

#include <stdlib.h>
#include <sstream>

// Function which prints the usage of this executable
void usage(std::string name)
{
  size_t beg = name.find_last_of("/\\");
  if (beg != std::string::npos) name = name.substr(beg + 1, std::string::npos);
  // Follows docopt.org format
  std::cerr << "Usage:\n"
            << "  " << name << " -i <file-in> <file-out>\n"
            << "  " << name << "--version\n";
  // note: this interface is likely to change in future kim-api releases
}

// quick replacement of to_string for C++98
inline std::string int_to_string(int value)
{
  std::ostringstream oss;
  oss << value;
  return oss.str();
}

int main(int argc, char** argv)
{
	if ((argc < 2) || (argc > 4))
	{
		usage(argv[0]);
		return 1;
	}
	
	// open the input file in binary-mode
	std::string input = argv[2];
	std::ifstream instream(input.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!instream.is_open())
	{
		usage("Could not open input file!");
		exit(-1);
	}
	
	// open the output file in binary-mode
	std::string output = argv[3];
	std::ofstream outstream(output.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!outstream.is_open())
	{
		usage("Could not open output file!");
		exit(-1);
	}

	// determine whether we need to encode or decode:
	std::string choice = argv[1];
	if (choice == "-e")
	{
		base64::encoder E;
		E.encode(instream, outstream);
	}
	else
	{
		usage("Invalid option " + choice);
	}

	return 0;
}

