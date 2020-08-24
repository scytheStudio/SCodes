

# SBarcodeReader

This project is Qt & Qml wrapper for [ZXing-C++ Library](https://github.com/nu-book/zxing-cpp) that is used for decoding 1D and 2D barcodes. This particular C++ ZXing port is one of the most recent C++ versions of popular ZXing library using modern compilers.

The goal of this project was to create convenient filter allowing to process video frame from phone camera. Thanks to SBarcodeReader you can start scanning barcodes in few steps. We used Qt 5.15 to built both wrapper and example application.

## Supported Formats

ZXing-C++ Library supports many other formats, but currently it is possible to only use these two barcode formats.

|    1D    |    2D
| -------- | -------
| Code 128 | QR Code


# How to use wrapper?
![SBarcodeReader gif](/scanner.gif)


All you need to do is to follow these steps.

1. Add SBarcodeReader as submodule, by typing `git submodule add git@gitlab.com:scythestudio/sbarcodereader.git`
2. Update submodule `git submodule update --recursive --init` (you can also put wrapper files to your project manually without adding submodule)
3. Add `include(sbarcodereader/src/SBarcodeReader.pri` to your .pro file
4. Import SBarcodeReader in your Qml file `import com.scythestudio.sbarcodereader 1.0`
5. Import multimedia module `import QtMultimedia 5.15`
6. If build fails, try to add `CONFIG += c++17` to your .pro file
7. You are done. Get inspired by [QML Barcode Reader demo](https://github.com/scytheStudio/SBarcodeReader/blob/master/examples/QmlBarcodeReader/qml/ScannerPage.qml) to test wrapper.

If you use Qt version older than 5.15 you need to manually register BarcodeFilter e.g. in main.cpp. Use this line for this purpose:
```
qmlRegisterType<MyClass>("com.scythestudio.sbarcodereader", 1, 0, "BarcodeFilter");
```
Don't forget to firstly include BarcodeFilter.
