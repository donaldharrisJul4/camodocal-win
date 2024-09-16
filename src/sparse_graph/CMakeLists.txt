add_library(camodocal_sparse_graph STATIC
  Transform.cc
)

target_link_libraries(camodocal_sparse_graph
  Eigen3::Eigen
  ${OpenCV_LIBS}
)
