#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>

#include "visualcallable.hxx"
#include "visualcallableproto.hxx"

using namespace std;
using cls = visualfunc::formulation::visualcallableproto;
using visualcallable = visualfunc::formulation::visualcallable;

vector<visualcallable> cls::get_callables() {
        vector<visualcallable> callables;
        
        int next_y = 0;
        int text_w = 0;
        int text_h = 0;
        int accumulated_h = 0;
        int remaining_h = 0;

        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;

        int rh = 0;

        const int max_elems = 6;

        for(int index = 0; index < max_elems; index++) {
                visualcallable callable(index);

                x = 0;
                y = next_y;
                w = _screen_w;

                switch(index) {
                        case 0://RSS Reader Header
                        {
                                fl_font(FL_HELVETICA, 72);
                                
                                /*
                                        Usually results in a bounding box larger than  
                                        the text based on the same font size.
                                */
                                fl_measure("W", text_w, text_h, 1);

                                callable.label("RSS Reader");

                                h = text_h;
                        }
                        break;
                        case 1://RSS Reader Headlines
                        {
                                h = remaining_h / 2;
                        }
                        break;
                        case 2://RSS Reader article content
                        {
                                double dv = 1.4;
                                h = remaining_h / dv;
                        }
                        break;
                        case 3:
                        {
                                double dv = 3;
                                rh = remaining_h / dv;

                                h = rh;

                                callable.label("test 1");
                        }
                        break;
                        case 4:
                        {
                                h = rh;

                                callable.label("test 2");
                        }
                        break;
                        case 5:
                        {
                                h = rh;
                                
                                callable.label("test 3");
                        }
                        break;
                }

                callable.x(x);
                callable.y(y);
                callable.w(w);
                callable.h(h);
                
                accumulated_h = (accumulated_h + callable.h());
                next_y = (next_y + callable.h());
                //remaining_h = ((workarea_h - window_chrome_offset) - accumulated_h);
                remaining_h = (_screen_h - accumulated_h);

                //cout << "index: " << index << " ";
                //cout << " h: " << h;
                //cout << " accumulated_h: " << accumulated_h;
                //cout << " remaining_h: " << remaining_h;
                //cout << "\n";

                callables.push_back(callable);
        }
        
        return callables;
}
