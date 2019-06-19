#include <type_traits>
#include <cstdint>

template <typename T, typename = void>
struct is_uint8 : public std::false_type {};

template <typename T>
struct is_uint8<T, std::enable_if_t<std::is_same_v<T, uint8_t>>> : std::true_type {};

// T��uint8_t�ł��邩�ǂ�����bool�^�ŕԂ�
template <typename T>
constexpr bool is_uint8_v = is_uint8<T>::value;

namespace jpeg {

// ���̍ċA�I����
template <int32_t recursionDepth>
constexpr void ErrorizeWhenInvalidByteData() {
}

// "args�̌^���S��uint8_t" && "�ċA�񐔂�4��ȉ�" �Ȃ牽�����Ȃ��A�łȂ���΃R���p�C���G���[
template <int32_t recursionDepth, class Head, class... Tail, std::enable_if_t<is_uint8_v<Head>, std::nullptr_t> = nullptr>
constexpr void ErrorizeWhenInvalidByteData(Head head, Tail... tail) {
  // 4B�ȏ�̃o�C�g����f�V���A���C�Y���悤�Ƃ���ƃR���p�C���G���[
  static_assert(recursionDepth < 4, "**ERROR** OVERFLOW in Deserialize()");
  ErrorizeWhenInvalidByteData<recursionDepth + 1>(tail...);
}

// ���̎��� �r�b�O�G���f�B�A���Ńo�C�g������߂��� (���������ς���΃��g���G���f�B�A���ɂ��Ή��ł���)
constexpr uint32_t DeserializeImpl(const std::initializer_list<uint8_t>& byteRow) {
  int shiftAmount = byteRow.size() - 1;
  int result = 0;
  for (uint8_t byte : byteRow) {
    result += static_cast<uint32_t>(byte) << 8 * shiftAmount;
    --shiftAmount;
  }
  return result;
}

// 4�ȉ���uint8_t���󂯎��Auint32_t�Ƀf�V���A���C�Y
template <class... Args>
constexpr uint32_t Deserialize(Args... args) {
  ErrorizeWhenInvalidByteData<0>(args...);
  return DeserializeImpl({ args... });
}

}// namespace jpeg