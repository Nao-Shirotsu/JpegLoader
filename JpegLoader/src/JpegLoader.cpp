#include <array>
#include <fstream>
#include <iostream>

#include "JpegLoader.hpp"
#include "Deserialize.hpp"

JpegLoader::JpegLoader(const std::string& fileName)
  : binaryData() {
  std::ifstream ifs(fileName);
  if (!ifs) {
    std::cout << "*ERROR* image file loading failed" << std::endl;
    return;
  }

  // �ꉞ�擪�ɔ��
  ifs.seekg(0);

  // EOF�܂�1�o�C�g���ǂ݂����ă����o��binaryData��emplace
  while (!ifs.eof()) {
    binaryData.emplace_back(ifs.get());
  }
}

void JpegLoader::DumpRawData() {
  for (const auto& elem : binaryData) {
    std::cout << elem;
  }
}

void JpegLoader::DumpExif() {
  if( binaryData.empty() ) {
    return;
  }
  auto itr = binaryData.begin();
  std::cout << std::hex;

  // �ŏ���ffd8���΂�
  std::cout << static_cast<int32_t>(*itr);
  ++itr;

  std::cout << static_cast<int32_t>(*itr) << " | START" << std::endl;
  ++itr;

  int32_t segmentSize;
  
  // Exif��񂪂���APP1�Z�O�����g���O�܂Ŕ�΂�
  while (true) {
    uint8_t ff = *itr;
    ++itr;

    uint8_t marker = *itr;
    ++itr;

    std::cout << std::hex << static_cast<int32_t>(ff) << static_cast<int32_t>(marker) << std::dec << " | SIZE: ";

    uint8_t segmentSizeByte1 = *itr;
    ++itr;

    uint8_t segmentSizeByte2 = *itr;
    ++itr;

    segmentSize = Deserialize(segmentSizeByte1, segmentSizeByte2);
    std::cout << segmentSize << std::endl;
    
    if( marker == 0xe1 ) {
      break;
    }

    itr += segmentSize - 2;
  }

  // 
  std::array<uint8_t, 6> exifId;
  std::cout << std::hex;
  for (auto& elem : exifId) {
    elem = *itr;
    std::cout << static_cast<int32_t>(elem) << ' ';
    ++itr;
  }
  std::cout << std::endl;
  std::cout << std::dec;
}

//void JpegLoader::DumpInRange(int begin, int end) {
//  std::cout << std::hex;
//  for (int i = begin; i <= end; ++i) {
//    std::cout << static_cast<int>(binaryData[i]) << ' ';
//  }
//  std::cout << "\n================" << std::endl;
//  std::cout << std::dec;
//}

//bool JpegLoader::IsJpegFile() {
//  return false;
//}