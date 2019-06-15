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
  // 0th IFDの先頭を指すitrを渡して再帰的に出力する
  void OutputIFD(std::vector<uint8_t>::iterator itr);

  // 各タグフィールドの先頭を指すitrを渡して出力する
  void OutputTagField(std::vector<uint8_t>::iterator itr);

  // Exif識別コードの先頭までを指すイテレータとセグメントサイズのペアを取得
  std::pair<std::vector<uint8_t>::iterator, int32_t> GetItrAtExifId();

  // 画像データをそのまま全部ロードするベクタ
  std::vector<uint8_t> binaryData;

  // APP1タグフィールドのvalueの位置に飛ぶためのオフセットの起点位置
  std::vector<uint8_t>::iterator basePosItr;
};