#pragma once

#include <vector>
#include <optional>

#include "JpegTagType.h"

namespace jpeg::tag {

// Tag::Typeを文字列に変換
const char* TypeToStr(const tag::Type type);

// Tag::Typeのバイト数
std::optional<uint16_t> SizeOf(const int32_t type);

// Tag::Typeの値が何個あると何バイトになるか
std::optional<int32_t> ByteLengthOf(const int32_t type, const int32_t count);

// タグフィールドの先頭を指すイテレータを渡すと情報を出力
void OutputValue(std::vector<uint8_t>::const_iterator itr, const int32_t sizeOfValueType, const int32_t count);
  
}// namespace jpeg::tag