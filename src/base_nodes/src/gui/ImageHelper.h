#pragma once 

#include "GUI.h"

#include <opencv2/opencv.hpp>

class ImageHelper {
    cv::Mat image;
    GLuint texture;
public:
    //image must be in RGBA
    ImageHelper(cv::Mat image);
    // ImageHelper();
    
    void imguiDrawImage();
};