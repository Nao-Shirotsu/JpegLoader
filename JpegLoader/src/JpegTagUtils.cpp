#include <iostream>

#include "JpegDeserialize.hpp"
#include "JpegTagUtils.hpp"

namespace jpeg::tag {

// TagField��TagValue�̌^�𕶎���ɕϊ�
std::string TypeToStr(const tag::Type type) {
  using namespace std::literals::string_literals;
  switch (type) {
  case Type::Byte:
    return "byte"s;

  case Type::Ascii:
    return "ascii"s;

  case Type::Undefined:
    return "undefined"s;

  case Type::Short:
    return "short"s;

  case Type::Long:
    return "long"s;

  case Type::SLong:
    return "signed long"s;

  case Type::Rational:
    return "rational"s;

  case Type::SRational:
    return "signed rational"s;
  }
  return "INVALID TYPE"s;
}

// TagField��TagValue�̌^�̒l1�̃o�C�g��
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

// TagField��TagValue�̎�蓾��^�Ƃ��̒l�̐�����o�C�g�����v�Z
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