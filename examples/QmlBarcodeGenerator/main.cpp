#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "SBarcodeGenerator.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // If you use Qt version older than 5.15 you need to manually register SBarcodeGenerator
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
        qmlRegisterType<SBarcodeGenerator>("com.scythestudio.scodes", 1, 0, "SBarcodeGenerator");
#endif

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/GeneratorPage.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
