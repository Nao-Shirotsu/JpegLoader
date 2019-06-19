#pragma once

#include <vector>
#include <cstdint>
#include <array>

#include "JpegTagType.h"

namespace jpeg::tag {

// uint8_t�^�̃o�C�g��Ƃ��ă^�O�t�B�[���h��value��ێ�����N���X
class Field {
public:
  // �t�B�[���h�̎��ʎq, value�̌^, �f�[�^�̌�������������B
  // �܂��Aitr�ɂ���ăo�C�g���ǂ��byteVec�Ɋi�[����BbasePosItr�͊eoffset�̋N�_���w���C�e���[�^��DI�B
  Field(std::vector<uint8_t>::const_iterator itr, std::vector<uint8_t>::const_iterator basePositr_);

  // ���̃t�B�[���h��IFD�ւ̃|�C���^�ł��邩�ǂ���
  bool IsPointer();

  // �W���o�͂���
  void Print();

private:
  void PrintImpl();

  const std::vector<uint8_t>::const_iterator basePosItr;
  Type type;
  uint32_t length;
  uint32_t tailPos; // ���̃^�O�t�B�[���h�̖����ʒu��basePos���牽�o�C�g�̈ʒu��
  std::array<uint8_t, 2> id;
  std::vector<uint8_t> byteVec;
};

}// namespace jpeg::tag