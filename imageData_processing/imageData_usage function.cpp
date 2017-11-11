#include<stdio.h>  
#include<conio.h>
#include<iostream>  
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include<opencv2/imgproc/imgproc.hpp>  

using namespace std;
using namespace cv;

void f_point(BYTE *data, int x, int y, int c, int w, int l);         //obtain single image point data
void f_row(BYTE *data, int x, int start, int end, int c,int l);     //obtain single image row data
void f_col(BYTE *data, int y, int start, int end, int c, int l);    //obtain single image col data
void f_rect(BYTE *data, int x, int y, int x_range, int y_range, int c, int w, int l);    //obtain image ROI data
void draw_data(BYTE *data, int w, int l, int c);          //draw image ROI

void main(int argc, char** argv)
{
	//////////////////////////use OpenCV to obtain basic information from raw image
	string input_file_path = "C:\\Users\\JQHS1001B\\Desktop\\2.jpg";
	Mat src = imread(input_file_path);       //read pic
	/*int channels, width, length;*/
	int channels = src.channels();             //image channels
	int width = src.rows;                      //image width
	int length = src.cols;                    //image length
	if (width == 0 || length == 0 || channels == 0) {                            //check non-data image transfer
		cout << "there is no information about this pic" << endl;
	}
	BYTE* iPtr = new BYTE[length*width*channels];                       //allocate iPtr with certain space
	for (int i = 0; i<width; i++)                                   
	{
		for (int j = 0; j<length; j++)
		{
			for (int k = 0; k<channels; k++)
			{
				iPtr[i*length*channels + j*channels + k] = src.at<Vec3b>(i, j)[k];        //transfer image RGB value to iPtr pointer
			}
	     }
	}

	/////////////test draw_data function
	/*draw_data(iPtr, width, length, 3);*/
	/*namedWindow("test");*/


	/*test f_point function*/
	//namedWindow("test");
	//imshow("test", src);
	//f_point(iPtr, 100, 40, 3, width, length);

	/*test f_rect function*/
	/*f_rect(iPtr, 100, 40, 1000, 1000, 3, width, length);*/

	/*test f_row function*/
	 //namedWindow("test");
	 //imshow("test", src);
	 //f_row(iPtr, 100, 100,300, 3, length);

	/*test f_row function*/
	//namedWindow("test");
	//imshow("test", src);
	//f_col(iPtr, 100, 0,width, 3, width);


	waitKey(0);
}


//////////////////obtain single image point data *data:input image data    x:input x coordinate  y:input y coordinate  c:channels w:image width l:image length
void f_point(BYTE *data, int x, int y, int c,int w,int l)   
{
	BYTE* point = new BYTE[c];           //allocate space for a certain point
	if (x >= 0 && y >= 0 && c == 1)      // for single channel image, transfer certain image point to a pointer
	{
		point[0] = data[x*l + y];
		cout << point[0] << endl;
	}else if(x >= 0 && y >= 0 && c == 3)       //for multi-channel image, transfer certain image point to a pointer
		{
			point[0] = data[x*l * 3 + y];
			point[1] = data[x*l * 3 + y + 1];
			point[2] = data[x*l * 3 + y + 2];
			char temp0[10], temp1[10], temp2[10];
			sprintf_s(temp0, "%d", point[0]);
			sprintf_s(temp1, "%d", point[1]);
			sprintf_s(temp2, "%d", point[2]);
			/*sprintf_s(temp, "%d%d%d", point[0], point[1], point[2]);*/
			cout <<temp0<<" "<<temp1<<" "<<temp2<< endl;
		}
}


//////////////////obtain image ROI data *data:input image data   x:input x coordinate  y:input y coordinate  x_range: rectangle length y_range:rectangle width c:channels w:image width l:image length
void f_rect(BYTE *data, int x, int y, int x_range, int y_range, int c, int w, int l)
{
	BYTE *rect = new BYTE[x_range*y_range*c];    //allocate space for ROI
	if((x<0)&&(y<0)&&(x_range<0)&&(y_range<0)&&(x_range>l)&&(y_range>w))  //check input argument available
	{
		cout << "the arguments that you input is not correct" << endl;
	}
	else if (((x + x_range) > l) || ((y + y_range) > w)){           //check input range
	cout << "ROI size is out of image range" << endl;
		}
	else{                                                   //obtain certain rectangle data from input image
		for (int i = y; i < y + y_range; i++)
		{
			for (int j = x; j < x + x_range; j++)
			{
				for (int k = 0; k < c; k++)
				{
					rect[(i - y)*x_range*c + (j - x)*c + k] = data[i*l * c + j * c + k];
				}
			}
		}
	}
	/*namedWindow("test1");*/
	//waitKey(0);
	draw_data(rect, y_range, x_range, 3);
	delete rect;                          //delete used data
}

//////////////////obtain single image row data   *data:input image data  x: desired rows location  start: start point  end: end point cchannels  l:image length
void f_row(BYTE *data, int x,int start,int end, int c, int l)
{
	BYTE *row = new BYTE[(end-start+1)*c];             //allocate space for rows
	if ((x<0)&&(start<0)&&(start>l)&&(end<0)&&(end>l)&&(x>l))            //check
	{
		cout << "the arguments that you input is not correct" << endl;
	}else {
		for (int i = start; i <=end; i++)                                          //x rows
		{
				for (int k = 0; k < c; k++)
				{
					row[(i-start)*c + k] = data[x*l * c + i * c + k];             //obtain image pointer data for each point
					char temp[10];
					sprintf_s(temp, "%d", row[(i - start)*c + k]);              //transfer uchar data to char
					cout << temp << " ";                                        //output row data RGB value
				}
				cout << endl;
		}
	}
}

//////////////////obtain single image col data    *data:input image data  y: desired cols location  start:start point  end: end point  c:channels  w:image width
void f_col(BYTE *data, int y, int start, int end, int c, int w)
{
	BYTE *col = new BYTE[(end - start + 1)*c];                             //allocate space for cols
	if ((y<0) && (start<0) && (start>w) && (end<0) && (end>w)&&(y>w))              //check
	{
		cout << "the arguments that you input is not correct" << endl;
	}
	else {
		for (int i = start; i <= end; i++)                                     //y cols
		{
			for (int k = 0; k < c; k++)
			{
				col[(i - start)*c + k] = data[y*w * c + i * c + k];                 //obtain image pointer data for each point 
				char temp[10];
				sprintf_s(temp, "%d", col[(i - start)*c + k]);              //transfer uchar data to char
				cout << temp << " ";                                        //output col data RGB value
			}
			cout << endl;
		}
	}
}



//////////////////draw image ROI *data: input image data    w:image width  l:image length  c:channels
void draw_data(BYTE *data,int w,int l,int c)
{
	if (c == 1) {                           //for single channel image
		Mat test(w, l, CV_8UC1);
		for (int i = 0; i<w; i++)
		{
			for (int j = 0; j<l; j++)
			{
				for (int k = 0; k<c; k++)
				{
					test.at<Vec3b>(i, j)[k] = data[i*l*c + j*c + k];   //transfer image data from pointer to class Mat
				}
			}
		}
		namedWindow("test");
		imshow("test", test);
	}
	else {                                      //for multichannel image
		Mat test(w, l, CV_8UC3);
		for (int i = 0; i<w; i++)
		{
			for (int j = 0; j<l; j++)
			{
				for (int k = 0; k<c; k++)
				{
					test.at<Vec3b>(i, j)[k] = data[i*l*c + j*c + k];    // //transfer image data from pointer to class Mat
				}
			}
		}
		namedWindow("test");
		imshow("test", test);
	}
}