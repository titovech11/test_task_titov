#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;

class _Figure
{
public:
    _Figure() = default;
    _Figure(const _Figure&) = default;
    _Figure(const Point pt)
    {
        _pt1.x = pt.x;
        _pt1.y = pt.y;
    }
    ~_Figure() = default;

    _Figure& operator=(const _Figure&) = default;

    void virtual draw() {};
    void virtual rescale() {};
    void virtual hide() {};
    void virtual move() {};


    Point _pt1 = Point(0, 0);
    float scale{ 1 };
};

class _Rect : public _Figure
{
public:
    _Rect() = default;
    _Rect(const _Rect&) = default;
    _Rect(const Point pt1, const Point pt2, const int thick)
    {
        _pt1.x = pt1.x;
        _pt1.y = pt1.y;
        _pt2.x = pt2.x;
        _pt2.y = pt2.y;
        thickness = thick;
    };
    ~_Rect() = default;

    _Rect& operator=(const _Rect&) = default;

    void virtual draw(Mat& img)
    {
        rectangle(img,
            Point(_pt1.x, _pt1.y),
            Point(_pt2.x, _pt2.y),
            Scalar(255, 255, 255),
            thickness,
            lineType);
    }

    void virtual hide(Mat& img)
    {
        rectangle(img,
            Point(_pt1.x, _pt1.y),
            Point(_pt2.x, _pt2.y),
            Scalar(0, 0, 0),
            thickness,
            lineType);
    };

    void virtual rescale(const int scale_koef)
    {
        _pt2.x = _pt2.x * scale_koef;
        _pt2.y = _pt2.y * scale_koef;
        scale = scale_koef;
    }

    void virtual move(const int x, const int y)
    {
        _pt1.x = _pt1.x + x;
        _pt1.y = _pt1.y + y;
        _pt2.x = _pt2.x + x;
        _pt2.y = _pt2.y + y;
    }


private:
    Point _pt2 = Point(0, 0);
    int thickness{ 0 };
    int lineType = LINE_8;
};

class _Triangle : public _Figure
{
public:
    _Triangle() = default;
    _Triangle(const _Triangle&) = default;
    _Triangle(const Point pt1, const Point pt2, const Point pt3, const int thick)
    {
        _pt1.x = pt1.x;
        _pt1.y = pt1.y;
        _pt2.x = pt2.x;
        _pt2.y = pt2.y;
        _pt3.x = pt3.x;
        _pt3.y = pt3.y;
        thickness = thick;
    };
    ~_Triangle() = default;

    _Triangle& operator=(const _Triangle&) = default;

    void virtual draw(Mat& img)
    {
        line(img,
            _pt1,
            _pt2,
            Scalar(255, 0, 255),
            thickness,
            lineType);
        line(img,
            _pt1,
            _pt3,
            Scalar(255, 0, 255),
            thickness,
            lineType);
        line(img,
            _pt3,
            _pt2,
            Scalar(255, 0, 255),
            thickness,
            lineType);
    }

    void virtual hide(Mat& img)
    {
        line(img,
            _pt1,
            _pt2,
            Scalar(0, 0, 0),
            thickness,
            lineType);
        line(img,
            _pt1,
            _pt3,
            Scalar(0, 0, 0),
            thickness,
            lineType);
        line(img,
            _pt3,
            _pt2,
            Scalar(0, 0, 0),
            thickness,
            lineType);
    };

    void virtual rescale(const int scale_koef)
    {
        _pt2.x = _pt2.x * scale_koef;
        _pt2.y = _pt2.y * scale_koef;
        _pt3.x = _pt3.x * scale_koef;
        _pt3.y = _pt3.y * scale_koef;
    }

    void virtual move(const int x, const int y)
    {
        _pt1.x = _pt1.x + x;
        _pt1.y = _pt1.y + y;
        _pt2.x = _pt2.x + x;
        _pt2.y = _pt2.y + y;
        _pt3.x = _pt3.x + x;
        _pt3.y = _pt3.y + y;
    }


private:
    Point _pt2 = Point(0, 0);
    Point _pt3 = Point(0, 0);
    int thickness{ 0 };
    int lineType = LINE_8;
};

class _Circle : public _Figure
{
public:
    _Circle() = default;
    _Circle(const _Circle&) = default;
    _Circle(const Point center, const int r, const int thick):_r(r), thickness(thick)
    {
        _pt1.x = center.x;
        _pt1.y = center.y;
    };
    ~_Circle() = default;

    _Circle& operator=(const _Circle&) = default;

    void virtual draw(Mat& img)
    {
        circle(img,
            _pt1,
            _r,
            Scalar(0, 0, 255),
            thickness,
            lineType);
    }

    void virtual hide(Mat& img)
    {
        circle(img,
            _pt1,
            _r,
            Scalar(0, 0, 0),
            thickness,
            lineType);
    };

    void virtual rescale(const int scale_koef)
    {
        _r = _r * scale_koef;
    }

    void virtual move(const int x, const int y)
    {
        _pt1.x = _pt1.x + x;
        _pt1.y = _pt1.y + y;
    }

private:
    int _r{ 0 };
    int thickness{ 0 };
    int lineType = LINE_8;
};

class _Ellipse : public _Figure
{
public:
    _Ellipse() = default;
    _Ellipse(const _Ellipse&) = default;
    _Ellipse(const Point center, const Size size, const float angle, const int thick) : thickness(thick), _angle(angle)
    {
        _pt1.x = center.x;
        _pt1.y = center.y;
        axes = size;
    };
    ~_Ellipse() = default;

    _Ellipse& operator=(const _Ellipse&) = default;

    void virtual draw(Mat& img)
    {
        ellipse(img,
            _pt1,
            axes,
            _angle,
            0,
            360,
            Scalar(255, 0, 0),
            thickness,
            lineType);
    }

    void virtual hide(Mat& img)
    {
        ellipse(img,
            _pt1,
            axes,
            _angle,
            0,
            360,
            Scalar(255, 255, 255),
            thickness,
            lineType);
    };

    void virtual rescale(const int scale_koef)
    {
        axes.height = axes.height * scale_koef;
        axes.width = axes.width * scale_koef;
    }

    void virtual move(const int x, const int y)
    {
        _pt1.x = _pt1.x + x;
        _pt1.y = _pt1.y + y;
    }

private:
    int thickness{ 0 };
    int lineType = LINE_8;
    Size axes = Size(0, 0);
    float _angle{ 0 };
};

int main() {

    Mat img = Mat::zeros(800, 800, CV_8UC3);

    _Rect x(Point(100, 100), Point(200, 200), 2);
    x.draw(img);
    x.rescale(2);
    x.move(50, 50);
    x.draw(img);

    _Circle y(Point(400, 400), 100, 2);
    y.rescale(2);
    y.move(-50, 50);
    y.draw(img);

    _Ellipse z(Point(400, 400), Size(50, 100), 45, 2);
    z.draw(img);
    z.move(-50, 50);
    z.rescale(2);
    z.draw(img);


    _Triangle v(Point(100, 100), Point(50, 100), Point(200, 50), 2);
    v.draw(img);

    imshow("qwqw", img);
    waitKey(0);
}
