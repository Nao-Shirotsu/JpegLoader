#include <array>
#include <fstream>
#include <iostream>
#include <optional>

#include "JpegDeserialize.hpp"
#include "JpegLoader.hpp"

namespace {

// TagField��TagValue�̌^�𕶎���ɕϊ�
const char* ValueTypeToString(const int32_t type) {
  switch (type) {
  case 1:
    return "byte";

  case 2:
    return "ascii";

  case 7:
    return "undefined";

  case 3:
    return "short";

  case 4:
    return "long";

  case 9:
    return "signed long";

  case 5:
    return "rational";

  case 10:
    return "signed rational";
  }
  return "INVALID TYPE";
}

// TagField��TagValue�̌^�̒l1�̃o�C�g��
std::optional<uint16_t> SizeOfValueType(const int32_t type) {
  switch (type) {
  case 1: // BYTE
  case 2: // ASCII
  case 7: // UNDEFINED
    return 1;

  case 3: // SHORT
    return 2;

  case 4: // LONG
  case 9: // SLONG
    return 4;

  case 5:  // RATIONAL
  case 10: // SRATIONAL
    return 8;
  }
  return std::nullopt;
}

// TagField��TagValue�̎�蓾��^�Ƃ��̒l�̐�����o�C�g�����v�Z
std::optional<int32_t> TagValueLength(const int32_t type, const int32_t count) {
  auto singleValueSize = SizeOfValueType(type);
  if (!singleValueSize) {
    return count * singleValueSize.value();
  }
  return std::nullopt;
}

}// anonymous namespace

namespace jpeg{

Loader::Loader(const std::string& fileName)
  : binaryData()
  , basePosItr() {
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

void Loader::DumpRawData() {
  for (const auto& elem : binaryData) {
    std::cout << elem;
  }
}

void Loader::DumpExif() {
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
  uint32_t skipOffset = jpeg::Deserialize(readBytes[0], readBytes[1], readBytes[2], readBytes[3]);
  std::cout << "offset to IFD0 | " << skipOffset << std::endl;

  // �^�O�t�B�[���h��value�I�t�Z�b�g�̋N�_�ʒu
  basePosItr = itr - 8;

  // �����IFD0�̐擪�܂Ŕ��
  itr = basePosItr + skipOffset;
  OutputIFD(itr);
}

void Loader::OutputIFD(std::vector<uint8_t>::iterator itr) {
  std::cout << "\n~~~~~~~~~~~~~~~~IFD~~~~~~~~~~~~~~~~" << std::endl;

  // ===== test output =====
  // std::cout << "------\nbeginning of OutputIFD" << std::endl;
  // =======================

  // ===== test output =====
  // std::cout << "itr pos : " << (itr - binaryData.begin()) << std::endl;
  // =======================

  std::array<uint8_t, 4> bytes;

  // IFD�̃^�O����ǂ�
  bytes[0] = *itr;
  ++itr;
  bytes[1] = *itr;
  ++itr;
  uint32_t numOfTag = jpeg::Deserialize(bytes[0], bytes[1]);
  std::cout << "Num of Tag: " << numOfTag << std::endl;

  // ===== test output =====
  // std::cout << "calculated numOfTag" << std::endl;
  // =======================

  // �^�O�̐��Ԃ�^�O�t�B�[���h��ǂ�
  for (int i = 0; i < numOfTag; ++i) {
    OutputTagField(itr);
    itr += 12; // �^�O�t�B�[���h��12�o�C�g�ŌŒ肾����A����Ŏ��̃^�O��ǂ߂�
  }

  // ===== test output =====
  // std::cout << "read all tags" << std::endl;
  // =======================

  // ����IFD�ւ�offset��ǂ�
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  auto offset = jpeg::Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);
  std::cout << "Offset : " << numOfTag << std::endl;

  // ===== test output =====
  // std::cout << "read offset to next IFD" << std::endl;
  // =======================

  // N-th IFD����M-th�ւ�offset��0�Ȃ�M-th IFD�͑��݂��Ȃ�
  if (offset == 0) {
    std::cout << "\n~~READING DONE, EXIT APP1 SEGMENT~~"<< std::endl;
    return;
  }

  itr = basePosItr + offset;

  // ===== test output =====
  // std::cout << "itr addition\n------" << std::endl;
  // =======================

  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  OutputIFD(itr);
}

void Loader::OutputTagField(std::vector<uint8_t>::iterator itr) {
  std::array<uint8_t, 4> bytes;

  // �^�O�ԍ��ƃ^�O�^�C�v��ǂ�
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  auto tagNumber = jpeg::Deserialize(bytes[0], bytes[1]);
  auto tagType = jpeg::Deserialize(bytes[2], bytes[3]);
  std::cout << std::hex;
  std::cout << "\n=======Tag Field=======" << std::endl;
  std::cout << "Number   : " << static_cast<int32_t>(bytes[0]) << " " << static_cast<int32_t>(bytes[1]) << std::endl;
  std::cout << "Type     : " << ValueTypeToString(Deserialize(bytes[2], bytes[3])) << std::endl;

  // �^�O�Ɋ܂܂��l�̐���ǂ�
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  auto tagCount = jpeg::Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);
  auto valueByteLength = TagValueLength(tagType, tagCount);
  std::cout << "Count    : " << tagCount << std::endl;

  // �^�O��value�ւ�offset��ǂ�
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }

  // offset��4bytes�ȉ��Ȃ�W�����v�͂��Ȃ�
  if (valueByteLength > 4) {
    // value�̒����Ԃ�ǂ�Ŗ߂��Ă��� = itr�����̃^�O�̖����̈���ɂ���
  }
  else {
    std::cout << "Val/offs : ";

    switch(SizeOfValueType( tagType ).value()) {
    case 1:
      std::cout << Deserialize(bytes[0]) << std::endl;
      break;

    case 2:
      std::cout << Deserialize(bytes[0], bytes[1]) << std::endl;
      break;

    case 4:
      std::cout << Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]) << std::endl;
      break;
    }
  }
  std::cout << "\nEND AT [" << (itr - binaryData.begin()) << "] bytes" << std::endl;
  std::cout << "=======================\n" << std::endl;
}


std::pair<std::vector<uint8_t>::iterator, int32_t> Loader::GetItrAtExifId() {
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

    segmentSize = jpeg::Deserialize(segmentSizeByte1, segmentSizeByte2);
    // std::cout << segmentSize << std::endl;

    if (marker == 0xe1) {
      break;
    }

    itr += segmentSize - 2;
  }

  return std::make_pair(itr, segmentSize);
}

}// namespace jpeg