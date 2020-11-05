#ifndef SCAMERA_H
#define SCAMERA_H

#include <QCamera>
#include <qqml.h>

class SCamera : public QCamera
{
    Q_OBJECT
    QML_ELEMENT
public:
    SCamera();
};

#endif // SCAMERA_H
