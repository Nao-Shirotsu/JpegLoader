#pragma once

#include <string>
#include <vector>
#include <utility>

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
  // Exif識別コードの先頭までを指すイテレータとセグメントサイズのペアを取得
  std::pair<std::vector<uint8_t>::iterator, int32_t> GetItrAtExifId();

  // 画像データをそのまま全部ロードするベクタ
  std::vector<uint8_t> binaryData;
};