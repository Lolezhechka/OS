#include <string>
#ifdef ENCRYPTER_EXPORTS
#define ENCRYPTER_API __declspec(dllexport)
#else
#define ENCRYPTER_API __declspec(dllimport)
#endif
namespace encrypter {
	void encrypt(std::string& msg);
	void decrypt(std::string& msg);
}