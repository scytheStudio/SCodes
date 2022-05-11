

![SCodes ](/assets/scodes.png)
# SCodes

This project is Qt & Qml wrapper for [ZXing-C++ Library](https://github.com/nu-book/zxing-cpp) that is used for decoding and generating 1D and 2D barcodes. This particular C++ ZXing port is one of the most recent C++ versions of popular ZXing library using modern compilers.

The goal of this project was to create convenient filter allowing to process video frame from phone camera. Thanks to SCodes you can start scanning barcodes in few steps. We used Qt 5.15 to built both wrapper and example application, but it's compatible with older Qt versions as well. Feel free to read ["How to scan barcodes in Qt Qml application" blog post](https://scythe-studio.com/blog/how-to-scan-barcodes-in-qt-qml-application) if you are interested in the details behind scanning mechanism. This blog post contains step by step tutorial on how to do that.

SCodes supports generating barcodes as well. It is covered in ["How to generate barcode in Qt Qml" application blog post](https://scythe-studio.com/en/blog/how-to-generate-barcode-in-qt-qml-application).

---

[![Scythe Studio](./assets/scytheStudioLogo.png)](https://scythe-studio.com)

[![Built with Qt](./assets/buildWithQt.png)](https://qt.io)

---

## Supported Formats

There are plenty of supported formats and we constantly work on adding new.

## Supported 1D Formats

|      Format      |    Supports scanning     |    Supports generating
| ---------------- | ------------------------ |    -------------------
|      UPC-A       |   <center>✔️</center>    |   <center>✔️</center>
|      UPC-E       |   <center>✔️</center>    |   <center>✔️</center>
|      EAN-8       |   <center>✔️</center>    |   <center>✔️</center>
|      EAN-13      |   <center>✔️</center>    |   <center>✔️</center>
|     DataBar      |   <center>✔️</center>    |   <center>❌</center>
| DataBar Expanded |   <center>❌</center>    |   <center>❌</center>
|     Code 39      |   <center>✔️</center>    |   <center>✔️</center>
|     Code 93      |   <center>✔️</center>    |   <center>✔️</center>
|     Code 128     |   <center>✔️</center>    |   <center>✔️</center>
|     Codabar      |   <center>✔️</center>    |   <center>✔️</center>
|       ITF        |   <center>❌</center>    |   <center>✔️</center>

## Supported 2D Formats

|    Format    |    Supports scanning     |    Supports generating
| ------------ | ------------------------ |    -------------------
|   QR Code    |   <center>✔️</center>    |   <center>✔️</center>
|  DataMatrix  |   <center>✔️</center>    |   <center>✔️</center>
|    Aztec     |   <center>✔️</center>    |   <center>✔️</center>
|    PDF417    |   <center>✔️</center>    |   <center>✔️</center>
|   MaxiCode   |   <center>❌</center>    |   <center>❌</center>


# How to use wrapper?
![SCodes Scanner Preview](/assets/scanner.gif)![SCodes Generator Preview](/assets/generator.gif)

## Using 

### QMake
All you need to do is to follow these steps.

1. Add SCodes as submodule, by typing `git submodule add git@gitlab.com:scythestudio/scodes.git`
2. Update submodule `git submodule update --recursive --init` (you can also put wrapper files to your project manually without adding submodule)
3. Add `include(scodes/src/SCodes.pri` to your .pro file
4. Import SCodes in your Qml file `import com.scythestudio.scodes 1.0`
5. Import multimedia module `import QtMultimedia 5.15`
6. If build fails, try to add `CONFIG += c++17` to your .pro file
7. You are done. Get inspired by [QML Barcode Reader demo](https://github.com/scytheStudio/SCodes/blob/master/examples/QmlBarcodeReader/qml/ScannerPage.qml) to test wrapper.

### CMake

1. Add SCodes as submodule, by typing `git submodule add git@gitlab.com:scythestudio/scodes.git`
2. Update submodule `git submodule update --recursive --init` (you can also put wrapper files to your project manually without adding submodule)
3. Add to your project SCodes library

```cmake
add_subdirectory(SCodes)
```

4. Link SCodes library to your library or executable. 

```cmake 
target_link_libraries(${PROJECT_NAME} PUBLIC SCodes)
```

5. Import SCodes in your Qml file 

```qml
import com.scythestudio.scodes 1.0
```

6. You are done. Get inspired by [QML Barcode Reader demo](https://github.com/scytheStudio/SCodes/blob/master/examples/QmlBarcodeReader/qml/ScannerPage.qml) to test wrapper.

### Trying various formats
`SBarcodeFilter` is a class that you need to use for scanning case. By default it scans only specific basic formats of code (Code 39, Code 93, Code 128, QR Code and DataMatrix.).
The goal of that is to limit number of possible formats and improve performance.

To specify formats that should be accepted by the `SBarcodeFilter` instance, you need to set it's `format` property accordingly. This property allows setting multiple enum values as it's for flags. Add the following to your `SBarcodeFilter` item in Qml code:
```qml
Component.onCompleted: {
    barcodeFilter.format = SCodes.OneDCodes
}
```
See the enumeration values that represent supported formats in [SBarcodeFormat.h](https://github.com/scytheStudio/SCodes/blob/master/src/SBarcodeFormat.h)
To accept all supported formats use `SCodes.Any`.

## Note 

Both build systems have their examples located in same directory. All you need to do is to just open proper file(CMakeLists.txt or *.pro file) for different build system to be used.


### Supported Kits for Examples


| PROJECT | BUILD SYSTEM | WINDOWS-MinGW | WINDOWS-MSVC | LINUX-GCC | ANDROID |
| ------ | ------ | ------ | ------ | ------ | ------ |
| QmlBarcodeReader | qmake |<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|
| QmlBarcodeGenerator | qmake |<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|
| QmlBarcodeReader | cmake |<center>❌</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|
| QmlBarcodeGenerator | cmake |<center>❌</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|


To be able to build cmake based examples for android devices, you will need to set ANDROID_NDK environment variable in project build settings to NDK version 22 or greater(tested on NDK version 22.1.7171670).


# About Scythe Studio
SCodes project was developed and is maintained mainly by Scythe Studio company.
We are an official Qt Service Partner and a provider of Qt Software Development services including:
- Desktop applications development
- Mobile applications development
- Embedded systems development
- Qt and C++ consulting
- UI/UX designing

Do not hesitate visting https://scythe-studio.com to discover our capabilities and learn more about Qt Software Development from [Scythe Studio Blog](https://scythe-studio.com/en/blog).
