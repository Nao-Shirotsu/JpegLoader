#include <array>
#include <fstream>
#include <iostream>
#include <optional>

#include "Deserialize.hpp"
#include "JpegLoader.hpp"

// TagField��TagValue�̎�蓾��^�Ƃ��̒l�̐�����o�C�g�����v�Z����util�֐�
std::optional<int32_t> CalcTagValueLength(const int32_t type, const int32_t count) {
  switch (type) { // ����switch�D������Ȃ��Ȃ�
  case 1: // BYTE
  case 2: // ASCII
  case 7: // UNDEFINED
    return count;

  case 3: // SHORT
    return 2 * count;

  case 4: // LONG
  case 9: // SLONG
    return 4 * count;

  case 5:  // RATIONAL
  case 10: // SRATIONAL
    return 8 * count;
  }

  return std::nullopt; // �s���l
}

JpegLoader::JpegLoader(const std::string& fileName)
  : binaryData()
  , basePosItr(){
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
  if (binaryData.empty()) {
    return;
  }

  auto [itr, segmentSize] = GetItrAtExifId();

  std::cout << std::hex;

  std::cout << "Exif ID code   | ";
  for (int i = 0; i < 6; ++i) {
    std::cout << static_cast<int32_t>(*itr) << " ";
    ++itr;
  }
  std::cout << std::endl;

  // ===== test output =====
  // for( int i = 0; i < segmentSize; ++i ) {
  //  std::cout << *itr;
  //  ++itr;
  // }
  // =======================

  std::cout << "byte order     | ";
  std::cout << static_cast<int32_t>(*itr) << " ";
  ++itr;
  std::cout << static_cast<int32_t>(*itr) << std::endl;
  ++itr;

  std::cout << "tiff version   | ";
  std::cout << static_cast<int32_t>(*itr) << " ";
  ++itr;
  std::cout << static_cast<int32_t>(*itr) << std::endl;
  ++itr;

  std::array<uint8_t, 4> readBytes;
  for (auto& elem : readBytes) {
    elem = *itr;
    ++itr;
  }
  uint32_t skipOffset = Deserialize(readBytes[0], readBytes[1], readBytes[2], readBytes[3] );
  std::cout << "offset to IFD0 | " << skipOffset << std::endl;

  // �^�O�t�B�[���h��value�I�t�Z�b�g�̋N�_�ʒu
  basePosItr = itr - 8;

  // �����IFD0�̐擪�܂Ŕ��
  itr += (skipOffset - 8);
  OutputIFD(itr);
}

void JpegLoader::OutputIFD(std::vector<uint8_t>::iterator itr) {
  std::array<uint8_t, 4> bytes;

  // IFD�̃^�O����ǂ�
  bytes[0] = *itr;
  ++itr;
  bytes[1] = *itr;
  ++itr;
  uint32_t numOfTag = Deserialize(bytes[0], bytes[1]);

  // �^�O�̐��Ԃ�^�O�t�B�[���h��ǂ�
  for (int i = 0; i < numOfTag; ++i) {
    OutputTagField(itr);
  }

  // ����IFD�ւ�offset��ǂ�
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  auto offset = Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);

  // N-th IFD����M-th�ւ�offset��0�Ȃ�M-th IFD�͑��݂��Ȃ�
  if(offset == 0) {
    return;
  }

  itr = basePosItr + offset;
}

void JpegLoader::OutputTagField(std::vector<uint8_t>::iterator itr) {
  std::array<uint8_t, 4> bytes;

  // �^�O�ԍ��ƃ^�O�^�C�v��ǂ�
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  auto tagNumber = Deserialize(bytes[0], bytes[1]);
  auto tagType = Deserialize(bytes[2], bytes[3]);
  std::cout << std::dec;
  std::cout << "\n=======Tag Field=======" << std::endl;
  std::cout << "Number : " << tagNumber << std::endl;
  std::cout << "Type   : " << tagType << std::endl;

  // �^�O�Ɋ܂܂��l�̐���ǂ�
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  auto tagCount = Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);
  auto valueByteLength = CalcTagValueLength(tagType, tagCount);
  std::cout << "Count  : " << tagCount << std::endl;

  // �^�O��value�ւ�offset��ǂ�
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  
  // offset��4bytes�ȉ��Ȃ�W�����v�͂��Ȃ�
  if (valueByteLength > 4) {
    // value�̒����Ԃ�ǂ�Ŗ߂��Ă��� = itr�����̃^�O�̖����̈���ɂ���
  }
  std::cout << "\n=======================" << std::endl;
}


std::pair<std::vector<uint8_t>::iterator, int32_t> JpegLoader::GetItrAtExifId() {
  auto itr = binaryData.begin();
  std::cout << std::hex;

  // �ŏ���ffd8���΂�
  // std::cout << static_cast<int32_t>(*itr);
  ++itr;

  // std::cout << static_cast<int32_t>(*itr) << " | START" << std::endl;
  ++itr;

  int32_t segmentSize;

  // Exif��񂪂���APP1�Z�O�����g���O�܂Ŕ�΂�
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