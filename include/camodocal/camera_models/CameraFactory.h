#ifndef CAMERAFACTORY_H
#define CAMERAFACTORY_H

#include <memory>
#include <opencv2/core/core.hpp>

#include "camodocal/camera_models/Camera.h"

namespace camodocal
{

class CameraFactory
{
public:
    CameraFactory();

    static std::shared_ptr<CameraFactory> instance(void);

    CameraPtr generateCamera(Camera::ModelType modelType,
                             const std::string& cameraName,
                             cv::Size imageSize) const;

    CameraPtr generateCameraFromYamlFile(const std::string& filename);

private:
    static std::shared_ptr<CameraFactory> m_instance;
};

}

#endif
