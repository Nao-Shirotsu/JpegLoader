#include <array>
#include <fstream>
#include <iostream>

#include "JpegDeserialize.hpp"
#include "JpegLoader.hpp"
#include "JpegTagUtils.hpp"
#include "JpegTagField.hpp"

namespace jpeg {

Loader::Loader(const std::string& fileName)
  : binaryData(){
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

void Loader::DumpRawData() const{
  for (const auto& elem : binaryData) {
    std::cout << elem;
  }
}

void Loader::DumpExif() const{
  if (binaryData.empty()) {
    return;
  }

  auto itr = ExifBasePosItr();
  itr -= 6;

  std::cout << "Exif ID code   | ";
  for (int32_t i = 0; i < 6; ++i) {
    std::cout << static_cast<int32_t>(*itr) << " ";
    ++itr;
  }
  std::cout << std::endl;

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
  ExifBasePosItr() = itr - 8;

  // �����IFD0�̐擪�܂Ŕ��
  OutputIFD(ExifBasePosItr() + skipOffset);
}

void Loader::Emplace(tag::Field&& field) {
  exifTagFields.emplace_back(field);
}

void Loader::ConstructTagFields() {
}

void Loader::OutputIFD(std::vector<uint8_t>::const_iterator itr) const{
  std::cout << "\n~~~~~~~~~~~~~~~~IFD~~~~~~~~~~~~~~~~" << std::endl;

  // �ǂݏo�����l���ꎞ�ۑ�����array
  std::array<uint8_t, 4> bytes;

  // IFD�̃^�O����ǂ�
  bytes[0] = *itr;
  ++itr;
  bytes[1] = *itr;
  ++itr;
  uint32_t numOfTag = jpeg::Deserialize(bytes[0], bytes[1]);
  std::cout << "Num of Tag: " << numOfTag << std::endl;

  // �^�O�̐��Ԃ�^�O�t�B�[���h��ǂ�
  for (int32_t i = 0; i < numOfTag; ++i) {
    auto tagField = tag::Field(itr, ExifBasePosItr());
    tagField.Print();
    itr += 12; // �^�O�t�B�[���h��12�o�C�g�ŌŒ肾����A����Ŏ��̃^�O��ǂ߂�

    if (auto itrToIfd = tagField.GetItrAtNextIFD(); itrToIfd) {
      OutputIFD(itrToIfd.value());
    }
  }

  // ����IFD�ւ�offset��ǂ�
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  auto offset = jpeg::Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);
  std::cout << "Offset : " << numOfTag << std::endl;

  // N-th IFD����M-th�ւ�offset��0�Ȃ�M-th IFD�͑��݂��Ȃ�
  if (offset == 0) {
    std::cout << "\n~~READING DONE, EXIT APP1 SEGMENT~~" << std::endl;
    return;
  }

  itr = ExifBasePosItr() + offset;

  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  OutputIFD(itr);
}

std::vector<uint8_t>::const_iterator Loader::ExifBasePosItr() const{
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