include_directories(
  ${CERES_INCLUDE_DIRECTORIES}
)

add_library(camodocal_gpl STATIC 
CubicSpline.cc
EigenQuaternionParameterization.cc
gpl.cc
)

target_link_libraries(camodocal_gpl
  Eigen3::Eigen
  ${OpenCV_LIBS}
  ${CERES_LIBRARIES}
)
