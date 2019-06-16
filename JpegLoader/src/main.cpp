#include "JpegLoader.hpp"

int main(int argc, char* argv[]){
  jpeg::Loader jpegFile(argv[1]);
  //jpegFile.DumpRawData();
  jpegFile.DumpExif();
}