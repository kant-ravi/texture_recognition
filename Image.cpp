/*

 * Image.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: ravi
 */

#include "Image.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>
using namespace std;

Image::Image(int h,int w,int c,char *location) {

	FILE *file;
	if(!(file=fopen(location,"rb"))) {
			cout << "Cannot open file: "<<location<<endl;
			exit(1);
		}
	if(h>0 && w>0 && c>0)
	{
		height  = h;
		width   = w;
		channel = c;
		data    = new float[h*w*c];
		padImageLimit=-1;
	}
	else
	{
		cout<<"Invalid arguments, cannot allocate memory";
		exit(1);
	}
	unsigned int row,col,curC;
	unsigned char *value;
	for(row = 0; row < height; row++){
		for(col = 0; col < width; col++) {
			for(curC = 0; curC < c; curC++) {

				fread(value, sizeof(unsigned char), 1,file);
				data[(width*channel*row)+(channel*col)+c-1]=float(*value);
			}
		}
	}
	//fread(data, sizeof(unsigned char), h*w*c,file);
	fclose(file);

}
Image::Image(int h,int w,int c) {

	height  = h;
	width   = w;
	channel = c;
	data    = new float[h*w*c];
	padImageLimit=-1;
}

Image::Image() {

}
int Image::rows() {
	return height;
}

int Image::cols() {
	return width;
}

int Image::channels() {
	return channel;
}

void Image::setPadImageLimit(int limit) {
	padImageLimit = limit;
}

float Image::getPixel(int row,int col, int c) {

	int wd=width;
	int ht=height;
	if(row < 0 || col < 0 || row >= ht || col >= wd){
		if(padImageLimit==-1){
			cout<<"\nTrying to access pixel that is outside image data bounds";
			cout<<"\nPlease set image pad limit if you want to enable mirroring";
			exit(1);
		}
	}

	if(row < 0 && abs(row)<=padImageLimit) {
		row = abs(row);
	}

	if(col < 0 && abs(col)<=padImageLimit) {
		col = abs(col);
	}

	if(row >= ht && (row-ht)<padImageLimit) {
		row = 2 * (ht - 1) - row;
	}
	if(col >= wd && (col-wd)<padImageLimit) {
			col = 2 * (wd - 1) - col;
	}
	return(data[(width*channel*row)+(channel*col)+c]);
}

void Image::setPixel(int row,int col, int c = 0,float value = 0.0) {
	data[(width*channel*row)+(channel*col)+c] = value;
}

int Image::size() {
	return height*width;
}

Image Image::clone(Image orignalImg){
	Image newImage;
	newImage.height=orignalImg.height;
	newImage.width=orignalImg.width;
	newImage.channel=orignalImg.channel;
	newImage.data = new float[orignalImg.height * orignalImg.width * orignalImg.channel];
	newImage.padImageLimit=-1;
	return newImage;
}

Image::~Image() {
	// TODO Auto-generated destructor stub
}

