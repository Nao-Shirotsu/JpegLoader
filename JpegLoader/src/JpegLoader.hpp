#pragma once

#include <string>
#include <vector>

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
  // binaryData��begin�Ԃ���end�Ԃ܂œǂ�ŕW���o��
  void DumpInRange(int, int size) noexcept(false);

  // Exif����Jpeg���ǂ������f
  bool IsJpegFile();

  std::vector<unsigned char> binaryData;
};

int Deserialize2bytes(unsigned char first, unsigned char second);