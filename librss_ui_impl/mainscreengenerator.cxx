/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#include "mainscreengenerator.hxx"

rss::ui::mainscreengenerator::mainscreengenerator(const wxString& title, const wxPoint& pos, const wxSize& size, int style) : wxFrame(NULL, wxID_ANY, title, pos, size, style) {
}
/*


	Constructors	Initialization


*/
void rss::ui::mainscreengenerator::generate() {
    create_ui_window();

    show_feed(0);

    show_feed_names();

    CenterOnParent();
    Show(true);

    return;
}
/*


	wxWidgets	EventHandlers


*/
/*		Headline Click ... Shows the headline details*/
void rss::ui::mainscreengenerator::headline_list_select_event(wxCommandEvent& event) {
    int id = _headlines_list_view->GetSelection();

    if(id > -1) {
        show_headline_description(id);
    }

    return;
}
/*		Feed Click ... Shows the headlines for a feed*/
void rss::ui::mainscreengenerator::feed_change_event(wxCommandEvent& event) {
    int event_id = event.GetId();

    if(event_id > -1) {
        show_feed(event_id);
    }

    return;
}
/*		Feed Save Click ... Add/Update a feed name and network address*/
void rss::ui::mainscreengenerator::feed_save_event(wxCommandEvent& event) {
    int feed_index = _feed_index;

    std::vector<news::rss_data_feed_name_spec> feed_names_before = get_feed_names();
    news::rss_data_feed_name_spec feed_name_before = get_feed_name(feed_index);

    update_feed_source();

    std::vector<news::rss_data_feed_name_spec> feed_names_after = get_feed_names();

    const int feed_names_size_before = feed_names_before.size();
    const int feed_names_size_after = feed_names_after.size();

    if(feed_names_size_before != feed_names_size_after) {
        if(feed_names_size_after > 0 && feed_index < feed_names_size_after) {
            news::rss_data_feed_name_spec feed_name_after = feed_names_after[feed_index];

            if(!(feed_name_after.name == feed_name_before.name && feed_name_after.url == feed_name_before.url)) {
                show_feed(feed_index);
            } else {
                show_feed_edit(feed_index);
            }
        } else {
            show_feed(0);
        }

        show_feed_names();

        /*Triggers size and placement re-calculation ... call before update*/
        Layout();
        /*Draws the window contents*/
        Update();
    }

    return;
}
/*


	Application Logic


*/
std::string rss::ui::mainscreengenerator::get_feeds_location() {
    return "feeds.txt";
}

std::vector<news::rss_data_feed_name_spec> rss::ui::mainscreengenerator::get_feed_names() {
    news::rss_cycle_feed_name rss_c_feed_name;

    rss_c_feed_name.init(get_feeds_location());
    news::rss_set_feed_name feed_name_set = rss_c_feed_name.get_feed_names();

    std::vector<news::rss_data_feed_name_spec> feed_names = feed_name_set.get_specs();

    return feed_names;
}

news::rss_data_feed_name_spec rss::ui::mainscreengenerator::get_feed_name(const int feed_index) {
    news::rss_cycle_feed_name rss_c_feed_name;

    rss_c_feed_name.init(get_feeds_location());
    news::rss_data_feed_name_spec feed_name = rss_c_feed_name.get_single_feed_name(feed_index);

    return feed_name;
}

void rss::ui::mainscreengenerator::show_feed(const int feed_index) {
    _feed_index = feed_index;

    show_feed_edit(feed_index);

    show_headlines();

    return;
}

