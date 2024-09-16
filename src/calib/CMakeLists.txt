include_directories(
  ${CERES_INCLUDE_DIRECTORIES}
)

set(SRCS
    CameraCalibration.cc
    StereoCameraCalibration.cc
)

set(CALIB_OPTIONAL_DEPENDENCIES
    ${OpenCV_LIBS}
    camodocal_camera_models
    camodocal_gpl
    camodocal_sparse_graph
)


add_library(camodocal_calib STATIC ${SRCS})

target_link_libraries(camodocal_calib
  Eigen3::Eigen
  ${CALIB_OPTIONAL_DEPENDENCIES}
  ${CERES_LIBRARIES}
)


