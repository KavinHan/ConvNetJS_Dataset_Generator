#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);

	ofxXmlSettings XML;

	ofDirectory inputDir;
	ofDirectory outputDir;

	XML.load("setting.xml");

	string inputName = XML.getValue("INPUT", "input");
	string outputName = XML.getValue("OUTPUT", "output");
	string prefixName = XML.getValue("PREFIX", "dataset");

	inputDir.listDir(inputName);

	int i, j, k;
	int totalImgCount = 0;
	// load image from input folder
	for (i = 0; i < inputDir.size(); i++) {		
		ofDirectory subDir;
		subDir.listDir(inputDir.getPath(i));
		vector<ofImage> subImgs;
		for (j = 0; j < subDir.size(); j++) {
			ofImage subImg;
			subImg.load(subDir.getPath(j));
			subImg.setImageType(OF_IMAGE_GRAYSCALE);

			int w = subImg.getWidth();
			int h = subImg.getHeight();

			if (w > h) subImg.crop(0, 0, h, h); // crop h x h
			else subImg.crop(0, 0, w, w); // crop w x w

			subImg.resize(32, 32);

			subImgs.push_back(subImg);
			totalImgCount++;
		}
		inputImgs.push_back(subImgs);
	}
	
	assert(inputImgs.size() > 0 && "input folder is empty");
	assert(inputImgs[0].size() > 0 && "label image folder is empty");
	// dataset size same is sub folder image count. (one label source image count)
	// getnerate random index array
	vector<vector<int>> randIndArr; //[3][5(random)] : [dataset size][label count]
	for (i = 0; i < inputImgs[0].size(); i++) {
		randIndArr.push_back(getRandomUniqIntArr(0, inputImgs.size()));
	}

	for (i = 0; i < randIndArr.size(); i++) {
		vector<ofImage> a;
		randomImgs.push_back(a);
		for (j = 0; j < randIndArr[i].size(); j++) {
			randomImgs[i].push_back(inputImgs[ randIndArr[i][j] ][i]);
		}
	}

	
	// use random index input images, generate dataset
	// last dataset is test dataset
	// dataset = [img.width * img.height * img count];
	int datasetWidth = 32 * 32;
	unsigned char * oneDatasetPixels = new unsigned char[datasetWidth * randomImgs[0].size()];
	ofImage datasetImg;
	for (i = 0; i < randomImgs.size(); i++) {
		for (j = 0; j < randomImgs[i].size(); j++) {
			unsigned char * p = randomImgs[i][j].getPixels().getData();
			for (k = 0; k < datasetWidth; k++) {
				oneDatasetPixels[k + datasetWidth * j] = p[k];
			}
		}
		datasetImg.setFromPixels(oneDatasetPixels, 32 * 32, randomImgs[i].size(), OF_IMAGE_GRAYSCALE);
		datasetImgs.push_back(datasetImg);
		
		// !import: last dataset is test dataset
		datasetImg.save(outputName + "/" + prefixName + "_batch_" + ofToString(i) + ".png");
	}

	// use datasetImgs regenerate out randomImgs
	unsigned char * oneImgPixels = new unsigned char[datasetWidth];

	for (i = 0; i < datasetImgs.size(); i++) {
		ofImage datasetImg = datasetImgs[i];
		unsigned char * dP = datasetImg.getPixels().getData();
		for (j = 0; j < datasetImg.getHeight(); j++) {
			ofImage checkImg;
			for (k = 0; k < datasetImg.getWidth(); k++) {
				int tI = k + j * datasetImg.getWidth();
				oneImgPixels[k] = dP[tI];
			}
			checkImg.setFromPixels(oneImgPixels, 32, 32, OF_IMAGE_GRAYSCALE);
			checkImgs.push_back(checkImg);
		}
	}

	// save label js
	string labelStr = "var labels=[";
	for (int i = 0; i < randIndArr.size(); i++) {
		for (int j = 0; j < randIndArr[i].size(); j++) {
			labelStr += ofToString(randIndArr[i][j]) + ",";
		}
	}
	labelStr = labelStr.substr(0, labelStr.size() - 1);
	labelStr += "]";

	ofFile fileFinalYFile(outputName + "/" + prefixName + "_labels.js", ofFile::WriteOnly);
	fileFinalYFile << labelStr;
	fileFinalYFile.close();

}
vector<int> ofApp::getRandomUniqIntArr(int _min, int _max) {
	vector<int> arr;
	int count = _max - _min;
	for (int i = 0; i < count; i++) {
		int ranNum = ofRandom(0, count);
		bool bPass = false;
		while (!bPass) {
			bool bSame = false;
			for (int k = 0; k < arr.size(); k++) {
				if (ranNum == arr[k]) bSame = true;
			}

			if (bSame) ranNum = ofRandom(0, count);
			else bPass = true;
		}
		arr.push_back(ranNum);
	}

	return arr;
}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofPushMatrix();
	ofTranslate(20, 20);
	ofSetColor(255);
	ofDrawBitmapString("input images:", 0, 0);
	ofPushMatrix();
	ofTranslate(0, 10);
	for (int i = 0; i < inputImgs.size(); i++) {
		for (int j = 0; j < inputImgs[i].size(); j++) {
			inputImgs[i][j].draw(j * 32, i * 32);
		}
	}
	ofPopMatrix();
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(40 + inputImgs[0].size() * inputImgs[0][0].getWidth(), 20);
	ofSetColor(255);
	ofDrawBitmapString("random images:", 0, 0);
	ofPushMatrix();
	ofTranslate(0, 10);
	for (int i = 0; i < randomImgs.size(); i++) {
		for (int j = 0; j < randomImgs[i].size(); j++) {
			randomImgs[i][j].draw(j * 32, i * 32);
		}
	}
	ofPopMatrix();
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(20, 60 + inputImgs[0][0].getWidth() * inputImgs.size());
	ofSetColor(255);
	ofDrawBitmapString("dataset images:", 0, 0);
	ofPushMatrix();
	ofTranslate(0, 10);
	for (int i = 0; i < datasetImgs.size(); i++) {
		datasetImgs[i].draw(0, i * (datasetImgs[i].getHeight() + 10));
	}
	ofPopMatrix();
	ofPopMatrix();

	ofPopMatrix();
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(20, 90 + inputImgs[0][0].getWidth() * inputImgs.size() + datasetImgs.size() * (datasetImgs[0].getHeight()+10) );
	ofSetColor(255);
	ofDrawBitmapString("check images:", 0, 0);
	ofPushMatrix();
	ofTranslate(0, 10);
	for (int i = 0; i < checkImgs.size(); i++) {
		int datasetHeight = datasetImgs[0].getHeight();
		checkImgs[i].draw(i % datasetHeight * checkImgs[i].getWidth(), i / datasetHeight * checkImgs[i].getWidth());
	}
	ofPopMatrix();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
