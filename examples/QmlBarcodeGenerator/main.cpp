#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "SBarcodeGenerator.h"
#include "ColorController.h"

int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType(QUrl("qrc:/qml/Theme.qml"), "Theme", 1, 0, "Theme");

    qmlRegisterType<SBarcodeGenerator>("com.scythestudio.scodes", 1, 0, "SBarcodeGenerator");
    qmlRegisterType<ColorController>("com.scythestudio.scodes", 1, 0, "ColorController");

    engine.load(QUrl(QStringLiteral("qrc:/qml/GeneratorPage.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
