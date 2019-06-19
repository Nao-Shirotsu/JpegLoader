#include <type_traits>
#include <cstdint>

template <typename T, typename = void>
struct is_uint8 : public std::false_type {};

template <typename T>
struct is_uint8<T, std::enable_if_t<std::is_same_v<T, uint8_t>>> : std::true_type {};

// Tがuint8_tであるかどうかをbool型で返す
template <typename T>
constexpr bool is_uint8_v = is_uint8<T>::value;

namespace jpeg {

// ↓の再帰終了時
template <int32_t recursionDepth>
constexpr void ErrorizeWhenInvalidByteData() {
}

// "argsの型が全てuint8_t" && "再帰回数が4回以下" なら何もしない、でなければコンパイルエラー
template <int32_t recursionDepth, class Head, class... Tail, std::enable_if_t<is_uint8_v<Head>, std::nullptr_t> = nullptr>
constexpr void ErrorizeWhenInvalidByteData(Head head, Tail... tail) {
  // 4B以上のバイト列をデシリアライズしようとするとコンパイルエラー
  static_assert(recursionDepth < 4, "**ERROR** OVERFLOW in Deserialize()");
  ErrorizeWhenInvalidByteData<recursionDepth + 1>(tail...);
}

// ↓の実装 ビッグエンディアンでバイト列を解釈する (ここだけ変えればリトルエンディアンにも対応できる)
constexpr uint32_t DeserializeImpl(const std::initializer_list<uint8_t>& byteRow) {
  int shiftAmount = byteRow.size() - 1;
  int result = 0;
  for (uint8_t byte : byteRow) {
    result += static_cast<uint32_t>(byte) << 8 * shiftAmount;
    --shiftAmount;
  }
  return result;
}

// 4つ以下のuint8_tを受け取り、uint32_tにデシリアライズ
template <class... Args>
constexpr uint32_t Deserialize(Args... args) {
  ErrorizeWhenInvalidByteData<0>(args...);
  return DeserializeImpl({ args... });
}

}// namespace jpeg