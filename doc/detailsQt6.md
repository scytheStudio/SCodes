# Implementation details in Qt 6
VideoOutput QML element has major changes and it is not possible to use same QML files for both version. If you want to implement barcode reader functionality for both version you need to create two separate QML file too(that's because of QML VideoOutput changes in multimedia module of Qt6). Check our [QmlBarcodeReader example](https://github.com/scytheStudio/SCodes/tree/master/examples/QmlBarcodeReader) for more details. 

`SBarcodeFilter.cpp` and `SBarcodesScanner.cpp` files included/excluded according to the Qt version in the [SCodes CMakeLists file](https://github.com/scytheStudio/SCodes/blob/master/src/CMakeLists.txt). The idea of excluding the related class according to Qt version is prevent to get error from not existing libraries when you compile the project for Qt6(e.g. QVideoFilterRunnable, QAbstractVideoFilter). Likewise in Qt5 for QVideoSink.

SBarcodeScanner class is inherited from QVideoSink class. It's initiated in 2 phases: First, the constructor sets up the connections to processing threads. Then, after QmlEngine sets up properties, `forwardVideoSink` and `camera`, a `componentComplete()` function is called that instantiates a default `QCamera` if none is set.

This allows to set camera object in QML:
```qml
SBarcodeScanner {
    ...
    camera: Camera {
        ...
    }
}

```

Also, in the constructor, `Worker` object that dependent on scanner variable passed to workerThread instance. So, we can access to resources of the scanner object.
```c++
    worker->moveToThread(&workerThread); 
```
`VideoOutput` has a `forwardVideoSink` property in Qt6 which allows to pass the videoframes further to a `VideoOutput` sink or some other process. 

To handle the frames `QVideoSink::videoFrameChanged` signal connected to `SBarcodeScanner::handleFrameCaptured` slot.
As soon as new frame has came to the handle function;
```c++
    emit process(QImage &img);
```
signal emitted and `imageProcess` function started to run in worker thread.
Also, frame passed to m_videoSink in order to show the frames on the screen.
```c++
    m_videoSink->setVideoFrame(frame);
```
