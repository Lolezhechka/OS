// SENDER

#include <iostream>

#include "Pipes.hpp"
#include "..\Encrypter\Encrypter.h"

int main()
{
    std::string exit_word = "exit";
    encrypter::encrypt(exit_word);

    PipeStream pipea = PipeStream::CreateServerAndWaitConnection("hPipe_");
    std::string buffer;
    do {
        std::getline(std::cin, buffer);
        encrypter::encrypt(buffer);
        pipea.SendMsg(buffer);
    } while (buffer != exit_word);
    pipea.Close();
}