void rss::ui::mainscreengenerator::show_feed_edit(const int feed_index) {
    news::rss_data_feed_name_spec feed_name = get_feed_name(_feed_index);

    _feed_name_text->SetLabel(wxString(feed_name.name));
    _feed_name_edit->ChangeValue(wxString(feed_name.name));
    _feed_url_edit->ChangeValue(wxString(feed_name.url));

    return;
}
void rss::ui::mainscreengenerator::show_feed_names() {
    _feed_buttons_sizer->Clear(true);
    _feed_buttons_sizer->Layout();

    const std::vector<news::rss_data_feed_name_spec> feed_names = get_feed_names();

    const int feednames_size = feed_names.size();

    for(int feedname_index = 0; feedname_index < feednames_size; feedname_index++) {
        std::string feedname = feed_names[feedname_index].name;

        wxButton* feed_button = new wxButton(this, wxID_ANY, wxString(feedname));
        feed_button->SetBackgroundColour(wxColour("#ffcc00"));
        feed_button->SetForegroundColour(wxColour("#800000"));
        feed_button->SetId(feedname_index);
        feed_button->Bind(wxEVT_BUTTON, &rss::ui::mainscreengenerator::feed_change_event, this);

        _feed_buttons_sizer->Add(feed_button, 0, wxLEFT | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 7);
    }

    return;
}

void rss::ui::mainscreengenerator::show_headlines() {
    _headlines_list_view->Clear();

    _feed_headline_hyperlink->SetURL(wxString("http://about:blank"));
    _feed_headline_hyperlink->SetLabel(wxString("no headline selected"));

    _article_summary_text->SetLabel(wxString(""));

    news::rss_data_feed_name_spec feed_name = get_feed_name(_feed_index);

    news::rss_cycle_feed_headline rss_c_feed_headline;

    rss_c_feed_headline.init(feed_name.name + ".txt");
    news::rss_set_feed_headline feed_headline_set = rss_c_feed_headline.get_feed_headlines(feed_name);

    _feed_headlines = feed_headline_set.get_specs();

    const int headlines_size = _feed_headlines.size();

    for(int headline_index = 0; headline_index < headlines_size; headline_index++) {
        std::string headline = _feed_headlines[headline_index].headline;

        std::string headline_text = rss_techconstruct::make_printable(headline);

        _headlines_list_view->Append(wxString(headline_text));
    }

    if(headlines_size > 0) {
        _headlines_list_view->SetSelection(0);
        show_headline_description(0);
    }

    return;
}

void rss::ui::mainscreengenerator::show_headline_description(const int headline_index) {
    news::rss_data_feed_headline_spec feed_headline_entry;

    int headlines_size = _feed_headlines.size();

    if(headline_index < headlines_size) {
        feed_headline_entry = _feed_headlines[headline_index];

    } else {
        feed_headline_entry.url = "http://about:blank";
        feed_headline_entry.description = "Nothing to report";
    }

    _feed_headline_hyperlink->SetURL(wxString(feed_headline_entry.url));
    _feed_headline_hyperlink->SetLabel(wxString("Go to article"));

    _article_summary_text->SetLabel(wxString(rss_techconstruct::make_printable(feed_headline_entry.description)));

    Layout();
    Update();

    return;
}

bool rss::ui::mainscreengenerator::update_feed_source() {
    bool feed_sources_updated = false;

    std::string feedname = _feed_name_edit->GetValue().ToStdString();
    std::string feedurl = _feed_url_edit->GetValue().ToStdString();

    if(!feedname.empty() && !feedurl.empty()) {
        news::rss_data_feed_name_spec feed_name_entry;

        feed_name_entry.name = feedname;
        feed_name_entry.url = feedurl;

        std::vector<news::rss_data_feed_name_spec> feed_names = get_feed_names();

        bool feed_name_found = false;

        for(news::rss_data_feed_name_spec feed_name : feed_names) {
            feed_name_found = (feedname == feed_name.name);

            if(feed_name_found) {
                break;
            }
        }

        if(!feed_name_found) {
            news::rss_cycle_feed_name rss_c_feed_name;

            rss_c_feed_name.init(get_feeds_location());

            news::rss_set_consequence cs = rss_c_feed_name.set_single_feed_name(feed_name_entry);

            feed_sources_updated = (cs.size() > 0 && cs[0].get_catalog_number() > 0);
        }
    }

    return feed_sources_updated;
}
/*


	User Interface


*/
void rss::ui::mainscreengenerator::create_ui_window() {
    _frame_sizer = new wxBoxSizer(wxVERTICAL);

    SetBackgroundColour(wxColour("#f4e1c3"));

    create_ui_region_header();
    create_ui_region_headlines();
    create_ui_region_article_summary();
    create_ui_region_content();
    create_ui_region_feed_edit();
    create_ui_region_feed_names();

    _frame_sizer->Add(_program_header, 0, wxEXPAND | wxALIGN_CENTER);
    _frame_sizer->Add(_headlines_list_view, 6, wxEXPAND | wxALIGN_TOP);
    _frame_sizer->Add(_article_sizer, 0, wxEXPAND | wxALIGN_TOP);
    _frame_sizer->Add(_article_summary_text, 1, wxEXPAND | wxALIGN_TOP);
    _frame_sizer->Add(_feed_input_sizer, 0, wxEXPAND | wxALIGN_TOP);
    _frame_sizer->Add(_feed_buttons_sizer, 0, wxLEFT | wxEXPAND | wxALIGN_TOP);

    SetSizer(_frame_sizer);

    return;
}

