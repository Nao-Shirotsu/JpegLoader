#include "JpegLoader.hpp"

int main(int argc, char* argv[]){
  if (argc == 3 && std::string(argv[1]) == std::string("--tag")) {
    jpeg::Loader jpegFile(argv[2]);
    jpegFile.DumpExifTagFields();
    return 0;
  }
  if (argc == 3 && std::string(argv[2]) == std::string("--tag")) {
    jpeg::Loader jpegFile(argv[1]);
    jpegFile.DumpExifTagFields();
    return 0;
  }

  jpeg::Loader jpegFile(argv[1]);
  jpegFile.DumpExif();
}