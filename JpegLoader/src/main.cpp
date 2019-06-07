#include "JpegLoader.hpp"

int main(int argc, char* argv[]){
  JpegLoader jpegFile(argv[1]);
  //jpegFile.DumpRawData();
  jpegFile.DumpExif();
}