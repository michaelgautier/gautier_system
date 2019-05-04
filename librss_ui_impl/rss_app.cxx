/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#include "rss_app.hxx"

wxSize rss::news::rss_app::get_display2D_minimum_size() {
    wxRect physical_display_2D = wxGetClientDisplayRect();

    const double display_client_width2D = physical_display_2D.GetWidth();
    const double display_client_height2D = physical_display_2D.GetHeight();

    const double display_client_min_width = display_client_width2D * 0.8;
    const double display_client_min_height = display_client_height2D * 0.8;

    wxSize minimum_size((int)display_client_min_width, (int)display_client_min_height);

    return minimum_size;
}

bool rss::news::rss_app::OnInit() {
    rss::ui::mainscreengenerator* frame = new rss::ui::mainscreengenerator("Gautier RSS", wxPoint(100, 100), get_display2D_minimum_size(), wxDEFAULT_FRAME_STYLE | wxMAXIMIZE);

    frame->generate();

    return true;
}
