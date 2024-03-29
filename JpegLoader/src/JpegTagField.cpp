#include <array>
#include <iostream>

#include "JpegDeserialize.hpp"
#include "JpegTagField.hpp"
#include "JpegTagUtils.hpp"

namespace {
constexpr uint32_t ID_EXIF_IFD = jpeg::Deserialize(static_cast<uint8_t>(0x87), static_cast<uint8_t>(0x69));
constexpr uint32_t ID_GPS_IFD = jpeg::Deserialize(static_cast<uint8_t>(0x88), static_cast<uint8_t>(0x25));
constexpr uint32_t ID_INTERPOLATION_IFD = jpeg::Deserialize(static_cast<uint8_t>(0xa0), static_cast<uint8_t>(0x05));
}

namespace jpeg::tag {

Field::Field(std::vector<uint8_t>::const_iterator itr, std::vector<uint8_t>::const_iterator basePosItr_)
  : basePosItr(basePosItr_) {
  std::array<uint8_t, 4> bytes;

  // タグ番号とタグタイプを読む
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  id[0] = bytes[0];
  id[1] = bytes[1];

  type = tag::Type(jpeg::Deserialize(bytes[2], bytes[3]));

  // valueの値の数(count)を読む
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  length = jpeg::Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);

  byteVec = std::vector<uint8_t>(ByteLengthOf(type, length));

  // タグのvalueへのoffsetを読む
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }

  tailPos = itr - basePosItr;

  // valueを読んでbyteVecに格納
  if (auto byteLength = byteVec.size(); byteLength > 4) {
    uint32_t skipOffset = Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);
    itr = basePosItr + skipOffset;

    for (int32_t i = 0; i < byteLength; ++i) {
      byteVec[i] = *itr;
      ++itr;
    }
  }
  else {
    for (int32_t i = 0; i < byteLength; ++i) {
      byteVec[i] = bytes[i];
      ++itr;
    }
  }

  if (NextIFDItr ()) {
    
  }
}

std::optional<std::vector<uint8_t>::const_iterator> Field::NextIFDItr() const{
  uint32_t tagId = Deserialize(id[0], id[1]);
  if (tagId != ID_EXIF_IFD && tagId != ID_GPS_IFD && tagId != ID_INTERPOLATION_IFD) {
    return std::nullopt;
  }

  uint32_t offset = jpeg::Deserialize(byteVec[0], byteVec[1], byteVec[2], byteVec[3]);
  return basePosItr + offset;
}

void Field::Print() const{
  std::cout << "=======Tag Field=======" << std::endl;
  std::cout << std::hex;
  std::cout << "ID     : 0x[" << static_cast<uint32_t>(id[0]) << ' ' << static_cast<uint32_t>(id[1]) << "] aka " << IDToString(id[0], id[1]) << std::endl;
  std::cout << std::dec;
  std::cout << "Type   : " << tag::TypeToStr(type) << std::endl;
  std::cout << "Length : " << length << std::endl;
  std::cout << "Value  : ";
  PrintImpl();
  //std::cout << "\nEND AT [" << tailPos << "] bytes from basePos" << std::endl;
  std::cout << "=======================\n" << std::endl;
}

void Field::PrintSimply() const {
  std::cout << '[' << IDToString(id[0], id[1]) << "]\n";
  PrintImpl();
  std::cout << std::endl;
}

void Field::PrintImpl() const{
  switch (type) {
  case Type::Ascii:
    for (const auto elem : byteVec) {
      std::cout << elem;
    }
    break;

  case Type::Short:
    for (auto printItr = byteVec.cbegin(); printItr != byteVec.cend(); printItr += 2) {
      std::cout << Deserialize(*printItr, *(printItr + 1)) << ' ';
    }
    break;

  case Type::Long:
    for (auto printItr = byteVec.cbegin(); printItr != byteVec.cend(); printItr += 4) {
      std::cout << Deserialize(*printItr, *(printItr + 1), *(printItr + 2), *(printItr + 3)) << ' ';
    }
    break;

  case Type::SLong:
    for (auto printItr = byteVec.cbegin(); printItr != byteVec.cend(); printItr += 4) {
      char sign = (*printItr & 0b1000) ? '-' : '+';
      uint8_t headByte = *printItr & 0x0111;
      std::cout << sign << Deserialize(headByte, *(printItr + 1), *(printItr + 2), *(printItr + 3)) << ' ';
    }
    break;

  case Type::Rational:
    for (auto printItr = byteVec.cbegin(); printItr != byteVec.cend(); printItr += 8) {
      std::cout << Deserialize(*printItr, *(printItr + 1), *(printItr + 2), *(printItr + 3)) << '/';
      std::cout << Deserialize(*(printItr + 4), *(printItr + 5), *(printItr + 6), *(printItr + 7)) << ' ';
    }
    break;

  case Type::SRational:
    for (auto printItr = byteVec.cbegin(); printItr != byteVec.cend(); printItr += 8) {
      bool numerIsMinus = *printItr & 0b1000;
      bool denomIsMinus = *(printItr + 4) & 0b1000;
      char sign = (numerIsMinus ^ denomIsMinus) ? '-' : '+';
      uint8_t headByteNumer = *printItr & 0b0111;
      uint8_t headByteDenom = *(printItr + 4) & 0b0111;
      std::cout << sign << Deserialize(headByteNumer, *(printItr + 1), *(printItr + 2), *(printItr + 3)) << '/';
      std::cout << Deserialize(headByteDenom, *(printItr + 5), *(printItr + 6), *(printItr + 7)) << ' ';
    }
    break;

  default:
    std::cout << std::hex;
    for (const auto elem : byteVec) {
      std::cout << static_cast<int32_t>(elem) << ' ';
    }
    std::cout << std::dec;
  }
  std::cout << std::endl;
}

}