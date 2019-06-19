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

// �^�O�t�B�[���h�̐擪���w���C�e���[�^��n���Ə����o��
void OutputValue(std::vector<uint8_t>::const_iterator itr, const int32_t sizeOfValueType, const int32_t count);

} // namespace jpeg::tag:tag