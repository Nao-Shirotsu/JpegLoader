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
  //void DumpInRange(int, int size) noexcept(false);

  // Exif����Jpeg���ǂ������f
  //bool IsJpegFile();

  std::vector<uint8_t> binaryData;
};

template <class... Args>
int32_t Deserialize(Args... args) {
  auto byteRow = std::initializer_list{ args... };
  int shiftAmount = byteRow.size() - 1;
  int result = 0;
  for (uint8_t byte : byteRow) {
    result += static_cast<uint32_t>(byte) << 8 * shiftAmount;
    --shiftAmount;
  }
  return result;
}