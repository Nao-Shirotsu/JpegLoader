#pragma once

#include <string>
#include <vector>

#include "JpegTagField.hpp"

namespace jpeg {

// JPEG画像をロードして保持するクラス
class Loader {
public:
  // ファイル名を受け取ってデータをbinaryDataに格納
  Loader(const std::string& fileName);

  // データをそのまま標準出力
  void DumpRawData() const;

  // Exifをタグ付きで標準出力
  void DumpExifTagFields() const;

  // Exifを読みやすく表示
  void DumpExif() const;

private:
  // 0th IFDの先頭を指すitrを渡し、以降のIFDを全て解析してexifTagFieldを構築する
  void ParseIFD(std::vector<uint8_t>::const_iterator itr);

  // ExifTagフィールドの起点を指すイテレータを取得
  std::vector<uint8_t>::const_iterator ExifBasePosItr() const;

  // 全てのExifタグフィールドを保持する
  std::vector<tag::Field> exifTagFields;

  // 画像データをそのまま全部ロードするベクタ
  std::vector<uint8_t> binaryData;
};

} // namespace jpeg