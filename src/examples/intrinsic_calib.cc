#include <string>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
#include <cctype>

#include "camodocal/chessboard/Chessboard.h"
#include "camodocal/calib/CameraCalibration.h"
#include "../gpl/gpl.h"

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    cv::Size boardSize;
    float squareSize;
    std::string inputDir;
    std::string cameraModel;
    std::string cameraName;
    std::string prefix;
    std::string fileExtension;
    bool useOpenCV;
    bool viewResults;
    bool verbose;

    //========= Handling Program options =========
    const cv::String keys =
        "{help          |              | Print help message}"
        "{width w       | 9            | Number of inner corners on the chessboard pattern in x direction}"
        "{height h      | 6            | Number of inner corners on the chessboard pattern in y direction}"
        "{size s        | 120.0        | Size of one square in mm}"
        "{input i       | images       | Input directory containing chessboard images}"
        "{prefix p      | image        | Prefix of images}"
        "{file-extension e | .bmp      | File extension of images}"
        "{camera-model  | mei          | Camera model: kannala-brandt  mei  pinhole}"
        "{camera-name   | camera       | Name of camera}"
        "{opencv        | false        | Use OpenCV to detect corners (boolean flag)}"
        "{view-results  | false        | View results (boolean flag)}"
        "{verbose v     | false        | Verbose output (boolean flag)}";

    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("Intrinsic Calibration");

    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    boardSize.width = parser.get<int>("width");
    boardSize.height = parser.get<int>("height");
    squareSize = parser.get<float>("size");
    inputDir = parser.get<std::string>("input");
    prefix = parser.get<std::string>("prefix");
    fileExtension = parser.get<std::string>("file-extension");
    cameraModel = parser.get<std::string>("camera-model");
    cameraName = parser.get<std::string>("camera-name");
    useOpenCV = parser.get<bool>("opencv");
    viewResults = parser.get<bool>("view-results");
    verbose = parser.get<bool>("verbose");

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    if (!fs::exists(inputDir) && !fs::is_directory(inputDir))
    {
        std::cerr << "# ERROR: Cannot find input directory " << inputDir << "." << std::endl;
        return 1;
    }

    camodocal::Camera::ModelType modelType;
    if (cameraModel.compare("kannala-brandt") == 0)
    {
        modelType = camodocal::Camera::KANNALA_BRANDT;
    }
    else if (cameraModel.compare("mei") == 0)
    {
        modelType = camodocal::Camera::MEI;
    }
    else if (cameraModel.compare("pinhole") == 0)
    {
        modelType = camodocal::Camera::PINHOLE;
    }
    else
    {
        std::cerr << "# ERROR: Unknown camera model: " << cameraModel << std::endl;
        return 1;
    }

    switch (modelType)
    {
    case camodocal::Camera::KANNALA_BRANDT:
        std::cout << "# INFO: Camera model: Kannala-Brandt" << std::endl;
        break;
    case camodocal::Camera::MEI:
        std::cout << "# INFO: Camera model: Mei" << std::endl;
        break;
    case camodocal::Camera::PINHOLE:
        std::cout << "# INFO: Camera model: Pinhole" << std::endl;
        break;
    }

    // look for images in input directory
    std::vector<std::string> imageFilenames;
    for (const auto& entry : fs::directory_iterator(inputDir))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }

        std::string filename = entry.path().filename().string();

        // check if prefix matches
        if (!prefix.empty())
        {
            if (filename.compare(0, prefix.length(), prefix) != 0)
            {
                continue;
            }
        }

        // check if file extension matches
        if (filename.compare(filename.length() - fileExtension.length(), fileExtension.length(), fileExtension) != 0)
        {
            continue;
        }

        imageFilenames.push_back(entry.path().string());

        if (verbose)
        {
            std::cerr << "# INFO: Adding " << imageFilenames.back() << std::endl;
        }
    }

    if (imageFilenames.empty())
    {
        std::cerr << "# ERROR: No chessboard images found." << std::endl;
        return 1;
    }

    if (verbose)
    {
        std::cerr << "# INFO: # images: " << imageFilenames.size() << std::endl;
    }

    cv::Mat image = cv::imread(imageFilenames.front(), -1);
    const cv::Size frameSize = image.size();

    camodocal::CameraCalibration calibration(modelType, cameraName, frameSize, boardSize, squareSize);
    calibration.setVerbose(verbose);

    std::vector<bool> chessboardFound(imageFilenames.size(), false);
    for (size_t i = 0; i < imageFilenames.size(); ++i)
    {
        image = cv::imread(imageFilenames.at(i), -1);

        camodocal::Chessboard chessboard(boardSize, image);

        chessboard.findCorners(useOpenCV);
        if (chessboard.cornersFound())
        {
            if (verbose)
            {
                std::cerr << "# INFO: Detected chessboard in image " << i + 1 << std::endl;
            }

            calibration.addChessboardData(chessboard.getCorners());

            cv::Mat sketch;
            chessboard.getSketch().copyTo(sketch);

            cv::imshow("Image", sketch);
            cv::waitKey(50);
            cv::destroyWindow("Image");
        }
        else if (verbose)
        {
            std::cerr << "# INFO: Did not detect chessboard in image " << i + 1 << std::endl;
        }
        chessboardFound.at(i) = chessboard.cornersFound();
    }    

    if (calibration.sampleCount() < 10)
    {
        std::cerr << "# ERROR: Insufficient number of detected chessboards." << std::endl;
        return 1;
    }

    if (verbose)
    {
        std::cerr << "# INFO: Calibrating..." << std::endl;
    }

    double startTime = camodocal::timeInSeconds();

    calibration.calibrate();
    calibration.writeParams(cameraName + "_camera_calib.yaml");
    calibration.writeChessboardData(cameraName + "_chessboard_data.dat");

    if (verbose)
    {
        std::cout << "# INFO: Calibration took a total time of "
                  << std::fixed << std::setprecision(3) << camodocal::timeInSeconds() - startTime
                  << " sec.\n";
    }

    if (verbose)
    {
        std::cerr << "# INFO: Wrote calibration file to " << cameraName + "_camera_calib.yaml" << std::endl;
    }

    if (viewResults)
    {
        std::vector<cv::Mat> cbImages;
        std::vector<std::string> cbImageFilenames;

        for (size_t i = 0; i < imageFilenames.size(); ++i)
        {
            if (!chessboardFound.at(i))
            {
                continue;
            }

            cbImages.push_back(cv::imread(imageFilenames.at(i), -1));
            cbImageFilenames.push_back(imageFilenames.at(i));
        }

        // visualize observed and reprojected points
        calibration.drawResults(cbImages);

        for (size_t i = 0; i < cbImages.size(); ++i)
        {
            cv::putText(cbImages.at(i), cbImageFilenames.at(i), cv::Point(10,20),
                        cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 255, 255),
                        1, cv::LINE_AA);
            cv::imshow("Image", cbImages.at(i));
            cv::waitKey(0);
        }
    }

    return 0;
}
