add_executable(intrinsic_calib
  intrinsic_calib.cc
)

target_link_libraries(intrinsic_calib
  ${OpenCV_LIBS}
  camodocal_calib
  camodocal_chessboard
)

# Install the intrinsic_calib executable to the specified directory
install(TARGETS intrinsic_calib
  RUNTIME DESTINATION bin    # For executables on non-Windows systems
  ARCHIVE DESTINATION lib    # For static libraries (if any)
  LIBRARY DESTINATION lib    # For shared libraries (if any)
)

add_executable(stereo_calib
  stereo_calib.cc
)

target_link_libraries(stereo_calib
  ${OpenCV_LIBS}
  camodocal_calib
  camodocal_chessboard
)

# Install the stereo_calib executable to the specified directory
install(TARGETS stereo_calib
  RUNTIME DESTINATION bin    # For executables on non-Windows systems
  ARCHIVE DESTINATION lib    # For static libraries (if any)
  LIBRARY DESTINATION lib    # For shared libraries (if any)
)