#include <faceRecognition.h>
#include <database.h>
#include <QApplication>
#include <vector>
#include <iostream>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime>
#include <dlib/dnn.h>
#include <dlib/clustering.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <dlib/string.h>
#include <opencv2/opencv.hpp>
#include <experimental/filesystem>
#include <time.h>
#include "C:/Users/Gevorg/Downloads/json.h"
#include <fstream>
#include <set>
#include <string>
namespace filesys = std::experimental::filesystem;
using json = nlohmann::json;
using namespace dlib;
using namespace cv;
using namespace std;
template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;
template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;
template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;
template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;
template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;
using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
    alevel0<
    alevel1<
    alevel2<
    alevel3<
    alevel4<
    max_pool<3, 3, 2, 2, relu<affine<con<32, 7, 7, 2, 2,
    input_rgb_image_sized<150>
    >>>>>>>>>>>>;





std::vector<float> string_double(string n) {
    std::vector<float> st;
    string t;
    int i = 0;
    for (auto k : n) {
        if (k != '[' && k != ']' && k != ',' && k != ' ') {
            t += k;
        }
        else if (k == ',' || k == ']') {
            st.resize(st.size() + 1);
            st[i] = stof(t);
            i++;
            t = "";
        }
    }
    return st;
}



std::vector<float>  dlib_matrix_to_vector(matrix<float, 0, 1> desc_matrix);




string get_file_name(std::string filePath);
std::vector<string> filenames;
std::vector<string> names_of_data;
std::set<string> names_of_data_set;
std::map<std::string, std::vector<std::vector<float>>> map_files;
std::vector<string> json_of_data;
bool tf = 1;


string read_name_from_data(string data) {
    string p;
    for (auto i : data){
        if (i == ' ')break;
        else p += i;
    }
    return p;
}



std::vector<string> Data_for_pearson(string name_from_json) {
    string path = "C:\\Users\\Gevorg\\source\\repos\\Test\\Test\\TestSet1";
    ifstream myfile(path + "//" + "TestSet1_Data.json");
    string k;
    std::vector<string> data;
    string p;
    while (std::getline(myfile,k)){
        if (name_from_json == read_name_from_data(k)){
            for (auto o : k)
            {
                if (o == ' ')
                {
                    data.push_back(p);
                    p = "";
                }
                else p += o;
            }
            data.push_back(p);
            return data;
        }
    }
    return { "unknown" };
}




class FaceDescriptionExtractor{
public:
    void load(string path) {
        string temp_name;
        ifstream myfile(path + "//" + "TestSet1_face_descriptiors.json");
        int i = 0,j = 0;
        filenames.resize(i + 1);
        while (std::getline(myfile, filenames[i]))
        {
            temp_name = "";
            //cout << filenames[i] << endl;
            for (auto d : filenames[i])
            {
                if (d == ' ')tf = 0;
                if (tf) {
                    names_of_data.resize(j + 1);
                    names_of_data[j] += d;
                    temp_name += d;
                }
                else {
                    json_of_data.resize(j + 1);
                    json_of_data[json_of_data.size() - 1] += d;
                }
            }
            if(temp_name != "")names_of_data_set.insert(temp_name);
            if (!tf)j++;
            tf = 1;
            i++;
            filenames.resize(i + 1);
        }

        for (auto it = names_of_data_set.begin(); it != names_of_data_set.end();++it)
            map_files.insert(pair<string, std::vector< std::vector<float> > >(*it, std::vector< std::vector<float> >()));
        for (int i = 0; i < json_of_data.size(); i++)
        {
            map_files[names_of_data[i]].push_back(string_double(json_of_data[i]));
        }

    }
    void store(string path) {
        json descriptors_json;
        std::vector<cv::Mat> image;
        std::vector<cv::String> imageNames;
        ofstream t(path + "//TestSet1_face_descriptiors.json");
        dlib::cv_image<bgr_pixel> cimg;
        frontal_face_detector detect = get_frontal_face_detector();
        dlib::shape_predictor pose_model;
        deserialize("C:\\Users\\Gevorg\\source\\repos\\Test\\Test\\shape_predictor_5_face_landmarks.dat") >> pose_model;
        anet_type net;
        deserialize("C:\\Users\\Gevorg\\Downloads\\dlib_face_recognition_resnet_model_v1.dat") >> net;

        glob(path + "//*.jpg", imageNames);

        for (int i = 0; i < imageNames.size(); i++) {
            image.push_back(imread(imageNames[i], 1));
        }
        std::vector<matrix<rgb_pixel>> Faces;
        for (int i = 0; i < image.size(); i++)
        {
            cimg = image[i];
            Faces.clear();
            for (auto face : detect(cimg))
            {
                auto shape = pose_model(cimg, face);
                matrix<rgb_pixel> face_chip;
                extract_image_chip(cimg, get_face_chip_details(shape, 150, 0.25), face_chip);
                Faces.push_back(std::move(face_chip));

            }
            // 128D points face descriptors
            std::vector<matrix<float, 0, 1>> face_descriptors = net(Faces);
            cout << "desc" << endl;
            for (int j = 0; j < face_descriptors.size(); j++)
            {
                descriptors_json = face_descriptors[j];
                t << get_file_name(imageNames[i]) << " " << descriptors_json;
            }
            t << "\n";
        }
    }

};










