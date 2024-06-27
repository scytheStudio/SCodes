#ifndef COLORCONTROLLER_H
#define COLORCONTROLLER_H

#include <QObject>
#include <QColor>

class ColorController : public QObject
{
    Q_OBJECT
public:
    explicit ColorController(QObject *parent = nullptr);
    Q_INVOKABLE static QColor convertStringToColor(const QString &color);

public slots:
    bool checkColor(const QString &color);
};

#endif // COLORCONTROLLER_H
