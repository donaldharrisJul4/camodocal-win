include_directories(
  ${CERES_INCLUDE_DIRECTORIES}
)

add_library(camodocal_camera_models STATIC
  Camera.cc
  CameraFactory.cc
  CataCamera.cc
  CostFunctionFactory.cc
  EquidistantCamera.cc
  PinholeCamera.cc
)

target_link_libraries(camodocal_camera_models
  Eigen3::Eigen
  ${OpenCV_LIBS}
  camodocal_gpl
  ${CERES_LIBRARIES}
  ${GLOG_LIBRARIES}
)
