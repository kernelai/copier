#include <string>

class FileOperation {
  std::string path;
  int fd;

 public:
  FileOperation(std::string path);
  ~FileOperation();
};