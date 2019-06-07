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
  void DumpInRange(int, int size) noexcept(false);

  // ExifからJpegかどうか判断
  bool IsJpegFile();

  std::vector<unsigned char> binaryData;
};

int Deserialize2bytes(unsigned char first, unsigned char second);