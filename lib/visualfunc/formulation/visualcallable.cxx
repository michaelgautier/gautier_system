#include "visualcallable.hxx"
using namespace std;

using cls = visualfunc::formulation::visualcallable;

int cls::id() {
        return _id;
}

void cls::id(int v) {
        _id = v;
        
        return;
}

int cls::x() {
        return _x;
}

void cls::x(int v) {
        _x = v;
        
        return;
}

int cls::y() {
        return _y;
}

void cls::y(int v) {
        _y = v;
        
        return;
}

int cls::w() {
        return _w;
}

void cls::w(int v) {
        _w = v;
        
        return;
}

int cls::h() {
        return _h;
}

void cls::h(int v) {
        _h = v;
        
        return;
}

string cls::label() {
        return _label;
}

void cls::label(string v) {
        _label = v;
        
        return;
}
