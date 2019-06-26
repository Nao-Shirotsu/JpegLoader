# Exif Info Printer for JPEG

### 概要

JPEG 画像ファイルの Exif 情報を解析して CLI ターミナル上に出力します。

### ビルド
*Requirement* : C++17 対応コンパイラ     
CMake を利用してビルドできます。このプロジェクトのルートディレクトリで以下のコマンドを実行すると`bin/jpeg_exif_info`が生成されます。

```
$ cmake .
$ make
```

### 使い方

以下のようにビルドされた実行ファイルに JPEG 画像のパスを指定すると、Exif 情報を出力します。

```
$ pwd
/PATH/TO/THIS/PROJECT/JpegLoader
$ ./bin/jpeg_exif_info YOUR_PICTURE.jpg
<< Exif Information >>

[Maker]
EXAMPLE MAKER NAME

[Model]
EXAMPLE CAMERA NAME

...
```

また、`--tag` オプションを付けるとタグフィールドを強調して出力します。

```
$ ./bin/jpeg_exif_info --tag YOUR_PICTURE.jpg
byte order --> Big-Endian

=======Tag Field=======
ID     : 0x[1 f] aka Maker
Type   : ascii
Length : 19
Value  : EXAMPLE MAKER NAME
=======================

=======Tag Field=======
ID     : 0x[1 10] aka Model
Type   : ascii
Length : 20
Value  : EXAMPLE CAMERA NAME
=======================

...
```

