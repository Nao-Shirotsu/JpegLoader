#pragma once

#include <string>
#include <vector>

// JPEG画像をロードして保持するクラス
class JpegLoader {
public:
  // ファイル名を受け取ってデータをbinaryDataに格納
  JpegLoader(const std::string& fileName);

  // データをそのまま標準出力
  void DumpRawData();

  // Exifをタグ付きで標準出力
  void DumpExif();

private:
  // binaryDataのbegin番からend番まで読んで標準出力
  //void DumpInRange(int, int size) noexcept(false);

  // ExifからJpegかどうか判断
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