![SCodes ](/assets/scodes.png)
# SCodes

This project is Qt & Qml wrapper for [ZXing-C++ Library](https://github.com/nu-book/zxing-cpp) that is used for decoding and generating 1D and 2D barcodes. This particular C++ ZXing port is one of the most recent C++ versions of popular ZXing library using modern compilers.

Thanks to SCodes you can start scanning barcodes in few steps. We used Qt 5.15.2 and Qt 6.3.0 to built wrapper and example applications, but it's compatible with older Qt versions as well.

Implementation of same method for both Qt version does not seems possible([check why](#porting)). We have ported the SCodes wrapper to Qt6 by following the multimedia [changes](https://doc-snapshots.qt.io/qt6-6.3/qtmultimedia-changes-qt6.html).

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

Feel free to read ["How to scan barcodes in Qt Qml application" blog post](https://scythe-studio.com/blog/how-to-scan-barcodes-in-qt-qml-application) if you are interested in the details behind scanning mechanism. Also you can try out the QmlReaderExample in order to see how it is working.

SCodes supports generating barcodes as well. It is covered in ["How to generate barcode in Qt Qml" application blog post](https://scythe-studio.com/en/blog/how-to-generate-barcode-in-qt-qml-application).  Also you can try out the QmlGeneratorExample in order to see how it is working.

Above blog posts contains step by step tutorial on how to do that for Qt5 version. For Qt6 ([see here](#porting)).

### qmake
All you need to do is to follow these steps.

1. Add SCodes as submodule, by typing `git submodule add git@github.com:scytheStudio/SCodes.git`
2. Update submodule `git submodule update --recursive --init` (you can also put wrapper files to your project manually without adding submodule)
3. Add `include(scodes/src/SCodes.pri)` to your .pro file
4. If you want to use barcode reader functionality you need to register `SBarcodeFilter` class for Qt5 or `SBarcodeScanner` class for Qt6. For both version, separate them with if directive to register as we did in barcode reader example([how to register reader class](#register-reader)). As for barcode generator functionality you just need to register `SBarcodeGenerator` class([how to register generator class](#register-generator)).
5. Import SCodes in your Qml file `import com.scythestudio.scodes 1.0`
6. Import multimedia module `import QtMultimedia 5.15` for Qt5 or `import QtMultimedia` for Qt6.
7. If build fails, try to add `CONFIG += c++17` to your .pro file
8. You are done. Get inspired by [Qt5 QML Barcode Reader demo](https://github.com/scytheStudio/SCodes/blob/master/examples/QmlBarcodeReader/qml/Qt5ScannerPage.qml) or [Qt6 QML Barcode Reader demo](https://github.com/scytheStudio/SCodes/blob/master/examples/QmlBarcodeReader/qml/Qt6ScannerPage.qml) to test wrapper.

### CMake

1. Add SCodes as submodule, by typing `git submodule add git@gitlab.com:scythestudio/scodes.git`
2. Update submodule `git submodule update --recursive --init` (you can also put wrapper files to your project manually without adding submodule)
3. Add to your project SCodes library

    ```CMake
        add_subdirectory(SCodes)
    ```

4. Link SCodes library to your library or executable. 

    ```CMake 
        target_link_libraries(${PROJECT_NAME} PUBLIC SCodes)
    ```

5. Import SCodes in your Qml file 

    ```qml
        import com.scythestudio.scodes 1.0
    ```

6. You are done. Get inspired by [QML Barcode Reader demo](https://github.com/scytheStudio/SCodes/blob/master/examples/QmlBarcodeReader/qml/ScannerPage.qml) to test wrapper.

<a name="register-reader"></a>
### How to do
Registering the barcode reader classes with if directive:

```c++
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        qmlRegisterType<SBarcodeFilter>("com.scythestudio.scodes", 1, 0, "SBarcodeScanner");
    #else
        qmlRegisterType<SBarcodeScanner>("com.scythestudio.scodes", 1, 0, "SBarcodeScanner");
    #endif
```
<a name="register-generator"></a>
Registering the barcode generator class with associated enum:
```c++
    qmlRegisterType<SBarcodeGenerator>("com.scythestudio.scodes", 1, 0, "SBarcodeGenerator");
    qmlRegisterUncreatableMetaObject(SCodes::staticMetaObject, "com.scythestudio.scodes", 1, 0, "SCodes", "Error: only enums");
```

<a name="porting"></a>
### Implementation details in Qt6

Qt's multimedia library has major changes. The most importants are, changes in QML `VideoOutput`, absence of `QVideoFilterRunnable` and `QAbstractVideoFilter` classes in Qt6.

SCodes library is using `SBarcodeFilter` class for Qt5 and `SBarcodesScanner` class for Qt6 version. 

If you want to read more about implementation details of the library in Qt6 read the document: [Implementation Details in Qt6](https://github.com/scytheStudio/SCodes/blob/master/doc/detailsQt6.md)


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


### The examples tested on below kits:

#### Qt5.15.2 or less,

| PROJECT | BUILD SYSTEM | WINDOWS-MinGW | WINDOWS-MSVC | LINUX-GCC | ANDROID |
| ------ | ------ | ------ | ------ | ------ | ------ |
| QmlBarcodeReader | qmake |<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|
| QmlBarcodeGenerator | qmake |<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|
| QmlBarcodeReader | CMake |<center>❌</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|
| QmlBarcodeGenerator | CMake |<center>❌</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|

#### Qt6.3.0,

| PROJECT | BUILD SYSTEM | WINDOWS-MinGW | WINDOWS-MSVC | LINUX-GCC | ANDROID |
| ------ | ------ | ------ | ------ | ------ | ------ |
| QmlBarcodeReader | qmake |<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|
| QmlBarcodeGenerator | qmake |<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|
| QmlBarcodeReader | CMake |<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|
| QmlBarcodeGenerator | CMake |<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|<center>✔️</center>|

Please ensure that proper Java & NDK version installed on your system. This examples tested w/ Java 11 and 22.1.7171670 Android NDK version.

## About Scythe Studio
We are a group of Qt and C++ enthusiasts whose goal is to address growing demand for cross-platform Qt development services. Thanks to our expertise in **Qt Qml development**, quality of deliveries and proven track of projects developed for companies from various industries we have been awarded the title of an official **Qt Service Partner**.

<span> 
<a href="https://scythe-studio.com"><img width="340" height="150" src="https://user-images.githubusercontent.com/45963332/221174257-c1e1a9d9-0efa-4b25-996b-4b364ccb325c.svg"></a>
<a href="https://clutch.co/profile/scythe-studio"><img height="150" width="150" src="https://user-images.githubusercontent.com/45963332/221174280-99b32a1d-7418-4a49-bcea-6927639cf557.png"></a>
</span>

The company offers broad spectrum of services for the clients who wish to bring their ideas to life. We have extensive and practical knowledge about various Qt modules and other technologies allowing to create high quality product in a cost effective approach. If you want to see what Scythe Studio is is capable of and what services we provide, check out [this link](https://scythe-studio.com/en/services).

## Professional Support
We offer professional support for those who need assistance with integrating SCodes library or require additional features. For more information on our support options and pricing, please contact us at sales@scythe-studio.com

## Follow us

Check out those links if you want to see Scythe Studio in action and follow the newest trends saying about Qt Qml development.

* 🌐 [Scythe Studio Website](https://scythe-studio.com/en/)
* ✍️  [Scythe Studio Blog Website](https://scythe-studio.com/en/blog)
* 👔 [Scythe Studio LinkedIn Profile](https://www.linkedin.com/company/scythestudio/mycompany/)
* 👔 [Scythe Studio Facebook Page](https://www.facebook.com/ScytheStudiio)
* 🎥 [Scythe Studio Youtube Channel](https://www.youtube.com/channel/UCf4OHosddUYcfmLuGU9e-SQ/featured)
