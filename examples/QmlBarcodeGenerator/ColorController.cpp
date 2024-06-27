#include "ColorController.h"

ColorController::ColorController(QObject *parent)
    : QObject{parent}
{}

bool ColorController::checkColor(const QString &color)
{
    return QColor(color).isValid();
}

QColor ColorController::convertStringToColor(const QString &color)
{
    return QColor(color);
}
