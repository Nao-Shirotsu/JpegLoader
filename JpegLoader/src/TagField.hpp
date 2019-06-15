#pragma once

#include <vector>
#include <cstdint>

// 任意の型の値を格納するExifセグメントのタグフィールドクラス
template <typename T>
class TagField {
public:
  TagField(uint16_t tagNumber_, uint16_t type_, uint32_t count_, uint32_t offset){}

private:
  std::vector<T> valueVec;
};