void rss::ui::mainscreengenerator::create_ui_region_header() {
    _program_header = new wxStaticText(this, wxID_ANY, wxString("Gautier RSS"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    _program_header->SetBackgroundColour(wxColour("#ffb380"));

    wxFont program_header_font = _program_header->GetFont();
    program_header_font.SetPointSize(28);

    _program_header->SetFont(program_header_font);

    return;
}

void rss::ui::mainscreengenerator::create_ui_region_headlines() {
    _headlines_list_view = new wxListBox(this, wxID_ANY);
    _headlines_list_view->SetExtraStyle(wxLB_SINGLE | wxLB_ALWAYS_SB);

    _headlines_list_view->Bind(wxEVT_LISTBOX, &rss::ui::mainscreengenerator::headline_list_select_event, this);

    return;
}

void rss::ui::mainscreengenerator::create_ui_region_article_summary() {
    _article_sizer = new wxFlexGridSizer(1, 2, 7, 7);

    _feed_name_text = new wxStaticText(this, wxID_ANY, wxString("NO RSS FEED YET"));
    _feed_headline_hyperlink = new wxHyperlinkCtrl(this, wxID_ANY, wxString("NO RSS URL YET"), wxString("http://about:blank"));

    _article_sizer->Add(_feed_name_text, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    _article_sizer->Add(_feed_headline_hyperlink, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    return;
}

void rss::ui::mainscreengenerator::create_ui_region_content() {
    _article_summary_text = new wxStaticText(this, wxID_ANY, wxString("NO ARTICLE LOADED"));
    _article_summary_text->SetBackgroundColour(wxColour("#ffffff"));

    return;
}

void rss::ui::mainscreengenerator::create_ui_region_feed_edit() {
    _feed_input_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this);

    auto box = _feed_input_sizer->GetStaticBox();
    box->SetBackgroundColour(wxColour("#f2fffc"));

    _feed_name_label = new wxStaticText(this, wxID_ANY, wxString("Feed name"));
    _feed_name_edit = new wxTextCtrl(this, wxID_ANY);
    _feed_url_label = new wxStaticText(this, wxID_ANY, wxString("Feed address"));
    _feed_url_edit = new wxTextCtrl(this, wxID_ANY);
    _feed_name_save_button = new wxButton(this, wxID_ANY, wxString("Save"));

    _feed_name_save_button->SetBackgroundColour(wxColour("#ffcc00"));
    _feed_name_save_button->SetForegroundColour(wxColour("#800000"));

    _feed_input_sizer->Add(_feed_name_label, 0, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 7);
    _feed_input_sizer->Add(_feed_name_edit, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 7);
    _feed_input_sizer->Add(_feed_url_label, 0, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 7);
    _feed_input_sizer->Add(_feed_url_edit, 2, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 7);
    _feed_input_sizer->Add(_feed_name_save_button, 0, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 7);

    _feed_name_save_button->Bind(wxEVT_BUTTON, &rss::ui::mainscreengenerator::feed_save_event, this);

    return;
}

void rss::ui::mainscreengenerator::create_ui_region_feed_names() {
    _feed_buttons_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this);

    auto box = _feed_buttons_sizer->GetStaticBox();
    box->SetBackgroundColour(wxColour("#aa4400"));
    box->SetWindowStyle(wxBORDER_NONE);

    return;
}

