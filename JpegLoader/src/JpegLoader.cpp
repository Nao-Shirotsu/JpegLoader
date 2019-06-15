#include <array>
#include <fstream>
#include <iostream>

#include "Deserialize.hpp"
#include "JpegLoader.hpp"

JpegLoader::JpegLoader(const std::string& fileName)
  : binaryData() {
  std::ifstream ifs(fileName);
  if (!ifs) {
    std::cout << "*ERROR* image file loading failed" << std::endl;
    return;
  }

  // 一応先頭に飛ぶ
  ifs.seekg(0);

  // EOFまで1バイトずつ読みだしてメンバのbinaryDataにemplace
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
  if (binaryData.empty()) {
    return;
  }

  auto [itr, segmentSize] = GetItrAtExifId();

  std::array<uint8_t, 6> exifId;
  //std::cout << "Exif ID code : ";
  std::cout << std::hex;
  for (auto& elem : exifId) {
    elem = *itr;
    //std::cout << static_cast<int32_t>(elem) << ' ';
    ++itr;
  }
  //std::cout << std::endl;
  std::cout << std::dec;

  // ===== test output =====
  for( int i = 0; i < segmentSize; ++i ) {
    std::cout << *itr;
    ++itr;
  }
  // =======================
}

std::pair<std::vector<uint8_t>::iterator, int32_t> JpegLoader::GetItrAtExifId() {
  auto itr = binaryData.begin();
  std::cout << std::hex;

  // 最初のffd8を飛ばす
  // std::cout << static_cast<int32_t>(*itr);
  ++itr;

  // std::cout << static_cast<int32_t>(*itr) << " | START" << std::endl;
  ++itr;

  int32_t segmentSize;

  // Exif情報があるAPP1セグメント直前まで飛ばす
  while (true) {
    uint8_t ff = *itr;
    ++itr;

    uint8_t marker = *itr;
    ++itr;

    // std::cout << std::hex << static_cast<int32_t>(ff) << static_cast<int32_t>(marker) << std::dec << " | SIZE: ";

    uint8_t segmentSizeByte1 = *itr;
    ++itr;

    uint8_t segmentSizeByte2 = *itr;
    ++itr;

    segmentSize = Deserialize(segmentSizeByte1, segmentSizeByte2);
    // std::cout << segmentSize << std::endl;

    if (marker == 0xe1) {
      break;
    }

    itr += segmentSize - 2;
  }

  return std::make_pair(itr, segmentSize);
}
