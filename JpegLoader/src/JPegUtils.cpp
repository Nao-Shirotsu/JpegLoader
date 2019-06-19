#include <iostream>

#include "JpegUtils.hpp"
#include "JpegDeserialize.hpp"

namespace jpeg::tag {

// TagFieldのTagValueの型を文字列に変換
std::optional<const char*> TypeToStr(const int32_t type) {
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
  return std::nullopt;
}

// TagFieldのTagValueの型の値1つのバイト数
std::optional<uint16_t> SizeOf(const int32_t type) {
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

// TagFieldのTagValueの取り得る型とその値の数からバイト数を計算
std::optional<int32_t> ByteLengthOf(const int32_t type, const int32_t count) {
  auto singleValueSize = tag::SizeOf(type);
  if (singleValueSize) {
    return count * singleValueSize.value();
  }
  return std::nullopt;
}

void OutputValue(std::vector<uint8_t>::const_iterator itr, const int32_t sizeOfValueType, const int32_t count) {
  switch (sizeOfValueType) {
  case 1:
    for (int i = 0; i < count; ++i, ++itr) {
      std::cout << *itr << ' ';
    }
    break;

  case 2:
    for (int i = 0; i < count; ++i, ++itr) {
      std::cout << jpeg::Deserialize(*itr, *(++itr)) << ' ';
    }
    break;

  case 4:
    for (int i = 0; i < count; ++i, ++itr) {
      std::cout << jpeg::Deserialize(*itr, *(++itr), *(++itr), *(++itr)) << ' ';
    }
    break;

  case 8:
    for (int i = 0; i < count; ++i, ++itr) {
      std::cout << jpeg::Deserialize(*itr, *(++itr), *(++itr), *(++itr)) << '/'
                << jpeg::Deserialize(*itr, *(++itr), *(++itr), *(++itr)) << ' ';
    }
    break;

  default:
    std::cout << "TAG VALUE READING FAILED";
  }

  std::cout << std::endl;
}

} // namespace jpeg