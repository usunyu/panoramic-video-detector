//
//  main.cpp
//  PanoramicVideoDetector
//
//  Created by Yu Sun on 6/11/16.
//  Copyright © 2016 Yu Sun. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

using namespace std;
using namespace cv;

const int looseFactor = 7;
const string panoramicVideoFolder = "/Users/sun/Movies/360 Videos/";
const string panoramicVideos[] = {
    "360 Google Spotlight Story HELP.mp4",
    "360 Google Spotlight Story Special Delivery.mp4",
    "360 Stereo Music _ The Balearic Dreamer (Ambient).mp4",
    "360 Stereo Music - Trapped in Cello (instrumental).mp4",
    "360 Video - Take a ride on Elon Musk’s Hyperloop.mp4",
    "360 video VR Girl - Natasha In The Bathroom (video girl for oculus rift).mp4",
    "Assassin’s Creed Syndicate - Jack the Ripper Interactive 360° Trailer [US].mp4",
    "Clash of Clans 360 Experience a Virtual Reality Raid.mp4",
    "Experience Vivid Sydney - Week 1 (360 video).mp4",
    "FFZERO1 Concept (No Audio).mp4",
    "Foreign_Nature_by_J_Horsthuis_low-bitrate.mp4",
    "Invasion! Sneak Peek 360.mp4", // <- 3d
    "Meteor Rain in amazing sky.mp4",
    "Redfoo - Booty Man (Official 360° Music Video).mp4",
    "Take Flight 360 VR.mp4", // <- 3d
    "THE CLIMB - Alps 360° Trailer (VR Game).mp4",
    "Through Mowgli's Eyes Part II Cold Lairs - Disney's The Jungle Book.mp4",
    "Unreal Engine 4 Showdown VR 360° 4K by 3Ds Saviel.mp4",
    "WARCRAFT.mp4",
    "林俊傑 JJ Lin – 彈唱 A Song for You Till the End of Time (360 HD MV 華納Official).mp4",
};

float panoramicFactor(string path) {
    Mat frame;
    VideoCapture cap;
    
    cap.open(path);
    if(!cap.isOpened()){
        cout << "VideoCapture not open" << endl;
        cin.get();
    }
    double count = cap.get(CV_CAP_PROP_FRAME_COUNT); // get the frame count
    cap.set(CV_CAP_PROP_POS_FRAMES,count / 2); // set index to middle frame
    cap >> frame;
    
    uint8_t* pixelPtr = (uint8_t*)frame.data;
    int cn = frame.channels();
    Scalar_<uint8_t> bgrPixel;
    int equal = 0;
    for (int i = 0; i < frame.rows; i++) {
        uint8_t leftB = pixelPtr[i * frame.cols * cn + 0];
        uint8_t leftG = pixelPtr[i * frame.cols * cn + 1];
        uint8_t leftR = pixelPtr[i * frame.cols * cn + 2];
        
        uint8_t rightB = pixelPtr[i * frame.cols * cn + frame.cols * cn + 0];
        uint8_t rightG = pixelPtr[i * frame.cols * cn + frame.cols * cn + 1];
        uint8_t rightR = pixelPtr[i * frame.cols * cn + frame.cols * cn + 2];
        
        if (
            abs(leftB - rightB) <= looseFactor &&
            abs(leftG - rightG) <= looseFactor &&
            abs(leftR - rightR) <= looseFactor) {
            equal++;
        }
    }
    return (float)equal / frame.rows;
}

void showInfoForCompare(string path) {
    Mat frame;
    VideoCapture cap;
    
    cap.open(path);
    if(!cap.isOpened()){
        cout << "VideoCapture not open" << endl;
        cin.get();
    }
    double count = cap.get(CV_CAP_PROP_FRAME_COUNT); // get the frame count
    cap.set(CV_CAP_PROP_POS_FRAMES,count / 2); // set index to middle frame
    cap >> frame;
    
    uint8_t* pixelPtr = (uint8_t*)frame.data;
    int cn = frame.channels();

    for (int i = 0; i < frame.rows; i++) {
        int leftB = pixelPtr[i * frame.cols * cn + 0];
        int leftG = pixelPtr[i * frame.cols * cn + 1];
        int leftR = pixelPtr[i * frame.cols * cn + 2];
        
        int rightB = pixelPtr[i * frame.cols * cn + frame.cols * cn + 0];
        int rightG = pixelPtr[i * frame.cols * cn + frame.cols * cn + 1];
        int rightR = pixelPtr[i * frame.cols * cn + frame.cols * cn + 2];
        
        cout << "|";
        cout << "B: " << leftB << ", G: " << leftG << ", R: " << leftR;
        cout << "|";
        cout << "B: " << rightB << ", G: " << rightG << ", R: " << rightR;
        cout << "|";
        if (
            abs(leftB - rightB) <= looseFactor &&
            abs(leftG - rightG) <= looseFactor &&
            abs(leftR - rightR) <= looseFactor) {
            cout << "Yes";
        } else {
            cout << "No";
        }
        cout << endl;
    }
    // resize frame image to fit
    Size size(800, 600);
    Mat resizedFrame;
    resize(frame, resizedFrame, size);//resize image
    
    namedWindow("Display frame", WINDOW_AUTOSIZE); // Create a window for display.
    imshow("Display frame", resizedFrame); // Show our frame inside it.
    waitKey(50000);
}

int main(int argc, const char * argv[]) {
    for (int i = 0; i < ARRAY_SIZE(panoramicVideos); i++) {
        float factor = panoramicFactor(panoramicVideoFolder + panoramicVideos[i]);
        cout << "Video: " << panoramicVideos[i] << endl;
        cout << "Factor: " << factor << endl;
    }
//    showInfoForCompare(panoramicVideoFolder + panoramicVideos[3]);
}
