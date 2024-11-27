// Encrypter.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "Encrypter.h"

namespace encrypter {
	void encrypt(std::string& msg) {
		for (auto&& i : msg)
			i ^= 19;
	}
	void decrypt(std::string& msg) {
		encrypt(msg);
	}
}