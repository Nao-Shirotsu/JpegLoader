#pragma once

#include <string>
#include <vector>
#include <utility>

// JPEG�摜�����[�h���ĕێ�����N���X
class JpegLoader {
public:
  // �t�@�C�������󂯎���ăf�[�^��binaryData�Ɋi�[
  JpegLoader(const std::string& fileName);

  // �f�[�^�����̂܂ܕW���o��
  void DumpRawData();

  // Exif���^�O�t���ŕW���o��
  void DumpExif();

private:
  // Exif���ʃR�[�h�̐擪�܂ł��w���C�e���[�^�ƃZ�O�����g�T�C�Y�̃y�A���擾
  std::pair<std::vector<uint8_t>::iterator, int32_t> GetItrAtExifId();

  // �摜�f�[�^�����̂܂ܑS�����[�h����x�N�^
  std::vector<uint8_t> binaryData;
};