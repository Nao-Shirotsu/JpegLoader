#pragma once

#include <vector>
#include <string>

#include "JpegTagType.h"

namespace jpeg::tag {

// Tag::Type�𕶎���ɕϊ�
std::string TypeToStr(const Type type);

// Tag::Type��1�̒l�̃o�C�g��
uint32_t SizeOf(const Type type);

// Tag::Type�̒l��������Ɖ��o�C�g�ɂȂ邩
uint32_t ByteLengthOf(const Type type, const int32_t count);

std::string IDToString(const uint8_t byte1, const uint8_t byte2);

} // namespace jpeg::tag:tag