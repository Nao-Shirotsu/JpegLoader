#pragma once

#include <string>
#include <vector>

#include "JpegTagIFieldEmplacer.hpp"
#include "JpegTagField.hpp"

namespace jpeg {

// JPEG�摜�����[�h���ĕێ�����N���X
class Loader : public tag::IFieldEmplacer {
public:
  // �t�@�C�������󂯎���ăf�[�^��binaryData�Ɋi�[
  Loader(const std::string& fileName);

  // �f�[�^�����̂܂ܕW���o��
  void DumpRawData() const;

  // Exif���^�O�t���ŕW���o��
  void DumpExif() const;

  void Emplace(tag::Field&& field) override;

private:
  void ConstructTagFields();

  // 0th IFD�̐擪���w��itr��n���čċA�I�ɏo�͂���
  void OutputIFD(std::vector<uint8_t>::const_iterator itr) const;

  // ExifTag�t�B�[���h�̋N�_���w���C�e���[�^���擾
  std::vector<uint8_t>::const_iterator ExifBasePosItr() const;

  // �S�Ă�Exif�^�O�t�B�[���h��ێ�����
  std::vector<tag::Field> exifTagFields;

  // �摜�f�[�^�����̂܂ܑS�����[�h����x�N�^
  std::vector<uint8_t> binaryData;
};

}// namespace jpeg