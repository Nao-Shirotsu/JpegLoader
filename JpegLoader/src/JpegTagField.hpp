#pragma once

#include <vector>
#include <cstdint>
#include <array>
#include <optional>

#include "JpegTagType.h"

namespace jpeg::tag {

// uint8_t型のバイト列としてタグフィールドのvalueを保持するクラス
class Field {
public:
  // フィールドの識別子, valueの型, データの個数を初期化する。
  // また、itrによってバイト列を読んでbyteVecに格納する。ExifBasePosItr()は各offsetの起点を指すイテレータのDI。
  Field(std::vector<uint8_t>::const_iterator itr, const std::vector<uint8_t>::const_iterator basePosItr_);

  // このフィールドがIFDへのポインタであればoffsetを、そうでなければnulloptを返す
  std::optional<std::vector<uint8_t>::const_iterator> NextIFDItr() const;

  // 標準出力する
  void Print() const;

private:
  void PrintImpl() const;

  const std::vector<uint8_t>::const_iterator basePosItr;
  Type type;
  uint32_t length;
  uint32_t tailPos; // このタグフィールドの末尾位置がbasePosから何バイトの位置か
  std::array<uint8_t, 2> id;
  std::vector<uint8_t> byteVec;
};

}// namespace jpeg::tag