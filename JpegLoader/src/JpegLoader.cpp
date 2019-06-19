#include <array>
#include <fstream>
#include <iostream>
#include <optional>

#include "JpegDeserialize.hpp"
#include "JpegLoader.hpp"
#include "JpegUtils.hpp"

namespace jpeg {

Loader::Loader(const std::string& fileName)
  : binaryData()
  , basePosItr() {
  std::ifstream ifs(fileName);
  if (!ifs) {
    std::cout << "*ERROR* image file loading failed" << std::endl;
    return;
  }

  // 一応先頭に飛ぶ
  ifs.seekg(0);

  // EOFまで1バイトずつ読みだしてメンバのbinaryDataにemplace
  while (!ifs.eof()) {
    binaryData.emplace_back(ifs.get());
  }
}

void Loader::DumpRawData() {
  for (const auto& elem : binaryData) {
    std::cout << elem;
  }
}

void Loader::DumpExif() {
  if (binaryData.empty()) {
    return;
  }

  auto [itr, segmentSize] = GetItrAtExifId();

  std::cout << "Exif ID code   | ";
  for (int i = 0; i < 6; ++i) {
    std::cout << static_cast<int32_t>(*itr) << " ";
    ++itr;
  }
  std::cout << std::endl;

  // ===== test output =====
  // for( int i = 0; i < segmentSize; ++i ) {
  //  std::cout << *itr;
  //  ++itr;
  // }
  // =======================

  std::cout << "byte order     | ";
  std::cout << static_cast<int32_t>(*itr) << " ";
  ++itr;
  std::cout << static_cast<int32_t>(*itr) << std::endl;
  ++itr;

  std::cout << "tiff version   | ";
  std::cout << static_cast<int32_t>(*itr) << " ";
  ++itr;
  std::cout << static_cast<int32_t>(*itr) << std::endl;
  ++itr;

  std::array<uint8_t, 4> readBytes;
  for (auto& elem : readBytes) {
    elem = *itr;
    ++itr;
  }
  uint32_t skipOffset = jpeg::Deserialize(readBytes[0], readBytes[1], readBytes[2], readBytes[3]);
  std::cout << "offset to IFD0 | " << skipOffset << std::endl;

  // タグフィールドのvalueオフセットの起点位置
  basePosItr = itr - 8;

  // これでIFD0の先頭まで飛ぶ
  OutputIFD(basePosItr + skipOffset);
}

void Loader::OutputIFD(std::vector<uint8_t>::const_iterator itr) {
  std::cout << "\n~~~~~~~~~~~~~~~~IFD~~~~~~~~~~~~~~~~" << std::endl;

  // 読み出した値を一時保存するarray
  std::array<uint8_t, 4> bytes;

  // IFDのタグ数を読む
  bytes[0] = *itr;
  ++itr;
  bytes[1] = *itr;
  ++itr;
  uint32_t numOfTag = jpeg::Deserialize(bytes[0], bytes[1]);
  std::cout << "Num of Tag: " << numOfTag << std::endl;

  // タグの数ぶんタグフィールドを読む
  for (int i = 0; i < numOfTag; ++i) {
    OutputTagField(itr);
    itr += 12; // タグフィールドは12バイトで固定だから、これで次のタグを読める
  }

  // ===== test output =====
  // std::cout << "read all tags" << std::endl;
  // =======================

  // 次のIFDへのoffsetを読む
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  auto offset = jpeg::Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);
  std::cout << "Offset : " << numOfTag << std::endl;

  // ===== test output =====
  // std::cout << "read offset to next IFD" << std::endl;
  // =======================

  // N-th IFD内のM-thへのoffsetが0ならM-th IFDは存在しない
  if (offset == 0) {
    std::cout << "\n~~READING DONE, EXIT APP1 SEGMENT~~" << std::endl;
    return;
  }

  itr = basePosItr + offset;

  // ===== test output =====
  // std::cout << "itr addition\n------" << std::endl;
  // =======================

  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  OutputIFD(itr);
}

void Loader::OutputTagField(std::vector<uint8_t>::const_iterator itr) {
  std::array<uint8_t, 4> bytes;

  // タグ番号とタグタイプを読む
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  const auto tagNumber = jpeg::Deserialize(bytes[0], bytes[1]);
  const auto tagType = jpeg::Deserialize(bytes[2], bytes[3]);
  std::cout << "\n=======Tag Field=======" << std::endl;
  std::cout << "Number   : " << static_cast<int32_t>(bytes[0]) << " " << static_cast<int32_t>(bytes[1]) << std::endl;

  if (auto tagTypeStr = ValueTypeToString(Deserialize(bytes[2], bytes[3])); tagTypeStr) {
    std::cout << "Type     : " << tagTypeStr.value() << std::endl;
  }

  // タグに含まれる値の数を読む
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }
  const auto tagCount = jpeg::Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);
  std::cout << "Count    : " << tagCount << std::endl;

  const auto valueByteLength = TagValueLength(tagType, tagCount).value();

  // タグのvalueへのoffsetを読む
  for (auto& elem : bytes) {
    elem = *itr;
    ++itr;
  }

  std::cout << "Val/offs : ";
  if (valueByteLength > 4) {
    int32_t skipOffset = Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]);
    OutputTagFieldValue(basePosItr + skipOffset, SizeOfValueType(tagType).value(), tagCount);
  }
  else {
    switch (SizeOfValueType(tagType).value()) {
    case 1:
      std::cout << Deserialize(bytes[0]) << std::endl;
      break;

    case 2:
      std::cout << Deserialize(bytes[0], bytes[1]) << std::endl;
      break;

    case 4:
      std::cout << Deserialize(bytes[0], bytes[1], bytes[2], bytes[3]) << std::endl;
      break;
    }
  }
  std::cout << "\nEND AT [" << (itr - binaryData.cbegin()) << "] bytes" << std::endl;
  std::cout << "=======================\n" << std::endl;
}


std::pair<std::vector<uint8_t>::const_iterator, int32_t> Loader::GetItrAtExifId() {
  auto itr = binaryData.cbegin();

  // 最初のffd8を飛ばす
  itr += 2;

  // 1つのセグメントのサイズ(bytes)
  int32_t segmentSize;

  // Exif情報があるAPP1セグメント直前まで飛ばす
  while (true) {
    uint8_t ff = *itr;
    ++itr;

    uint8_t marker = *itr;
    ++itr;

    uint8_t segmentSizeByte1 = *itr;
    ++itr;

    uint8_t segmentSizeByte2 = *itr;
    ++itr;

    segmentSize = jpeg::Deserialize(segmentSizeByte1, segmentSizeByte2);

    if (marker == 0xe1) {
      break;
    }

    itr += segmentSize - 2;
  }

  return std::make_pair(itr, segmentSize);
}

} // namespace jpeg