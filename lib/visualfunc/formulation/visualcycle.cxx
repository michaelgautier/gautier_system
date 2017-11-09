#include "visualcycle.hxx"
using namespace std;
namespace ns = visualfunc::formulation;

vector<ns::visualsheetconfigparam> ns::get_visual_sheet_config() {
        return vector<ns::visualsheetconfigparam>();
}

vector<ns::visualcallableproto> ns::get_visual_callable_prototypes() {
        return vector<ns::visualcallableproto>();
}

ns::visualsheet ns::make_visual_sheet(vector<ns::visualsheetconfigparam> parameters) {
        return ns::visualsheet();
}

vector<ns::visualcallable> ns::make_visual_callables(vector<ns::visualcallableproto> prototypes) {
        return vector<ns::visualcallable>();
}

