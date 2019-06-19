#pragma once

namespace jpeg::tag {

enum class Type {
  // 1 byte
  Byte = 1,
  Ascii = 2,
  Undefined = 7,

  // 2 bytes
  Short = 3,

  // 4 bytes
  Long = 4,
  SLong = 9,

  // 8bytes
  Rational = 5,
  SRational = 10,
};

}// namespace jpeg::tag