#include <iostream>

#include "JpegDeserialize.hpp"
#include "JpegTagUtils.hpp"

namespace jpeg::tag {

// TagFieldのTagValueの型を文字列に変換
std::string TypeToStr(const tag::Type type) {
  using namespace std::literals::string_literals;
  switch (type) {
  case Type::Byte:
    return "byte"s;

  case Type::Ascii:
    return "ascii"s;

  case Type::Undefined:
    return "undefined"s;

  case Type::Short:
    return "short"s;

  case Type::Long:
    return "long"s;

  case Type::SLong:
    return "signed long"s;

  case Type::Rational:
    return "rational"s;

  case Type::SRational:
    return "signed rational"s;
  }
  return "INVALID TYPE"s;
}

// TagFieldのTagValueの型の値1つのバイト数
uint32_t SizeOf(const Type type) {
  switch (type) {
  case Type::Byte:
  case Type::Ascii:
  case Type::Undefined:
    return 1;

  case Type::Short:
    return 2;

  case Type::Long:
  case Type::SLong:
    return 4;

  case Type::Rational:
  case Type::SRational:
    return 8;
  }
  return 0;
}

// TagFieldのTagValueの取り得る型とその値の数からバイト数を計算
uint32_t ByteLengthOf(const Type type, const int32_t count) {
  return count * tag::SizeOf(type);
}

// TagFieldの各IDを文字列に変換
std::string IDToString(const uint8_t byte1, const uint8_t byte2) {
  using namespace std::literals::string_literals;
  uint32_t id = Deserialize(byte1, byte2);

  switch (id) {
  case 0x010e:
    return "ImageDescription"s;

  case 0x010f:
    return "Maker"s;

  case 0x0110:
    return "Model"s;

  case 0x0112:
    return "Orientation"s;

  case 0x011a:
    return "XResolution"s;

  case 0x011b:
    return "YResolution"s;

  case 0x0128:
    return "ResolutionUnit"s;

  case 0x012d:
    return "TransferFunction"s;

  case 0x0131:
    return "Software"s;

  case 0x0132:
    return "DateTime"s;

  case 0x013b:
    return "Artist"s;

  case 0x013e:
    return "WhitePoint"s;

  case 0x013f:
    return "PrimaryChromaticities"s;

  case 0x0211:
    return "YCbCrCoefficients"s;

  case 0x0213:
    return "YCbCrPositioning"s;

  case 0x0214:
    return "ReferenceBlackWhite"s;

  case 0x8298:
    return "Copyright"s;

  case 0x8769:
    return "Exif IFD Pointer"s;

  case 0x8825:
    return "GPSInfo IFD Pointer"s;

  case 0x829a:
    return "ExposureTime"s;

  case 0x829d:
    return "FNumber"s;

  case 0x8822:
    return "ExposureProgram"s;

  case 0x8824:
    return "SpectralSensitivity"s;

  case 0x8827:
    return "PhotographicSensitivity"s;

  case 0x8828:
    return "OECF"s;

  case 0x8830:
    return "SensitivityType"s;

  case 0x8831:
    return "StandardOutputSensitivity"s;

  case 0x8832:
    return "RecommendedExposureIndex"s;

  case 0x8833:
    return "ISOSpeed"s;

  case 0x8834:
    return "ISOSpeedLatitudeyyy"s;

  case 0x8835:
    return "ISOSpeedLatitudezzz"s;

  case 0x9000:
    return "ExifVersion"s;

  case 0x9003:
    return "DateTimeOriginal"s;

  case 0x9004:
    return "DateTimeDigitized"s;

  case 0x9101:
    return "ComponentsConfiguration"s;

  case 0x9102:
    return "CompressedBitsPerPixel"s;

  case 0x9201:
    return "ShutterSpeedValue"s;

  case 0x9202:
    return "ApertureValue"s;

  case 0x9203:
    return "BrightnessValue"s;

  case 0x9204:
    return "ExposureBiasValue"s;

  case 0x9205:
    return "MaxApertureValue"s;

  case 0x9206:
    return "SubjectDistance"s;

  case 0x9207:
    return "MeteringMode"s;

  case 0x9208:
    return "LightSource"s;

  case 0x9209:
    return "Flash"s;

  case 0x920a:
    return "FocalLength"s;

  case 0x9214:
    return "SubjectArea"s;

  case 0x927c:
    return "MakerNote"s;

  case 0x9286:
    return "UserComment"s;

  case 0x9290:
    return "SubSecTime"s;

  case 0x9291:
    return "SubSecTimeOriginal"s;

  case 0x9292:
    return "SubSecTimeDigitized"s;

  case 0xa000:
    return "FlashpixVersion"s;

  case 0xa001:
    return "ColorSpace"s;

  case 0xa002:
    return "PixelXDimension"s;

  case 0xa003:
    return "PixelYDimension"s;

  case 0xa004:
    return "RelatedSoundFile"s;

  case 0xa005:
    return "Interoperability IFD Pointer"s;

  case 0xa20b:
    return "FlashEnergy"s;

  case 0xa20c:
    return "SpatialFrequencyResponse"s;

  case 0xa20e:
    return "FocalPlaneXResolution"s;

  case 0xa20f:
    return "FocalPlaneYResolution"s;

  case 0xa210:
    return "FocalPlaneResolutionUnit"s;

  case 0xa214:
    return "SubjectLocation"s;

  case 0xa215:
    return "ExposureIndex"s;

  case 0xa217:
    return "SensingMethod"s;

  case 0xa300:
    return "FileSource"s;

  case 0xa301:
    return "SceneType"s;

  case 0xa302:
    return "CFAPattern"s;

  case 0xa401:
    return "CustomRendered"s;

  case 0xa402:
    return "ExposureMode"s;

  case 0xa403:
    return "WhiteBalance"s;

  case 0xa404:
    return "DigitalZoomRatio"s;

  case 0xa405:
    return "FocalLengthIn35mmFilm"s;

  case 0xa406:
    return "SceneCaptureType"s;

  case 0xa407:
    return "GainControl"s;

  case 0xa408:
    return "Contrast"s;

  case 0xa409:
    return "Saturation"s;

  case 0xa40a:
    return "Sharpness"s;

  case 0xa40b:
    return "DeviceSettingDescription"s;

  case 0xa40c:
    return "SubjectDistanceRange"s;

  case 0xa420:
    return "ImageUniqueID"s;

  case 0xa430:
    return "CameraOwnerName"s;

  case 0xa431:
    return "BodySerialNumber"s;

  case 0xa432:
    return "LensSpecification"s;

  case 0xa433:
    return "LensMake"s;

  case 0xa434:
    return "LensModel"s;

  case 0xa435:
    return "LensSerialNumber"s;

  case 0xa500:
    return "Gamma"s;

  case 0x0000:
    return "GPSVersionID"s;

  case 0x0001:
    return "GPSLatitudeRef"s;

  case 0x0002:
    return "GPSLatitude"s;

  case 0x0003:
    return "GPSLongitudeRef"s;

  case 0x0004:
    return "GPSLongitude"s;

  case 0x0005:
    return "GPSAltitudeRef"s;

  case 0x0006:
    return "GPSAltitude"s;

  case 0x0007:
    return "GPSTimeStamp"s;

  case 0x0008:
    return "GPSSatellites"s;
  
  case 0x0009:
    return "GPSStatus"s;

  case 0x000a:
    return "GPSMeasureMode"s;

  case 0x000b:
    return "GPSDOP"s;

  case 0x000c:
    return "GPSSpeedRef"s;

  case 0x000d:
    return "GPSSpeed"s;

  case 0x000e:
    return "GPSTrackRef"s;

  case 0x000f:
    return "GPSTrack"s;

  case 0x0010:
    return "GPSImgDirectionRef"s;

  case 0x0011:
    return "GPSImgDirection"s;

  case 0x0012:
    return "GPSMapDatum"s;

  case 0x0013:
    return "GPSDestLatitudeRef"s;

  case 0x0014:
    return "GPSDestLatitude"s;

  case 0x0015:
    return "GPSDestLongitudeRef"s;

  case 0x0016:
    return "GPSDestLongitude"s;

  case 0x0017:
    return "GPSDestBearingRef"s;

  case 0x0018:
    return "GPSDestBearing"s;

  case 0x0019:
    return "GPSDestDistanceRef"s;

  case 0x001a:
    return "GPSDestDistance"s;

  case 0x001b:
    return "GPSProcessingMethod"s;

  case 0x001c:
    return "GPSAreaInformation"s;

  case 0x001d:
    return "GPSDateStamp"s;

  case 0x001e:
    return "GPSDifferential"s;

  case 0x001f:
    return "GPSHPositioningError"s;
  }

  return "UNKNOWN TAG ID"s;
}

} // namespace jpeg::tag