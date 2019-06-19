#pragma once

#include <string>
#include <vector>

#include "JpegTagIFieldEmplacer.hpp"
#include "JpegTagField.hpp"

namespace jpeg {

// JPEG画像をロードして保持するクラス
class Loader : public tag::IFieldEmplacer {
public:
  // ファイル名を受け取ってデータをbinaryDataに格納
  Loader(const std::string& fileName);

  // データをそのまま標準出力
  void DumpRawData() const;

  // Exifをタグ付きで標準出力
  void DumpExif() const;

  void Emplace(tag::Field&& field) override;

private:
  void ConstructTagFields();

  // 0th IFDの先頭を指すitrを渡して再帰的に出力する
  void OutputIFD(std::vector<uint8_t>::const_iterator itr) const;

  // ExifTagフィールドの起点を指すイテレータを取得
  std::vector<uint8_t>::const_iterator ExifBasePosItr() const;

  // 全てのExifタグフィールドを保持する
  std::vector<tag::Field> exifTagFields;

  // 画像データをそのまま全部ロードするベクタ
  std::vector<uint8_t> binaryData;
};

}// namespace jpeg