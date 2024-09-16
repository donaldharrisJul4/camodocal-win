# CamOdoCal-Win

## Background

This project references [CamOdoCal](https://github.com/hengli/camodocal).

I am looking for a suitable alternative to OpenCV wide-angle camera calibration. OpenCV's [Fisheye camera model](https://docs.opencv.org/4.x/db/d58/group__calib3d__fisheye.html)  and OpenCV's [Omnidirectional Camera model](https://docs.opencv.org/4.x/dd/d12/tutorial_omnidir_calib_main.html) both target wide-angle or even fisheye cameras, but their calibration accuracy and stability are not satisfactory. Therefore, I found the CamOdoCal project. 

In this project, the Kannala-Brandt model corresponds to OpenCV's Fisheye camera model, and the Mei model corresponds to OpenCV's Omnidirectional Camera model, with parameters that can be matched one-to-one.

However, compiling CamOdoCal on Windows (Visual Studio) is quite challenging, so I ported and adapted the project.

Since I am only concerned with monocular and stereo calibration, I kept only the codes related to intrinsic_calib and stereo_calib from the examples, making it easier to port to Windows.

## Introduction

This C++ library supports the following tasks:

1. Intrinsic calibration of a generic camera.
2. Stereo calibration of two generic cameras.

The intrinsic calibration process and stereo calibration process use one of the following three camera models:

* Pinhole camera model
* Unified projection model (C. Mei, and P. Rives, Single View Point Omnidirectional Camera Calibration from Planar Grids, ICRA 2007)
* Equidistant fish-eye model (J. Kannala, and S. Brandt, A Generic Camera Model and Calibration Method for Conventional, Wide-Angle, and Fish-Eye Lenses, PAMI 2006)

By default, the unified projection model is used since this model approximates a wide range of cameras from normal cameras to catadioptric cameras. Note that in our equidistant fish-eye model, we use 8 parameters: k2, k3, k4, k5, mu, mv, u0, v0. k1 is set to 1.

The workings of the library are described in the three papers:

        Lionel Heng, Bo Li, and Marc Pollefeys,
        CamOdoCal: Automatic Intrinsic and Extrinsic Calibration of a Rig with Multiple Generic Cameras and Odometry,
        In Proc. IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS), 2013.
    
        Lionel Heng, Mathias Bürki, Gim Hee Lee, Paul Furgale, Roland Siegwart, and Marc Pollefeys,
        Infrastructure-Based Calibration of a Multi-Camera Rig,
        In Proc. IEEE International Conference on Robotics and Automation (ICRA), 2014.
        
        Lionel Heng, Paul Furgale, and Marc Pollefeys,
        Leveraging Image-based Localization for Infrastructure-based Calibration of a Multi-camera Rig,
        Journal of Field Robotics (JFR), 2015.

If you use this library in an academic publication, please cite at least one of the following papers depending on what you use the library for.

#### Acknowledgements ####

The primary author, Lionel Heng, is funded by the DSO Postgraduate Scholarship. This work is supported in part by the European Community's Seventh Framework Programme (FP7/2007-2013) under grant #269916 (V-Charge).

The CamOdoCal-Win library includes third-party code from the following sources:

        1. M. Rufli, D. Scaramuzza, and R. Siegwart,
           Automatic Detection of Checkerboards on Blurred and Distorted Images,
           In Proc. IEEE/RSJ International Conference on Intelligent Robots and Systems, 2008.
    
        2. Sameer Agarwal, Keir Mierle, and Others,
           Ceres Solver.
           https://code.google.com/p/ceres-solver/
        
        3. L. Kneip, D. Scaramuzza, and R. Siegwart,
           A Novel Parametrization of the Perspective-Three-Point Problem for a
           Direct Computation of Absolute Camera Position and Orientation,
           In Proc. IEEE Conference on Computer Vision and Pattern Recognition, 2011.

## Build Instructions for Windows

- Download [OpenCV 4.10.0 Windows version](https://github.com/opencv/opencv/releases/download/4.10.0/opencv-4.10.0-windows.exe) and install it. Then, add "abs_path_to_opencv4100/build/x64/vc16/bin" to the environment variables.

- Download [CMake](https://cmake.org/download/) and add "abs_path_to_cmake-3.30.3-windows-x86_64\bin" to the environment variables.
- Install the latest Visual Studio 2022 Community Edition.

The following required dependencies should be downloaded into the 3rdparty folder and unzipped:

- [Eigen 3.4.0](https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip)
- [glog 0.3.5](https://github.com/google/glog/archive/refs/tags/v0.3.5.zip)
- [gflags 2.2.2](https://github.com/gflags/gflags/archive/refs/tags/v2.2.2.zip)
- [Ceres Solver 2.1.0](https://github.com/ceres-solver/ceres-solver/archive/refs/tags/2.1.0.zip)

Then, the directory structure under the 3rdparty folder should look like this. Note that the folders xxx_build and xxx_install are pre-created for the subsequent compilation steps.

```
├─ceres-solver-2.1.0
├─ceres_build
├─ceres_install
├─eigen-3.4.0
├─eigen_build
├─eigen_install
├─gflags-2.2.2
├─gflags_build
├─gflags_install
├─glog-0.3.5
├─glog_build
└─glog_install
```

1. Check if CMake is in the environment variables and if the version is correct by using the following command: 

   ```powershell
   cmake --version
   ```

2. Build eigen.

   ```powershell
   cd eigen_build
   
   cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=11  -DCMAKE_CXX_STANDARD_REQUIRED=ON -DBUILD_TESTING=OFF -DEIGEN_COMPILER_SUPPORT_CPP11=ON  ../eigen-3.4.0 -DCMAKE_INSTALL_PREFIX="abs_path_to_3rdparty/eigen_install"
   
   cmake --build . --config Release
   
   cmake --install . --config Release
   ```

3. Build gflags.

   ```powershell
   cd gflags_build
   
   cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=11  -DCMAKE_CXX_STANDARD_REQUIRED=ON ../gflags-2.2.2 -DCMAKE_INSTALL_PREFIX="abs_path_to_3rdparty/gflags_install"
   
   cmake --build . --config Release
   
   cmake --install . --config Release
   ```

4. Build glog.

   ```powershell
   cd  glog_build
   
   cmake.exe -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=11  -DCMAKE_CXX_STANDARD_REQUIRED=ON ../glog-0.3.5 -DCMAKE_INSTALL_PREFIX="abs_path_to_3rdparty/glog_install"
   
   cmake --build . --config Release
   
   cmake --install . --config Release
   ```

5. Build ceres-solver.

   ```powershell
   cd ceres_build
   
   cmake.exe -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=11  -DCMAKE_CXX_STANDARD_REQUIRED=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_BENCHMARKS=OFF -DEigen3_DIR="../eigen_install/share/eigen3/cmake" -Dgflags_DIR="../gflags_install/lib/cmake/gflags" -Dglog_DIR="../glog_install/lib/cmake/glog" ../ceres-solver-2.1.0 -DCMAKE_INSTALL_PREFIX="abspath_to_3rdparty/ceres_install"
   
   cmake --build . --config Release
   
   cmake --install . --config Release
   ```

6. Go to camodocal-win's root folder and build.

   ```powershell
   cd camodocal_build
   
   cmake.exe -G "Visual Studio 17 2022" -A x64 -DEigen3_DIR="abs_path_to3rdparty/eigen_install/share/eigen3/cmake" -Dgflags_DIR="abs_path_to3rdparty/gflags_install/lib/cmake/gflags" -Dglog_DIR="abs_path_to3rdparty/glog_install/lib/cmake/glog" -DCeres_DIR="abs_path_to3rdparty/ceres_install/lib/cmake/Ceres" -DOpenCV_DIR="abs_path_to_opencv4100/build" ../ -DCMAKE_INSTALL_PREFIX="abspath_to_camodocal/camodocal_install"
   
   cmake --build . --config Release
   
   cmake --install . --config Release
   ```

   

## Examples

Go to the camodocal_install/bin folder where the executables corresponding to the examples are located in. To see all allowed options for each executable, use the --help option which shows a description of all available options.

1. Intrinsic calibration ([src/examples/intrinsic_calib.cc])

   ```powershell
   intrinsic_calib -i=abs_path_to_images_folder -p=img -e=.bmp -w=9 -h=6 -s=120 --camera-model=kannala-brandt --opencv=true --view-results=true -v=true
   ```

   The camera-model parameter takes one of the following three values: pinhole, mei, and kannala-brandt.

2. Stereo calibration  ([src/examples/stereo_calib.cc])

   ```powershell
   stereo_calib -i=abs_path_to_images_folder --prefix-l=left --prefix-r=right -e=.bmp -w=9 -h=6 -s=120 --camera-model=kannala-brandt --opencv=true --view-results=true -v=true
   ```

   The camera-model parameter takes one of the following three values: pinhole, mei, and kannala-brandt.

## Changes

- Remove the Scaramuzza model.
- Replace Boost (shared_ptr/string/filesystem) with std.
- Replace Boost (boost/numeric/ublas) with Eigen.
- Adapt cv functions to OpenCV 4.x.x.
- Replace `clock_gettime` with `std::chrono`.
- Use OpenCV's `cv::CommandLineParser` to replace `boost::program_options` for handling command-line arguments.
- Use `cv::cornerSubPix` to refine the corners detected by `cv::findChessboardCorners` to sub-pixel accuracy.



