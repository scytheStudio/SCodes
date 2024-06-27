#include "ColorController.h"

ColorController::ColorController(QObject *parent)
    : QObject{parent}
{}

bool ColorController::checkColor(QString color)
{
    return QColor(color).isValid();
}

QColor ColorController::convertStringToColor(QString color)
{
    return QColor(color);
}
