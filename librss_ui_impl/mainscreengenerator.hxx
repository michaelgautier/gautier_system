/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#ifndef __rss_ui_mainscreengenerator__
#define __rss_ui_mainscreengenerator__
#include <cctype>
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <wx/wx.h>
#include <wx/hyperlink.h>

#include "rss_cycle_feed_article.hxx"
#include "rss_cycle_feed_headline.hxx"
#include "rss_cycle_feed_name.hxx"
#include "rss_data_feed_article_spec.hxx"
#include "rss_data_feed_headline_spec.hxx"
#include "rss_data_feed_name_spec.hxx"
#include "rss_set_feed_headline.hxx"
#include "rss_set_feed_name.hxx"
#include "textfilter.hxx"

namespace rss {
namespace ui {
class mainscreengenerator : public wxFrame {
  public:
    mainscreengenerator(const wxString& title, const wxPoint& pos, const wxSize& size, int style);
    void generate();

  private:
    /*


    	wxWidgets	EventHandlers


    */
    void headline_list_select_event(wxCommandEvent& event);
    void feed_change_event(wxCommandEvent& event);
    void feed_save_event(wxCommandEvent& event);
    /*


    	Application Logic


    */
    std::vector<::news::rss_data_feed_headline_spec> _feed_headlines;

    int _feed_index = 0;

    bool update_feed_source();
    std::vector<news::rss_data_feed_name_spec> get_feed_names();
    news::rss_data_feed_name_spec get_feed_name(const int feed_index);
    std::string get_feeds_location();

    void show_feed(const int feed_index);
    void show_feed_names();
    void show_headlines();
    void show_headline_description(const int headline_index);
    void show_feed_edit(const int feed_index);
    /*


    	User Interface


    */
    wxBoxSizer* _frame_sizer;
    wxStaticText* _program_header;
    wxListBox* _headlines_list_view;

    wxPanel* _feed_info_panel;
    wxBoxSizer* _feed_info_sizer;

    wxStaticText* _feed_name_text;
    wxHyperlinkCtrl* _feed_headline_hyperlink;
    wxStaticText* _article_summary_text;

    wxPanel* _feed_input_panel;
    wxBoxSizer* _feed_input_sizer;

    wxStaticText* _feed_name_label;
    wxTextCtrl* _feed_name_edit;
    wxStaticText* _feed_url_label;
    wxTextCtrl* _feed_url_edit;
    wxButton* _feed_name_save_button;

    wxScrolledWindow* _feed_buttons_panel;
    wxBoxSizer* _feed_buttons_sizer;

    void create_ui_window();
    void create_ui_region_header();
    void create_ui_region_headlines();
    void create_ui_region_article_summary();
    void create_ui_region_content();
    void create_ui_region_feed_edit();
    void create_ui_region_feed_names();
};
}
}
#endif
