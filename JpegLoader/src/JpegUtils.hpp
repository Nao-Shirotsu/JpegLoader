#pragma once

#include <vector>
#include <optional>

#include "JpegTagType.h"

namespace jpeg::tag {

// Tag::Type�𕶎���ɕϊ�
const char* TypeToStr(const tag::Type type);

// Tag::Type�̃o�C�g��
std::optional<uint16_t> SizeOf(const int32_t type);

// Tag::Type�̒l��������Ɖ��o�C�g�ɂȂ邩
std::optional<int32_t> ByteLengthOf(const int32_t type, const int32_t count);

// �^�O�t�B�[���h�̐擪���w���C�e���[�^��n���Ə����o��
void OutputValue(std::vector<uint8_t>::const_iterator itr, const int32_t sizeOfValueType, const int32_t count);
  
}// namespace jpeg::tag