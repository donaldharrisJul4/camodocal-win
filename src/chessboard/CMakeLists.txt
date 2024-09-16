add_library(camodocal_chessboard STATIC
  Chessboard.cc
)

target_link_libraries(camodocal_chessboard
  Eigen3::Eigen
  ${OpenCV_LIBS}
)
