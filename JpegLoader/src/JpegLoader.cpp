#include <array>
#include <fstream>
#include <iostream>
#include <string>

#include "JpegDeserialize.hpp"
#include "JpegLoader.hpp"
#include "JpegTagField.hpp"
#include "JpegTagUtils.hpp"

namespace {

std::string EndianNumToString(uint8_t byte1, uint8_t byte2) {
  using namespace std::literals::string_literals;
  if (byte1 == static_cast<uint8_t>(0x49) && byte2 == static_cast<uint8_t>(0x49)) {
    return "Little-Endian"s;
  }
  if (byte1 == static_cast<uint8_t>(0x4d) && byte2 == static_cast<uint8_t>(0x4d)) {
    return "Big-Endian"s;
  }
  return ""s;
}

}

namespace jpeg {

Loader::Loader(const std::string& fileName)
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

  // �^�O�t�B�[���h��value�I�t�Z�b�g�̋N�_�ʒu
  ParseIFD(ExifBasePosItr() + 8);
}

void Loader::DumpRawData() const {
  for (const auto& elem : binaryData) {
    std::cout << elem;
  }
}

void Loader::DumpExifTagFields() const {
  if (binaryData.empty()) {
    return;
  }

  // �ǂݏo�����l���ꎞ�ۑ�����array
  std::array<uint8_t, 2> bytes;

  auto itr = ExifBasePosItr();

  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  std::cout << "byte order --> " << EndianNumToString(bytes[0], bytes[1]) << '\n'
            << std::endl;

  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }

  for (const auto& tagField : exifTagFields) {
    tagField.Print();
  }
}

void Loader::DumpExif() const {
  if (binaryData.empty()) {
    return;
  }

  std::cout << "<< Exif Information >>\n"
            << std::endl;

  for (const auto& tagField : exifTagFields) {
    tagField.PrintSimply();
  }
}

void Loader::ParseIFD(std::vector<uint8_t>::const_iterator itr) {
  // �ǂݏo�����l���ꎞ�ۑ�����array
  std::array<uint8_t, 4> bytes;

  // IFD�̃^�O����ǂ�
  bytes[0] = *itr;
  ++itr;
  bytes[1] = *itr;
  ++itr;
  uint32_t numOfTag = jpeg::Deserialize(bytes[0], bytes[1]);

  // �^�O�̐��Ԃ�^�O�t�B�[���h��ǂ�
  for (int32_t i = 0; i < numOfTag; ++i) {
    auto tagField = tag::Field(itr, ExifBasePosItr());

    exifTagFields.emplace_back(tagField);

    itr += 12; // �^�O�t�B�[���h��12�o�C�g�ŌŒ肾����A����Ŏ��̃^�O��ǂ߂�

    if (auto itrToNextIfd = tagField.NextIFDItr(); itrToNextIfd) {
      ParseIFD(itrToNextIfd.value());
    }
  }

  // ����IFD�ւ�offset��ǂ�
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  auto offset = jpeg::Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);

  // ����IFD��������
  if (offset == 0) {
    return;
  }

  // ����IFD��
  itr = ExifBasePosItr() + offset;
  ParseIFD(itr);
}

std::vector<uint8_t>::const_iterator Loader::ExifBasePosItr() const {
  auto itr = binaryData.cbegin();

  // �ŏ���ffd8���΂�
  itr += 2;

  // 1�̃Z�O�����g�̃T�C�Y(bytes)
  int32_t segmentSize;

  // Exif��񂪂���APP1�Z�O�����g���O�܂Ŕ�΂�
  while (true) {
    uint8_t ff = *itr;
    ++itr;

    uint8_t marker = *itr;
    ++itr;

    uint8_t segmentSizeByte1 = *itr;
    ++itr;

    uint8_t segmentSizeByte2 = *itr;
    ++itr;

    segmentSize = jpeg::Deserialize(segmentSizeByte1, segmentSizeByte2);

    if (marker == 0xe1) {
      break;
    }

    itr += segmentSize - 2;
  }

  // Exif���ʎq��6�Ԃ��΂�
  return itr + 6;
}

} // namespace jpeg