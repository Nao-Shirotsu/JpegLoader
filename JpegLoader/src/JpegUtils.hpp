#pragma once

#include <vector>
#include <optional>

namespace jpeg {

std::optional<const char*> ValueTypeToString(const int32_t type);
std::optional<uint16_t> SizeOfValueType(const int32_t type);
std::optional<int32_t> TagValueLength(const int32_t type, const int32_t count);
void OutputTagFieldValue(std::vector<uint8_t>::const_iterator itr, const int32_t sizeOfValueType, const int32_t count);
  
}// namespace jpeg