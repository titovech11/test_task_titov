#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>

#include "dav_class.h"
using namespace cv;

DAV test("Test.dav");
std::string path = "json/Test_";
std::vector<int> jsonCadrNumbers{ 2, 3, 4, 5, 6, 21, 46, 47, 48 };

Mat3b canvas;
Rect button1;

std::vector<Rect> rectFromJson(const std::string& filePath) {
	FileStorage fs(filePath, FileStorage::READ);
	Point first{0};
	Point second{0};
	std::vector<Rect> allRects{0};
	FileNode root = fs["shapes"];

	for (int i = 0; i < root.size(); ++i)
	{
		FileNode allPoints = fs["shapes"][i]["points"][0];
		first.x = allPoints[0].real();
		first.y = allPoints[1].real();

		allPoints = fs["shapes"][i]["points"][1];
		second.x = allPoints[0].real();
		second.y = allPoints[1].real();

		Rect signRect(first, second);
		allRects.push_back(signRect);
	}

	return allRects;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		size_t end_pos = start_pos + from.length();
		str.replace(start_pos, end_pos, to);
		start_pos += to.length();
	}
}

void showDav(DAV& davVideo, std::vector<int> jsonCadrNumbers, std::string pathToJson)
{
	std::vector<Rect> signRects;
	std::string search_string = "null";
	std::string replace_string = "0,";
	for (int i = 1; i <= davVideo.CountKadr; ++i)
	{
		Mat currCadr = davVideo.cadr(i);
		for (auto j = jsonCadrNumbers.cbegin(); j != jsonCadrNumbers.cend(); ++j)
		{
			if (*j == i)
			{
				pathToJson.insert(10, std::to_string(*j));
				pathToJson += ".json";
				std::string inbuf;
				std::fstream input_file(pathToJson, std::ios::in);
				pathToJson.erase(10);
				pathToJson.insert(10, std::to_string(*j));
				pathToJson += "_NoNull.json";
				std::ofstream output_file(pathToJson);
				while (!input_file.eof())
				{
					getline(input_file, inbuf);
					size_t foundpos = inbuf.find(search_string);
					if (foundpos != std::string::npos)
						replaceAll(inbuf, search_string, replace_string);

					output_file << inbuf << std::endl;
				}
				std::vector<Rect> signRect = rectFromJson(pathToJson);;
				pathToJson.erase(10);

				for (auto k = signRect.cbegin(); k != signRect.cend(); ++k)
				{
					rectangle(currCadr, *k, Scalar(0, 255, 0), 1);
				}
			}
		}

		namedWindow("in", 0);
		imshow("in", currCadr);
		if (cv::waitKey(33) >= 0) break;
	}
}

void saveResJson(std::string path, const std::vector<Point> points)
{
	std::string final_path = "json/answerCadr_" + path;
	FileStorage fs(final_path, FileStorage::WRITE);
	fs << "label" << "";
	fs << "points" << "[";
	for (int i = 0; i < points.size(); ++i)
	{
		fs << "[" << points[i].x;
		fs << points[i].y << "]";
	}
	fs << "]";
	fs << "group_id" << "null";
	fs << "shape_type" << "polygon";
	fs << "flags" << "{";
	fs << "tool" << "ZnakGenerator";
	fs << "NN type" << "UNet";
	fs << "classifier" << "road sign";
	fs << "object type" << 0;
	fs.release();
}

void callBackFuncButton1(int event, int x, int y, int flags, void* userdata)
{
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	std::string search_string = "null";
	std::string replace_string = "0,";
	if (event == EVENT_LBUTTONDOWN)
	{
		if (button1.contains(Point(x, y)))
		{
			std::cout << "Clicked!" << std::endl;
			std::vector<Mat> croppedSigns;
			std::vector<Mat> grayCroppedSigns;
			int l = 0;
			for (int i = 1; i <= test.CountKadr; ++i)
			{
				Mat currCadr = test.cadr(i);
				for (auto j = jsonCadrNumbers.cbegin(); j != jsonCadrNumbers.cend(); ++j)
				{
					if (*j == i)
					{
						path.insert(10, std::to_string(*j));
						path += ".json";
						std::string inbuf;
						std::fstream input_file(path, std::ios::in);
						path.erase(10);
						path.insert(10, std::to_string(*j));
						path += "_NoNull.json";
						std::ofstream output_file(path);
						while (!input_file.eof())
						{
							getline(input_file, inbuf);
							size_t foundpos = inbuf.find(search_string);
							if (foundpos != std::string::npos)
								replaceAll(inbuf, search_string, replace_string);
							output_file << inbuf << std::endl;
						}
						std::vector<Rect> signRect = rectFromJson(path);;
						path.erase(10);

						for (auto k = signRect.cbegin(); k != signRect.cend(); ++k)
						{
							croppedSigns.push_back(currCadr(*k));
							grayCroppedSigns.push_back(currCadr(*k));
							namedWindow("croppedSign", 0);
							imshow("croppedSign", test.cadr(i)(*k));
							medianBlur(grayCroppedSigns[l], grayCroppedSigns[l], 9);
							cvtColor(grayCroppedSigns[l], grayCroppedSigns[l], COLOR_BGR2GRAY);
							threshold(grayCroppedSigns[l], grayCroppedSigns[l], 83, 255, THRESH_BINARY);

							findContours(grayCroppedSigns[l], contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
							approxPolyDP(Mat(contours[0]), contours[0], 5, true);
							std::vector<Point> PolyPoints;
							for (auto pts = contours[0].begin(); pts != contours[0].end(); ++pts)
							{
								pts[0].x += signRect[0].x;
								pts[0].y += signRect[0].y;
								PolyPoints.push_back(Point(pts[0].x, pts[0].y));
								std::cout << "point x - " << pts[0].x << ", point y - " << pts[0].y << std::endl;
								pts[0].x -= signRect[0].x;
								pts[0].y -= signRect[0].y;
							}
							std::cout << std::endl;
							saveResJson(std::to_string(i) + "_Sign_" + std::to_string(l) + ".json", PolyPoints);

							drawContours(croppedSigns[l], contours, -1, Scalar(0, 0, 255), 1);

							namedWindow("PolySign", 0);
							imshow("PolySign", croppedSigns[l]);
							l++;
							if (cv::waitKey(500) >= 0) break;
						}
					}
				}
				if (cv::waitKey(100) >= 0) break;
			}
		}
	}
	if (event == EVENT_LBUTTONUP)
	{
		rectangle(canvas, button1, Scalar(200, 200, 200), 2);
	}

	imshow("Button", canvas);
	cv::waitKey(1);
}

int main() {
	Mat3b img(60, 50, Vec3b(0, 255, 0));
	button1 = Rect(0, 0, img.cols, img.rows);
	canvas = Mat3b(img.rows, img.cols, Vec3b(0, 0, 0));
	canvas(button1) = Vec3b(200, 200, 200);
	putText(canvas(button1), "click!", Point(button1.width * 0.15, button1.height * 0.6), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));

	namedWindow("Button", 0);
	setMouseCallback("Button", callBackFuncButton1);
	imshow("Button", canvas);

	showDav(test, jsonCadrNumbers, path);

    cv::waitKey(0);
}
