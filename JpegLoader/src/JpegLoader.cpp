#include <fstream>
#include <iostream>
#include <array>

#include "JpegLoader.hpp"

JpegLoader::JpegLoader(const std::string& fileName) {
  std::ifstream ifs(fileName);
  if( !ifs ) {
    std::cout << "*ERROR* File loading failed" << std::endl;
    return;
  }
  
  // 一応先頭に飛ぶ
  ifs.seekg(0);

  // EOFまで1バイトずつ読みだしてメンバのbinaryDataにemplace
  while( !ifs.eof() ) {
    binaryData.emplace_back( ifs.get() );
  }
}

void JpegLoader::DumpRawData() {
  for( const auto& elem : binaryData ) {
    std::cout << elem;
  }
}

void JpegLoader::DumpExif() {
  auto itr = binaryData.begin();
  std::cout << std::hex << *itr;
  ++itr;
  std::cout << *itr << " | START" << std::endl;
  ++itr;

  while (true) {
    int ff = *itr;
    ++itr;

    int marker = *itr;
    ++itr;

    if (marker == 0xda) {
      break;
    }

    std::cout << std::hex << ff << marker << std::dec << " | SIZE: ";

    unsigned char segmentSizeByte1 = *itr;
    ++itr;

    unsigned char segmentSizeByte2 = *itr;
    ++itr;

    int segmentSize = Deserialize2bytes(segmentSizeByte1, segmentSizeByte2);
    std::cout << segmentSize << std::endl;

    /*if (marker == 0xda) {
      std::cout << "------------\n";
      for (int i = 0; i < 30; ++i) {
        std::cout << static_cast<int>(*(itr + i)) << " ";
      }
      std::cout << "\n------------" << std::endl;
    }*/

    if (marker == 0xe1) {
      std::array<unsigned char, 6> exifId;
      for (auto& elem : exifId) {
        elem = *itr;
        ++itr;
      }


    }

    itr = itr - 2;
    itr += segmentSize;
    //++itr;
    //DumpInRange(itr-binaryData.begin(), *itr);
  }

  
}

void JpegLoader::DumpInRange(int begin, int end) {
  std::cout << std::hex;
  for (int i = begin; i <= end; ++i) {
    std::cout << static_cast<int>(binaryData[i]) << ' ';
  }
  std::cout << "\n================" << std::endl;
  std::cout << std::dec;
}

bool JpegLoader::IsJpegFile() {
  return false;
}

int Deserialize2bytes(unsigned char first, unsigned char second) {
  return (static_cast<int>(first) << 8) + static_cast<int>(second);
}