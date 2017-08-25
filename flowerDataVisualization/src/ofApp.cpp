#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetFullscreen(true);
    
    //色を初期化
    color[0] = ofColor(141,211,199);
    color[1] = ofColor(255,255,179);
    color[2] = ofColor(190,186,218);
    color[3] = ofColor(251,128,114);
    color[4] = ofColor(128,177,211);
    
    //CSVファイル読み込み
    if(csv.load("sensor_edi.csv")) {
        cout << "data loaded" << endl;
    } else {
        cout << "load error!" << endl;
    }
    
    //データ配列に読み込み
    for (int i = 0; i < csv.size(); i++) {
        timeStamp.push_back(csv[i].getString(0));
        temperature.push_back(csv[i].getFloat(1));
        humidityAir.push_back(csv[i].getFloat(2));
        pressure.push_back(csv[i].getFloat(3));
        illuminance.push_back(csv[i].getFloat(4));
        humiditySoil.push_back(csv[i].getFloat(5));
    }
    
    //vbomeshを初期化
    mesh.setMode(OF_PRIMITIVE_LINES);
    
    //グラフ範囲を初期化
    rectHeight = ofGetHeight() - 200;
    graphRect = ofRectangle(0, (ofGetHeight()-rectHeight)/2, ofGetWidth(), rectHeight);
    
    //画像読み込み
    ofDirectory dir(ofToDataPath("./"));
    imagesTotal = dir.listDir();
    images.resize(imagesTotal);
    for(int i = 0; i < imagesTotal; i++) {
        loader.loadFromDisk(images[i], dir.getName(i));
        images[i].resize(imageWidth, imageHeight);
    }
    cout << imagesTotal << " images loaded" << endl;
}

void ofApp::update(){
    
    //最大値、最小値
    float max, min, x, y;
    //ピックアップするデータ(列)
    current = ofGetFrameNum() % temperature.size();
    
    
    //気温
    max = *max_element(temperature.begin(), temperature.end());
    min = *min_element(temperature.begin(), temperature.end());
    
    x = ofGetWidth()/2+current*step;
    y = ofMap(temperature[current], min, max, graphRect.y + graphRect.height, graphRect.y);
    mesh.addVertex(ofPoint(x, y));
    mesh.addColor(color[0]);
    
    x += step;
    y = ofMap(temperature[current+1], min, max, graphRect.y + graphRect.height, graphRect.y);
    mesh.addVertex(ofPoint(x, y));
    mesh.addColor(color[0]);
    
    tem_point += (ofPoint(x,y)-tem_point)/8;

    //max = 28;
    //min = 22;
    hue = ofMap(temperature[current], min, max, 80, 0);
    saturation = ofMap(temperature[current], min, max, 0, 100);
    
    
    //大気湿度
    max = *max_element(humidityAir.begin(), humidityAir.end());
    min = *min_element(humidityAir.begin(), humidityAir.end());
    
    x = ofGetWidth()/2+current*step;
    y = ofMap(humidityAir[current], min, max, graphRect.y + graphRect.height, graphRect.y);
    mesh.addVertex(ofPoint(x, y));
    mesh.addColor(color[1]);
    
    x += step;
    y = ofMap(humidityAir[current+1], min, max, graphRect.y + graphRect.height, graphRect.y);
    mesh.addVertex(ofPoint(x, y));
    mesh.addColor(color[1]);
    
    humAir_point += (ofPoint(x,y)-humAir_point)/8;
    
    rectAlpha = ofMap(humidityAir[current], min, max, 0, 80);
    
    
    //気圧
    max = *max_element(pressure.begin(), pressure.end());
    min = *min_element(pressure.begin(), pressure.end());
    
    x = ofGetWidth()/2+current*step;
    y = ofMap(pressure[current], min, max, graphRect.y + graphRect.height, graphRect.y);
    mesh.addVertex(ofPoint(x, y));
    mesh.addColor(color[2]);
    
    x += step;
    y = ofMap(pressure[current+1], min, max, graphRect.y + graphRect.height, graphRect.y);
    mesh.addVertex(ofPoint(x, y));
    mesh.addColor(color[2]);
    
    pre_point += (ofPoint(x,y)-pre_point)/8;
    
    rectHeight = ofMap(pressure[current], min, max, ofGetHeight()-50, ofGetHeight()-200);

    
    
    //照度
    max = *max_element(illuminance.begin(), illuminance.end());
    min = *min_element(illuminance.begin(), illuminance.end());
    
    x = ofGetWidth()/2+current*step;
    y = ofMap(illuminance[current], min, max, graphRect.y, graphRect.y + graphRect.height);
    mesh.addVertex(ofPoint(x, y));
    mesh.addColor(color[3]);
    
    x += step;
    y = ofMap(illuminance[current+1], min, max, graphRect.y, graphRect.y + graphRect.height);
    mesh.addVertex(ofPoint(x, y));
    mesh.addColor(color[3]);
    
    ill_point += (ofPoint(x,y)-ill_point)/8;
    
    brightness = ofMap(illuminance[current], min, max, 255, 100);
    
    
    //土壌湿度
    max = *max_element(humiditySoil.begin(), humiditySoil.end());
    min = *min_element(humiditySoil.begin(), humiditySoil.end());
    
    x = ofGetWidth()/2+current*step;
    y = ofMap(humiditySoil[current], min, max, graphRect.y, graphRect.y + graphRect.height);
    mesh.addVertex(ofPoint(x, y));
    mesh.addColor(color[4]);
    
    x += step;
    y = ofMap(humiditySoil[current+1], min, max, graphRect.y, graphRect.y + graphRect.height);
    mesh.addVertex(ofPoint(x, y));
    mesh.addColor(color[4]);
    
    humSoil_point += (ofPoint(x,y)-humSoil_point)/8;
    
    
    if (current == 0) mesh.clear();
}

void ofApp::draw(){
    //バックグラウンド
    ofBackground(ofColor::fromHsb(hue, saturation, brightness));
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    //矩形を描画
    ofRectangle drawnRect(0, (ofGetHeight()-rectHeight)/2, ofGetWidth(), rectHeight);
    ofSetColor(255,rectAlpha);
    ofDrawRectangle(drawnRect);
    
    //グラフと文字を描画
    ofPushMatrix();
    ofTranslate(-current*step, 0);
    
    mesh.draw();
    
    ofSetColor(255);
    ofDrawBitmapString("temperature " + ofToString(temperature[current]) + "˚C", tem_point);
    ofDrawBitmapString("humidity_air " + ofToString(humidityAir[current]) + "%", humAir_point);
    ofDrawBitmapString("pressure " + ofToString(pressure[current]) + "hPa", pre_point);
    ofDrawBitmapString("illuminance " + ofToString(illuminance[current]), ill_point);
    ofDrawBitmapString("humidity_soil " + ofToString(illuminance[current]) + "%", humSoil_point);

    ofPopMatrix();
    
    ofDisableBlendMode();
    if (current < imagesTotal) images[current].draw(ofGetWidth()*4/5-imageWidth/2, ofGetHeight()*2/3-imageHeight/2, imageWidth, imageHeight);
    
    //日付表示
    ofSetColor(255);
    ofDrawBitmapStringHighlight(timeStamp[current], 20, 30);
}
