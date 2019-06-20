#pragma once

#include "JpegTagField.hpp"

namespace jpeg::tag {

// tag::Field�̃C���X�^���X��
class IFieldEmplacer {
public:
  inline virtual ~IFieldEmplacer() {}
  virtual void Emplace(Field&& field) = 0;
};

}