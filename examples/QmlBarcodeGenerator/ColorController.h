#ifndef COLORCONTROLLER_H
#define COLORCONTROLLER_H

#include <QObject>
#include <QColor>

class ColorController : public QObject
{
    Q_OBJECT
public:
    explicit ColorController(QObject *parent = nullptr);

public slots:
    bool checkColor(QString color);
    QColor convertStringToColor(QString color);
};

#endif // COLORCONTROLLER_H
