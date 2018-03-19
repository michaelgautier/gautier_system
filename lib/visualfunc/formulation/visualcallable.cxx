#include "visualcallable.hxx"
#include <vector>

using namespace std;

using cls = visualfunc::formulation::visualcallable;

int cls::id() {
    return _id;
}

void cls::id(const int v) {
    _id = v;

    return;
}

int cls::type_id() {
    return _type_id;
}

void cls::type_id(const int v) {
    _type_id = v;

    return;
}

double cls::x() {
    return _x;
}

void cls::x(const double v) {
    _x = v;

    return;
}

double cls::y() {
    return _y;
}

void cls::y(const double v) {
    _y = v;

    return;
}

double cls::w() {
    return _w;
}

void cls::w(const double v) {
    _w = v;

    return;
}

double cls::h() {
    return _h;
}

void cls::h(const double v) {
    _h = v;

    return;
}

double cls::x1() {
    return _x;
}

double cls::x2() {
    return _w;
}

double cls::y1() {
    return _y;
}

double cls::y2() {
    return _y + _h;
}

double cls::line_stroke_width_left() {
    return _line_stroke_width_left;
}

void cls::line_stroke_width_left(const double v) {
    _line_stroke_width_left = v;

    return;
}

double cls::line_stroke_width_top() {
    return _line_stroke_width_top;
}

void cls::line_stroke_width_top(const double v) {
    _line_stroke_width_top = v;

    return;
}

double cls::line_stroke_width_right() {
    return _line_stroke_width_right;
}

void cls::line_stroke_width_right(const double v) {
    _line_stroke_width_right = v;

    return;
}

double cls::line_stroke_width_bottom() {
    return _line_stroke_width_bottom;
}

void cls::line_stroke_width_bottom(const double v) {
    _line_stroke_width_bottom = v;

    return;
}

void cls::line_stroke_width(const double v) {
    _line_stroke_width_top = v;
    _line_stroke_width_left = v;
    _line_stroke_width_right = v;
    _line_stroke_width_bottom = v;

    return;
}

double cls::line_stroke_width() {
    double stroke_width = _line_stroke_width_top;

    vector<double> stroke_widths = {_line_stroke_width_left, _line_stroke_width_right, _line_stroke_width_bottom};

    for(int stroke_width_index; stroke_width_index < stroke_widths.size(); stroke_width_index++) {
        double stroke_width_value = stroke_widths[stroke_width_index];

        if(stroke_width_value > stroke_width) {
            stroke_width = stroke_width_value;
        }
    }

    return stroke_width;
}

string cls::label() {
    return _label;
}

void cls::label(string v) {
    _label = string(v);

    return;
}

void cls::label(const char* v) {
    _label = string(v);

    return;
}

vector<cls> cls::callables() {
    return _callables;
}

void cls::callables(vector<cls> v) {
    _callables = v;

    return;
}

void cls::add_descendant(visualcallable v) {
    _callables.push_back(v);

    return;
}
