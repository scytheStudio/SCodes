#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "SBarcodeDecoder.h"
#include "TestClass.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    qmlRegisterType<TestClass>("com.scythestudio.scodes", 1, 0, "TestClass");

    QQmlApplicationEngine engine;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    engine.load(QUrl(QStringLiteral("qrc:/qml/qt5main.qml")));
#else
    engine.load(QUrl(QStringLiteral("qrc:/qml/qt6main.qml")));
#endif

    if (engine.rootObjects().isEmpty()) { return -1; }

    return app.exec();
}
