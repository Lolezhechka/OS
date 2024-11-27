// Receiver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "..\OSproject\Pipes.hpp"
#include "..\Encrypter\Encrypter.h"
int main()
{
	PipeStream pipea = PipeStream::CreateClient("hPipe_");
	std::string buffer;
	do {	
		buffer = pipea.WaitMsg();
		std::cout << buffer << std::endl;
		encrypter::decrypt(buffer);
		std::cout << buffer << std::endl;
	} while (buffer != "exit");
	pipea.Close();
}

