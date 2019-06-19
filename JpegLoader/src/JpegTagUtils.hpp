#pragma once

#include <vector>
#include <string>

#include "JpegTagType.h"

namespace jpeg::tag {

// Tag::Typeを文字列に変換
std::string TypeToStr(const Type type);

// Tag::Typeの1つの値のバイト数
uint32_t SizeOf(const Type type);

// Tag::Typeの値が何個あると何バイトになるか
uint32_t ByteLengthOf(const Type type, const int32_t count);

// タグフィールドの先頭を指すイテレータを渡すと情報を出力
void OutputValue(std::vector<uint8_t>::const_iterator itr, const int32_t sizeOfValueType, const int32_t count);

} // namespace jpeg::tag:tag