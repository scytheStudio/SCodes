

# SCodes

This project is Qt & Qml wrapper for [ZXing-C++ Library](https://github.com/nu-book/zxing-cpp) that is used for decoding and generating 1D and 2D barcodes. This particular C++ ZXing port is one of the most recent C++ versions of popular ZXing library using modern compilers.

The goal of this project was to create convenient filter allowing to process video frame from phone camera. Thanks to SCodes you can start scanning barcodes in few steps. We used Qt 5.15 to built both wrapper and example application, but it's compatible with older Qt versions as well. Feel free to read ["How to scan barcodes in Qt Qml application" blog post](https://scythe-studio.com/blog/how-to-scan-barcodes-in-qt-qml-application) if you are interested in the details behind scanning mechanism. This blog post contains ste by step tutorial on how to do that.

SCodes supports generating barcodes as well. It will be covered in another blog post soon.

## Supported Formats

ZXing-C++ Library supports many other formats, but currently it is possible to only use these two barcode formats.

|    1D    |    2D
| -------- | -------
| Code 128 | QR Code


# How to use wrapper?
![SCodes gif](/scanner.gif)


All you need to do is to follow these steps.

1. Add SCodes as submodule, by typing `git submodule add git@gitlab.com:scythestudio/scodes.git`
2. Update submodule `git submodule update --recursive --init` (you can also put wrapper files to your project manually without adding submodule)
3. Add `include(scodes/src/SCodes.pri` to your .pro file
4. Import SCodes in your Qml file `import com.scythestudio.scodes 1.0`
5. Import multimedia module `import QtMultimedia 5.15`
6. If build fails, try to add `CONFIG += c++17` to your .pro file
7. You are done. Get inspired by [QML Barcode Reader demo](https://github.com/scytheStudio/SCodes/blob/master/examples/QmlBarcodeReader/qml/ScannerPage.qml) to test wrapper.

If you use Qt version older than 5.15 you need to manually register SBarcodeFilter e.g. in main.cpp. Use this line for this purpose:
```
qmlRegisterType<MyClass>("com.scythestudio.scodes", 1, 0, "SBarcodeFilter");
```
Don't forget to firstly include SBarcodeFilter.

In the case of barcodes scanning you will need to expose SBarcodeGenerator class.
If you are using SCodes for this case, check our [QML Barcode Generator demo](https://github.com/scytheStudio/SCodes/blob/master/examples/QmlBarcodeGenerator/qml/GeneratorPage.qml) to do that easily!
