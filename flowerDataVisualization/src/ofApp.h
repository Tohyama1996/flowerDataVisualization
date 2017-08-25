#pragma once

#include "ofMain.h"
#include "ofxCsv.h"
#include "ofxThreadedImageLoader.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    ofxCsv csv; //CSVデータ
    vector<string> timeStamp; //タイムスタンプ
    vector<float> temperature; //気温
    vector<float> humidityAir; //大気湿度
    vector<float> pressure; //気圧
    vector<float> illuminance; //照度
    vector<float> humiditySoil; //土壌湿度
    ofColor color[5]; //表示する色
    
    //描画範囲の矩形
    ofRectangle graphRect;
    float rectHeight;
    
    int current;
    int step = 3;
    float hue, saturation, brightness;
    float rectAlpha;
    
    ofPoint tem_point;
    ofPoint humAir_point;
    ofPoint pre_point;
    ofPoint ill_point;
    ofPoint humSoil_point;
    
    ofVboMesh mesh;
    
    //画像
    ofxThreadedImageLoader loader;
    vector<ofImage> images;
    int imagesTotal;
    int imageWidth = 480;
    int imageHeight = 360;
};
