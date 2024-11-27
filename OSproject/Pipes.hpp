#include "Windows.h"

#include <iostream>
#include <string>

struct PipeStream {
private:
  HANDLE hPipe_;

  PipeStream(HANDLE hPipe) : hPipe_(hPipe) {}

  static std::string CombinePipeName(std::string name) {
    return "\\\\.\\pipe\\" + name;
  }
public:
  std::string WaitMsg() {
    if (WaitForSingleObject(hPipe_, INFINITE) != WAIT_OBJECT_0) {
      std::cerr << "Wait failed[" << GetLastError() << "]" << std::endl;
      throw new std::exception{"Wait failed."};
    }

    DWORD length;
    if (PeekNamedPipe(hPipe_, NULL, 0, NULL, &length, NULL) == FALSE) {
      std::cerr << "Peek failed[" << GetLastError() << "]" << std::endl;
      throw new std::exception{"Peek failed."};
    }
    std::cerr << "Length: " << length << "\n";

    std::string buffer(length, '\0');
    if (ReadFile(hPipe_, &buffer[0], length, NULL, NULL) == FALSE) {
      std::cerr << "Read failed[" << GetLastError() << "]" << std::endl;
      throw new std::exception{"Read failed."};
    }

    std::cerr << "Read complited!\n";
    return buffer;
  }
  void SendMsg(std::string msg) {
    if (WriteFile(hPipe_, msg.data(), msg.size(), NULL, NULL) == FALSE) {
      std::cerr << "Write failed[" << GetLastError() << "]" << std::endl;
      throw new std::exception{"Write failed."};
    }
  }

  
  static PipeStream CreateServerAndWaitConnection(std::string name) {
    name = CombinePipeName(name);
    std::cerr << "Creating pipe with name: " << name << "\n";

    HANDLE hPipe = CreateNamedPipeA(
      name.c_str(),
      PIPE_ACCESS_DUPLEX,
      PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS,
      PIPE_UNLIMITED_INSTANCES,
      4096,
      4096,
      0,
      NULL);
    if (hPipe == NULL) {
      std::cerr << "Failed creating pipe." << std::endl;
      throw new std::exception{"Failed creating pipe."};
    }
    std::cerr << "Pipe created [" << hPipe << "]\n";

    if (!ConnectNamedPipe(hPipe, NULL)) {
      std::cerr << "Failed wait for connection." << std::endl;
      throw new std::exception{"Failed wait for connection."};
    }
    std::cerr << "Connected succesfuly.\n";

    return PipeStream{hPipe};
  }
  static PipeStream CreateClient(std::string name) {
    name = CombinePipeName(name);
    std::cerr << "Connecting pipe with name: " << name << "\n";

    HANDLE hPipe = CreateFileA(
      name.c_str(),
      GENERIC_READ | GENERIC_WRITE,
      0,
      NULL,
      OPEN_EXISTING,
      0,
      NULL);
    std::cerr << "Connected succesfuly.\n";

    return PipeStream{hPipe};
  }
  void Close() {
      if (CloseHandle(hPipe_) == FALSE) {
          std::cerr << "Failed closing pipe." << std::endl;
          throw new std::exception{ "Failed closing pipe." };
      }
  }
};