std::vector<float> dlib_matrix_to_vector(matrix<float, 0, 1> desc_matrix) {
    std::vector<float> desc_vec;
    std::vector<float> tem;

    for (auto i : desc_matrix)
    {
        desc_vec.push_back((double)i);
    }

    return desc_vec;

}





float euqlidesis_distance(std::vector<float>  vec1, std::vector<float> vec2) {

    float sums = 0;
    assert(vec1.size() == vec2.size());
    for (int i = 0; i < vec1.size(); i++)
    {
        sums += ((vec1[i] - vec2[i]) * (vec1[i] - vec2[i]));
    }

    return sqrt(sums);
}




string name;
string recognizier(std::map<string,std::vector<std::vector<float>>> my_map, std::vector<float> & vec) {
    name = "unknown";
    int i = 0;
    float dist = 0;

    float min_dist = 0.55;

    for (auto p : names_of_data_set){
        i = 0;
        for (auto k : my_map[p]) {
            dist = euqlidesis_distance(k, vec);

            if(dist < 0.45){
                cout << "dist == " << dist << endl;
                return p;
            }
            else if (dist < min_dist){
                i = 0;
                min_dist = dist;
                name = p;
            }
            else if (i <= my_map[p].size()) {i++;}
            else{break;}
        }
    }
    return name;
}




string get_file_name(std::string filePath) {
    filesys::path pathObj(filePath);
    string OnlyName;
    if (pathObj.has_stem())
    {
        for (auto k : pathObj.stem().string()) {
            if (k == '_')break;
            OnlyName += k;
        }
        return OnlyName;
    }
    return "";
}






void dlib_rect2cv_Rect(std::vector<Rect>& cv_R, std::vector<dlib::rectangle>& r)
{
    for (unsigned long int i = 0; i < r.size();i++)
    {
        cv_R.push_back(Rect(r[i].left(), r[i].top(), r[i].width(), r[i].height()));
    }
}



void dlib_lmarkPoint2cv_Point(std::vector<full_object_detection>& shapes, std::vector<std::vector<Point>>& lmarks)
{
    for (unsigned long int i = 0; i < shapes.size(); i++) {
        std::vector<Point> currVec;
        for (unsigned long int j = 0; j < shapes.at(i).num_parts(); j++)
        {
            dlib::point dlibP = shapes.at(i).part(j);
            cv::Point p(dlibP.x(), dlibP.y());
            currVec.push_back(p);
        }
        lmarks.push_back(currVec);
    }
}

