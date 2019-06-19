#pragma once

#include "JpegTagField.hpp"

namespace jpeg::tag {

// tag::Fieldのインスタンスを
class IFieldEmplacer {
public:
  inline virtual ~IFieldEmplacer() {}
  virtual void Emplace(Field&& field) = 0;
};

}