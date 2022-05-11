#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "SBarcodeFilter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<SBarcodeFilter>("com.scythestudio.scodes", 1, 0, "SBarcodeFilter");

    qmlRegisterSingletonType(QUrl("qrc:/qml/Theme.qml"), "Theme", 1, 0, "Theme");

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/qml/ScannerPage.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
