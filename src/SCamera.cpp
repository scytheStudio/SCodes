#include <QCameraInfo>
#include <QCameraFocus>
#include <QCameraViewfinderSettings>

#include "SCamera.h"

SCamera::SCamera()
{
//    setCamera(QCameraInfo::defaultCamera());
    this->load();
    QCameraViewfinderSettings viewfinderSettings;
    viewfinderSettings.setResolution(640, 480);
    viewfinderSettings.setMinimumFrameRate(15.0);
    viewfinderSettings.setMaximumFrameRate(30.0);
    this->setViewfinderSettings(viewfinderSettings);

    QCameraFocus *cameraFocus = this->focus();
    cameraFocus->setFocusMode(QCameraFocus::ContinuousFocus);
    cameraFocus->setFocusPointMode(QCameraFocus::FocusPointAuto);

    this->captureMode().setFlag(QCamera::CaptureVideo);
    this->start();
}
