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
#include <algorithm>
#include <sstream>

// Function which prints the usage of this executable
void usage(std::string name)
{
  size_t beg = name.find_last_of("/\\");
  if (beg != std::string::npos) name = name.substr(beg + 1, std::string::npos);
  // Follows docopt.org format
  std::cerr << "Usage:\n"
            << "  " << name << " -i <file-in> <file-out>\n"
            << "  " << name << " --version\n";
  // note: this interface is likely to change in future kim-api releases
}

void usage(std::string name, std::string message)
{
	std::cerr << message << std::endl;
	usage(name);
}
namespace B64ENCODE 
{
void WriteEncodedFile(std::ifstream & instream, std::ofstream & outstream, const std::string & encodeFormatFileName)
{
	std::string header = "extern unsigned char " + encodeFormatFileName + "[] = \n\"";
	outstream.write(header.data(),header.length());
	
	base64::encoder E;

	E.encode(instream, outstream);
	outstream.seekp(-1, std::ios::cur); // rewind by 1 character to delete the '\n' written by libb64
	std::string footer = "\";\nextern unsigned int " + encodeFormatFileName
                       + "_len = sizeof(" + encodeFormatFileName + ");\n";

	outstream.write(footer.data(),footer.length());
}
}

int main(int argc, char** argv)
{
	if ((argc < 2) || (argc > 4))
	{
		usage(argv[0]);
		return 1;
	}

	std::string option = argv[1];
	if(option == "--version")
	{
		printf("VERSION\n");
		return 0;
	}
	else if(option != "-i" || argc != 4)
	{
		usage(argv[0]);
		exit(-1);
	}
	
	// open the input file in binary-mode
	std::string input = argv[2];
	std::ifstream instream(input.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!instream.is_open())
	{
		usage(argv[0], "Could not open input file " + input);
		exit(-1);
	}
	
	// open the output file in binary-mode
	std::string output = argv[3];
	std::ofstream outstream(output.c_str(), std::ios_base::out);
	if (!outstream.is_open())
	{
		usage(argv[0], "Could not open output file " + output);
		exit(-1);
	}
	
	std::string encodeFormatFileName = input;
	std::replace(encodeFormatFileName.begin(),encodeFormatFileName.end(), '.','_');
	std::replace(encodeFormatFileName.begin(),encodeFormatFileName.end(), '-','_');

	B64ENCODE::WriteEncodedFile(instream,outstream,encodeFormatFileName);
	
	return 0;
}

