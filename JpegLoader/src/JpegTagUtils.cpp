#include <iostream>

#include "JpegDeserialize.hpp"
#include "JpegTagUtils.hpp"

namespace jpeg::tag {

// TagFieldのTagValueの型を文字列に変換
std::string TypeToStr(const tag::Type type) {
  switch (type) {
  case Type::Byte:
    return "byte";

  case Type::Ascii:
    return "ascii";

  case Type::Undefined:
    return "undefined";

  case Type::Short:
    return "short";

  case Type::Long:
    return "long";

  case Type::SLong:
    return "signed long";

  case Type::Rational:
    return "rational";

  case Type::SRational:
    return "signed rational";
  }
  return "INVALID TYPE";
}

// TagFieldのTagValueの型の値1つのバイト数
uint32_t SizeOf(const Type type) {
  switch (type) {
  case Type::Byte:
  case Type::Ascii:
  case Type::Undefined:
    return 1;

  case Type::Short:
    return 2;

  case Type::Long:
  case Type::SLong:
    return 4;

  case Type::Rational:
  case Type::SRational:
    return 8;
  }
  return 0;
}

// TagFieldのTagValueの取り得る型とその値の数からバイト数を計算
uint32_t ByteLengthOf(const Type type, const int32_t count) {
  return count * tag::SizeOf(type);
}

void OutputValue(std::vector<uint8_t>::const_iterator itr, const int32_t sizeOfValueType, const int32_t count) {
  switch (sizeOfValueType) {
  case 1:
    for (int32_t i = 0; i < count; ++i, ++itr) {
      std::cout << *itr << ' ';
    }
    break;

  case 2:
    for (int32_t i = 0; i < count; ++i, ++itr) {
      std::cout << jpeg::Deserialize(*itr, *(++itr)) << ' ';
    }
    break;

  case 4:
    for (int32_t i = 0; i < count; ++i, ++itr) {
      std::cout << jpeg::Deserialize(*itr, *(++itr), *(++itr), *(++itr)) << ' ';
    }
    break;

  case 8:
    for (int32_t i = 0; i < count; ++i, ++itr) {
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