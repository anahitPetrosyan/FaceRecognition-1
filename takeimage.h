#ifndef TAKEIMAGE_H
#define TAKEIMAGE_H

#pragma once
#include <vector>
#include <iostream>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <dlib/dnn.h>
#include <dlib/clustering.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <dlib/string.h>
#include <experimental/filesystem>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;
using namespace dlib;

class TakeImage{
public:
    std::vector<string> takeImageFromVideo(std::string folderpath,std::string name)
    {
        VideoCapture cap(0);
        Mat crop;
        string path;
        Mat resized;
        std::vector<string> imagePath;
        if(!cap.isOpened())
        {
            std::cout << "Unable to connect to camera" << std::endl;
        }
        frontal_face_detector detect  = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("C:\\Users\\Gevorg\\source\\repos\\Test\\Test\\shape_predictor_5_face_landmarks.dat") >> pose_model;
        int i = 0;
        while(i < 10)
        {

            Mat image;
            cap >> image;
            cv_image<bgr_pixel> cimg(image);
            std::vector<dlib::rectangle> faces;
            std::vector<cv::Rect> opencvRects;
            int k = faces.size();
            faces = detect(cimg);
            dlib_rect2cv_Rect(opencvRects,faces);
            for(int j = 0; j < opencvRects.size(); j++)
            {
                if(opencvRects[j].x >= 0 && opencvRects[j].y >= 0 && opencvRects[j].width + opencvRects[j].x < image.cols && opencvRects[j].height + opencvRects[j].y < image.rows)
                {
                    crop = image(opencvRects[j]);
                    path = folderpath+"/" +name+"_" +std::to_string(i + 1)+".jpg";
                    cv::resize(crop,resized,Size(150,150));
                    imwrite(path,resized);
                    imagePath.push_back(path);
                }
            }
            namedWindow("win",WINDOW_AUTOSIZE);
            imshow("win",image);
            waitKey(1);
            if(faces.size() > k)
                i++;
        }
        return imagePath;
    }
private:
    void dlib_rect2cv_Rect(std::vector<Rect>& cv_R,std::vector<dlib::rectangle>& r)
    {
        cv_R.clear();
        for(unsigned long int i = 0; i < r.size();i++ )
        {
            cv_R.push_back(Rect(r[i].left(),r[i].top(),r[i].width(),r[i].height()));
        }
    }
};
#endif // TAKEIMAGE_H
