#pragma once

#include <vector>
#include <cstdint>

// �C�ӂ̌^�̒l���i�[����Exif�Z�O�����g�̃^�O�t�B�[���h�N���X
template <typename T>
class TagField {
public:
  TagField(uint16_t tagNumber_, uint16_t type_, uint32_t count_, uint32_t offset){}

private:
  std::vector<T> valueVec;
};