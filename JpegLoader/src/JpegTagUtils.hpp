#pragma once

#include <vector>
#include <optional>

namespace jpeg::tag {

std::optional<const char*> TypeToStr(const int32_t type);
std::optional<uint16_t> SizeOf(const int32_t type);
std::optional<int32_t> ByteLengthOf(const int32_t type, const int32_t count);
void OutputValue(std::vector<uint8_t>::const_iterator itr, const int32_t sizeOfValueType, const int32_t count);
  
}// namespace jpeg::tag