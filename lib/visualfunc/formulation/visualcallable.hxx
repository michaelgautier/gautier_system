/*
Copyright 2018 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
POCO C++ Libraries released under the Boost Software License; Copyright 2018, Applied Informatics Software Engineering GmbH and Contributors;
C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#ifndef __visualfunc_formulation_visualcallable__
#define __visualfunc_formulation_visualcallable__
#include <string>
#include <vector>

namespace visualfunc {
namespace formulation {
using namespace std;
class visualcallable {
  public:
    visualcallable(const int id) {
        _id = id;

        return;
    }

    int id();
    void id(const int v);

    int type_id();
    void type_id(const int v);

    double x();
    void x(const double v);

    double y();
    void y(const double v);

    double w();
    void w(const double v);

    double h();
    void h(const double v);

    double line_stroke_width_left();
    void line_stroke_width_left(const double v);

    double line_stroke_width_top();
    void line_stroke_width_top(const double v);

    double line_stroke_width_right();
    void line_stroke_width_right(const double v);

    double line_stroke_width_bottom();
    void line_stroke_width_bottom(const double v);

    void line_stroke_width(const double v);
    double line_stroke_width();

    double x1();
    double x2();

    double y1();
    double y2();

    string label();

    void label(string v);
    void label(const char* v);

    vector<visualcallable> callables();
    void callables(vector<visualcallable> v);
    void add_descendant(visualcallable v);
  private:
    int _id = 0;
    int _type_id = 0;

    double _x = 0;
    double _y = 0;
    double _w = 0;
    double _h = 0;

    double _line_stroke_width_left = 1;
    double _line_stroke_width_top = 1;
    double _line_stroke_width_right = 1;
    double _line_stroke_width_bottom = 1;

    string _label;

    vector<visualcallable> _callables;
};
}
}
#endif