time_t  cur_time = time(0);
void videoFaceDetect()
{
    json descriptors;
    VideoCapture cap(0);
    //int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    //int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);

    //VideoWriter video("outcpp.mp4", VideoWriter::fourcc('M', 'S', 'V', 'C'), 10, Size(frame_width, frame_height));
    if (!cap.isOpened())
    {
        std::cout << "Unable to connect to camera" << std::endl;
    }
    image_window win;
    frontal_face_detector detect = get_frontal_face_detector();
    shape_predictor pose_model;
    deserialize("C:\\Users\\Gevorg\\source\\repos\\Test\\Test\\shape_predictor_5_face_landmarks.dat") >> pose_model;
    anet_type net;
    deserialize("C:\\Users\\Gevorg\\Downloads\\dlib_face_recognition_resnet_model_v1.dat") >> net;
    int face_size = 1;
    json JsonFaces;
    while (!win.is_closed())
    {

        Mat image;
        cap >> image;
        cv_image<bgr_pixel> cimg(image);
        std::vector<matrix<rgb_pixel>> Faces;
        std::vector<dlib::rectangle> faces;


        for (auto face : detect(cimg)){
            auto shape = pose_model(cimg, face);
            faces = detect(cimg);
            matrix<rgb_pixel> face_chip;
            extract_image_chip(cimg, get_face_chip_details(shape, 150, 0.25), face_chip);
            Faces.push_back(std::move(face_chip));
        }


        std::vector<cv::Rect> opencvRects;
        dlib_rect2cv_Rect(opencvRects, faces);
        for (int i = 0; i < faces.size(); i++){
            cv::rectangle(image, opencvRects[i], Scalar(255, 0, 0), 1, 8, 0);
        }
        std::vector<matrix<float, 0, 1>> face_descriptors = net(Faces);
        for (int i = 0; i < Faces.size(); i++) {
            std::vector<float> vec = dlib_matrix_to_vector(face_descriptors[i]);
            int begin = cv::getTickCount();
            name = recognizier(map_files, vec);
            int end = cv::getTickCount();
            std::cout << end - begin << endl;
            std::vector<string> data = Data_for_pearson(name);
            /*for (int i = 0; i < 4; i++)
            {
            }*/
            Scalar color_of_text = Scalar(0, 0, 255);
            if (data.size() > 1)
            {
                cv::putText(image, data[0], cv::Point(opencvRects[i].x, opencvRects[i].y - 45), cv::FONT_HERSHEY_SIMPLEX, 0.5, color_of_text, 1);
                cv::putText(image, data[1], cv::Point(opencvRects[i].x, opencvRects[i].y - 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, color_of_text, 1);
                cv::putText(image, data[2], cv::Point(opencvRects[i].x, opencvRects[i].y - 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, color_of_text, 1);
                cv::putText(image, data[3], cv::Point(opencvRects[i].x, opencvRects[i].y), cv::FONT_HERSHEY_SIMPLEX, 0.5, color_of_text, 1);
            }
            else cv::putText(image, "unknown", cv::Point(opencvRects[i].x, opencvRects[i].y), cv::FONT_HERSHEY_SIMPLEX, 0.5, color_of_text, 1);

        }

        /*		for (int i = 0; i < opencvRects.size(); i++)                  ////////frawing circles in landmark points
                {
                    for (unsigned long int j = 0; j < opencvFaceLandmarks.at(i).size(); j++)
                    {
                        cv::circle(image, opencvFaceLandmarks[i][j], 1, Scalar(255, 0, 0), FILLED, 16);
                    }
                }*/


        namedWindow("win", WINDOW_AUTOSIZE);
        cv::imshow("win", image);
        cv::waitKey(1);
    }
}

void imageFaceDetect() {

    json JsonFaces;
    Mat crop;
    std::string path;
    time_t  cur_time = time(0);
    frontal_face_detector detect = get_frontal_face_detector();			//for detecting rect of faces
    shape_predictor pose_model;											// for 68 landmarks

    std::vector<cv::Mat> image;
    std::vector<cv::String> imageNames;

    deserialize("C:\\Users\\Gevorg\\Downloads\\shape_predictor_68_face_landmarks.dat") >> pose_model;
    glob("C:/Users/Gevorg/source/repos/Test/Test/Face_detection_test.jpg", imageNames);			//take all image names in variable imageNames

    for (int i = 0; i < imageNames.size(); i++) {
        image.push_back(imread(imageNames[i], 1));							//all images from flie in image variable
    }

    cv_image<bgr_pixel> cimg;
    std::vector<dlib::rectangle> faces;
    std::vector<full_object_detection> shapes;
    std::vector<std::vector<Point>> opencvFaceLandmarks;
    for (int im = 0; im < image.size(); im++) {

        cimg = image[im];
        faces = detect(cimg);													//detect face rectangles
        for (int l = 0; l < faces.size(); l++)
        {
            shapes.push_back(pose_model(cimg, faces[l]));
            dlib_lmarkPoint2cv_Point(shapes, opencvFaceLandmarks);

            std::string filename = "C:\\Users\\Gevorg\\source\\repos\\Test\\Test\\jsons\\" + get_file_name(imageNames.at(im)) + "__face_" + std::to_string(l + 1) + ".json";
            std::ofstream jsonLmarks(filename);
            JsonFaces = { "id",cur_time };
            jsonLmarks << JsonFaces << "\n";


            for (int j = 0; j < opencvFaceLandmarks[l].size();j++)
            {
                JsonFaces = { opencvFaceLandmarks[l][j].x ,opencvFaceLandmarks[l][j].y };
                jsonLmarks << JsonFaces << "\n";
            }
            shapes.clear();
        }
        std::vector<cv::Rect> opencvRects;
        dlib_rect2cv_Rect(opencvRects, faces);
        for (int j = 0; j < opencvRects.size(); j++)
        {


            if (opencvRects[j].x >= 0 && opencvRects[j].y >= 0 && opencvRects[j].width + opencvRects[j].x < image[im].cols && opencvRects[j].height + opencvRects[j].y < image[im].rows)crop = image[im](opencvRects[j]);
            else crop = image[im];
            path = "C:\\Users\\Gevorg\\source\\repos\\Test\\Test\\jsons\\" + get_file_name(imageNames.at(im)) + "__face_" + std::to_string(j + 1) + ".jpg";
            imwrite(path, crop);
        }
    }
}


int main(int argc,char* argv[])try
{
//    string path = "C:\\Users\\Gevorg\\source\\repos\\Test\\Test\\TestSet1";
//    FaceDescriptionExtractor f;
//    f.load(path);
//    videoFaceDetect();
    QApplication a(argc,argv);
    faceRecognition w;
    w.show();
    return a.exec();
//    DataBase db;
//    std::vector<PersonInfo> p;
//    p = db.SelectPersonInfo();
//    for (auto i : p){
//        cout << i.name << " " << i.lastName << " " << i.officeName << " "  << i.position<< " "  << endl;
//    }
}

catch (const std::exception& ex)
{
    std::cout << ex.what();
}
