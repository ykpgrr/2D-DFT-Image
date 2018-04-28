//
//  main.cpp
//  Discreate Fourier Transform
//  Created by Yakup Gorur on 4/3/18.
//  Copyright © 2018 Yakup Gorur. All rights reserved.
//  Digital Signal Processing Design and Application 2017-2018 Spring
//  Homework 6
//  Lecturer: Prof. Dr. Bilge Gunsel, Research Assistant Yağmur Sabucu

// !!! Please ensure that the image folder path is true.


#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>

using namespace std; //namespace for  C++ standard library
using namespace cv;  //namespace for  OpenCV library

//**************************-User Defined Settings-****************************
//image folder path
string image_folder_path = "/Users/yakup/Software Developer/DigSignalProcess/HW6/output/";

//To make debug: It shows all outputs in the operations
bool debug = false
;

//***********************************--End--***********************************

//writing the pixels of Image in txt file.
void Writepixels1channel(const Mat& input, string filename);
void SaveImageMagnitudePhase(const Mat input, string filename);


int main(int argc, const char * argv[]) {
    
    
    //Reading the "img.jpg" image file into "img" Mat Object
    Mat img = imread(image_folder_path + "img.jpg", IMREAD_GRAYSCALE);
    
    
    if (img.empty()){ //Check the Is the file opened or not?
        std::cerr<<"can't open image: <img.jpg>"<<std::endl;
        return - 1;
    }
    
    
    if(debug){ //Debug: Show the img image file
        imshow("""img.jpg"" image file", img);
        waitKey();
    }
    
    //*********************************----a----********************************
    //Convert img to float type.
    Mat img_float;
    img.convertTo(img_float, CV_32FC1);
    
    //Fourier Transform
    Mat FourierTransform;
    dft(img_float, FourierTransform);
    
    //Extract information of the "magnitude" and "phase"
    Mat planes[] = {Mat::zeros(img_float.size(), CV_32FC1), Mat::zeros(img_float.size(), CV_32FC1)};
    split(FourierTransform, planes);// planes[0] = Re(DFT(I)) & planes[1] = Im(DFT(I))
    Mat magnitude, phase;
    cartToPolar(planes[0], planes[1], magnitude, phase);
    
    //Write the Pixel values of Magnitude and Phase Matrix
    Writepixels1channel(magnitude, "genlik");
    Writepixels1channel(phase, "faz");
    
    //*********************************----b----********************************
    SaveImageMagnitudePhase(magnitude.clone(),"genlik");
    SaveImageMagnitudePhase(phase.clone(),"faz");
    
    //*********************************----c----********************************
    //Inverse Fourier Transform from "magnitude" and "phase" of Image
    polarToCart(magnitude, phase, planes[0], planes[1]);
    Mat FourierTransform2;
    merge(planes, 1, FourierTransform2);
    Mat inverseDFT;
    dft(FourierTransform2, inverseDFT, DFT_INVERSE);
    normalize(inverseDFT, inverseDFT, 0, 255, CV_MINMAX);
    Mat new_img;
    inverseDFT.convertTo(new_img, CV_8U);
    
    //Save image
    imwrite(image_folder_path + "InverseTransform.jpg", new_img);
    //Debug image
    if(debug){
        imshow("InverseTransform.jpg", new_img);
        waitKey();
    }
    
    //*********************************----d----********************************
    //Difference between original Image and Inverse DFT Image
    Mat diffImage;
    absdiff(img, new_img, diffImage);

    //Save the differences as Image.
    imwrite(image_folder_path + "fark.jpg", diffImage);
    
    
    if(debug){
        imshow("diff", diffImage);
        waitKey();
    }

    //To look at the maximum difference between images
    /*
    double min,max;
    minMaxLoc(diffImage, &min, &max);
    for(int i = 0; i <diffImage.cols; i++ ){
        for (int j= 0; j <diffImage.rows; j++) {
            cout<<(int)diffImage.at<uchar>(j, i)<<endl;
        }
    }
    
    cout<<"max:"<<max<<endl;
     */
    
    //*********************************----e----********************************
    //Convert difimg to float type.
    Mat diff_float;
    diffImage.convertTo(diff_float, CV_32FC1);
    
    //Fourier Transform
    Mat FT_diff;
    dft(diff_float, FT_diff);
    
    //Calculate Power
    Mat diff_power;
    pow(FT_diff, 2, diff_power);
    diff_power = cv::abs(diff_power);
    SaveImageMagnitudePhase(diff_power, "power");
    //Write the power as txt
    Writepixels1channel(diff_power, "power");
    
    //*********************************----f----********************************
    
    double powerFFT =0, powerTime=0;
    for(int i = 0; i <diffImage.rows; i++ ){
        for (int j= 0; j <diffImage.cols; j++) {
            powerFFT += (double) diff_power.at<float>(i,j);
            powerTime += pow((double)diffImage.at<uchar>(i,j),2);
        }
    }
    powerFFT /= (diff_power.cols * diff_power.rows);
    powerTime /= (diff_power.cols * diff_power.rows);
    
    cout<<endl<<"power FFT: "<<powerFFT<<endl<<"power time: "<<powerTime<<endl;
    
    return 0;
}

void Writepixels1channel(const Mat& input, string filename){
    
    // Open the file in write mode.
    ofstream outputFile;
    outputFile.open(image_folder_path+filename+".txt");
    
    for(int i = 0; i <input.rows; i++ ){
        for (int j= 0; j <input.cols; j++) {
            outputFile <<"ROW"<<i << " COL"<<j<< ": "<< (float)input.at<uchar>(i, j) << '\t';
        }
        outputFile << endl;
    }
    
    outputFile.close();
    return;
}

void SaveImageMagnitudePhase(const Mat input, string filename){
    
    //Save Image
    imwrite(image_folder_path + filename + ".jpg", input);
    
    if(debug){
        imshow(filename, input);
        waitKey();
    }
    
    //Logaritmic Scale to visualize
    Mat log_scale_input(input);
    log_scale_input += Scalar::all(1);
    log(log_scale_input, log_scale_input);
    normalize(log_scale_input, log_scale_input, 0, 255, CV_MINMAX);
    
    //Save Logaritmic Scale image
    imwrite(image_folder_path + filename + "_LogScale.jpg", log_scale_input);
    
    
    if(debug){
        imshow(filename + "_LogScale.jpg", log_scale_input);
        waitKey();
    }
    
}
