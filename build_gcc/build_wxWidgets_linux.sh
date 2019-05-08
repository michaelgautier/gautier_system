../configure --prefix=/home/michaelgautier/build_shared/wxWidgets --with-gtk=3 --with-opengl --enable-monolithic --disable-shared --disable-precomp-headers --disable-debug_flag --enable-exceptions --enable-stl --enable-std_containers --enable-std_iostreams --enable-std_string --enable-unicode --disable-ftp --disable-http --enable-sockets --disable-ipv6 --disable-ole --disable-arcstream --disable-base64 --enable-cmdline --disable-dialupman --enable-dynlib --enable-dynamicloader --disable-fs_inet --disable-fs_zip --disable-sound --disable-propgrid --disable-ribbon --disable-mediactrl --disable-gstreamer8 --enable-menus --disable-miniframe --enable-tooltips --disable-splines --enable-validators --disable-busyinfo --disable-hotkey --disable-joystick --disable-accessibility --disable-uiactionsim --disable-palette --disable-compat26 --disable-compat28

#  --build=BUILD     configure for building on BUILD [guessed]
#  --host=HOST       cross-compile to build programs to run on HOST [BUILD]

#  --enable-monolithic     build wxWidgets as single library
#  --enable-plugins        build parts of wxWidgets as loadable components
#  --enable-official_build official build of wxWidgets (win32 DLL only)
#  --enable-vendor=VENDOR  vendor name (win32 DLL only)
#  --disable-all-features  disable all optional features to build minimal library
#  --enable-universal      use wxWidgets GUI controls instead of native ones
#  --enable-nanox          use NanoX
#  --enable-gpe            use GNOME PDA Environment features if possible
#  --enable-debug          build library for debugging
#  --disable-debug_flag    disable all debugging support
#  --enable-debug_info     generate debug information
#  --enable-debug_gdb      create code with extra GDB debugging information
#  --enable-debug_cntxt    obsolete, don't use: use wxDebugContext
#  --enable-mem_tracing    obsolete, don't use: create code with memory tracing
#  --disable-shared        create static library instead of shared
#  --enable-stl            use standard C++ classes for everything
#  --enable-std_containers use standard C++ container classes
#  --enable-std_iostreams  use standard C++ stream classes
#  --enable-std_string     use standard C++ string classes
# --enable-std_string_conv_in_wxstring     provide implicit conversion to std::string in wxString
#  --disable-unicode       compile without Unicode support
#  --enable-mslu           use MS Layer for Unicode on Windows 9x (Win32 only)
#  --enable-utf8           use UTF-8 representation for strings (Unix only)
#  --enable-utf8only       only support UTF-8 locales in UTF-8 build (Unix only)
#  --enable-extended_rtti  use extended RTTI (XTI)
#  --enable-omf            use OMF object format
#  --disable-optimise      compile without optimisations
#  --enable-profile        create code with profiling information
#  --enable-no_rtti        create code without RTTI information
#  --enable-no_exceptions  create code without C++ exceptions handling
#  --enable-permissive     compile code disregarding strict ANSI
#  --enable-no_deps        create code without dependency information
#  --disable-vararg_macros don't use vararg macros, even if they are supported
#  --enable-universal_binary=archs create universal binary for the specified (or all supported) architectures
#  --enable-macosx_arch=ARCH  build for just the specified architecture
#  --enable-compat26       enable wxWidgets 2.6 compatibility
#  --disable-compat28      disable wxWidgets 2.8 compatibility
#  --disable-rpath         disable use of rpath for uninstalled builds
#  --enable-objc_uniquifying enable Objective-C class name uniquifying
#  --disable-visibility    disable use of ELF symbols visibility even if supported
#  --disable-tls           disable use of compiler TLS support
#  --enable-intl           use internationalization system
#  --enable-xlocale        use x-locale support (requires wxLocale)
#  --enable-config         use wxConfig (and derived) classes
#  --enable-protocols      use wxProtocol and derived classes
#  --enable-ftp            use wxFTP (requires wxProtocol
#  --enable-http           use wxHTTP (requires wxProtocol
#  --enable-fileproto      use wxFileProto class (requires wxProtocol
#  --enable-sockets        use socket/network classes
#  --enable-ipv6           enable IPv6 support in wxSocket
#  --enable-ole            use OLE classes (Win32 only)
#  --enable-dataobj        use data object classes
#  --enable-ipc            use interprocess communication (wxSocket etc.)
#  --enable-baseevtloop    use event loop in console programs too
#  --enable-epollloop      use wxEpollDispatcher class (Linux only)
#  --enable-selectloop     use wxSelectDispatcher class
#  --enable-any            use wxAny class
#  --enable-apple_ieee     use the Apple IEEE codec
#  --enable-arcstream      use wxArchive streams
#  --enable-base64         use base64 encoding/decoding functions
#  --enable-backtrace      use wxStackWalker class for getting backtraces
#  --enable-catch_segvs    catch signals in wxApp::OnFatalException (Unix only)
#  --enable-cmdline        use wxCmdLineParser class
#  --enable-datetime       use wxDateTime class
#  --enable-debugreport    use wxDebugReport class
#  --enable-dialupman      use dialup network classes
#  --enable-dynlib         use wxLibrary class for DLL loading
#  --enable-dynamicloader  use (new) wxDynamicLibrary class
#  --enable-exceptions     build exception-safe library
#  --enable-ffile          use wxFFile class
#  --enable-file           use wxFile class
#  --enable-filehistory    use wxFileHistory class
#  --enable-filesystem     use virtual file systems classes
#  --enable-fontenum       use wxFontEnumerator class
#  --enable-fontmap        use font encodings conversion classes
#  --enable-fs_archive     use virtual archive filesystems
#  --enable-fs_inet        use virtual HTTP/FTP filesystems
#  --enable-fs_zip         now replaced by fs_archive
#  --enable-fsvolume       use wxFSVolume class
#  --enable-fswatcher      use wxFileSystemWatcher class
#  --enable-geometry       use geometry class
#  --enable-log            use logging system
#  --enable-longlong       use wxLongLong class
#  --enable-mimetype       use wxMimeTypesManager
#  --enable-printfposparam use wxVsnprintf() which supports positional parameters
#  --enable-snglinst       use wxSingleInstanceChecker class
#  --enable-sound          use wxSound class
#  --enable-stdpaths       use wxStandardPaths class
#  --enable-stopwatch      use wxStopWatch class
#  --enable-streams        use wxStream etc classes
#  --enable-sysoptions     use wxSystemOptions
#  --enable-tarstream      use wxTar streams
#  --enable-textbuf        use wxTextBuffer class
#  --enable-textfile       use wxTextFile class
#  --enable-timer          use wxTimer class
#  --enable-variant        use wxVariant class
#  --enable-zipstream      use wxZip streams
#  --enable-url            use wxURL class
#  --enable-protocol       use wxProtocol class
#  --enable-protocol-http  HTTP support in wxProtocol
#  --enable-protocol-ftp   FTP support in wxProtocol
#  --enable-protocol-file  FILE support in wxProtocol
#  --enable-threads        use threads
#  --enable-iniconf        use wxIniConfig (Win32 only)
#  --enable-regkey         use wxRegKey class (Win32 only)
#  --enable-docview        use document view architecture
#  --enable-help           use help subsystem
#  --enable-mshtmlhelp     use MS HTML Help (win32)
#  --enable-html           use wxHTML sub-library
#  --enable-htmlhelp       use wxHTML-based help
#  --enable-xrc            use XRC resources sub-library
#  --enable-aui            use AUI docking library
#  --enable-propgrid       use wxPropertyGrid library
#  --enable-ribbon         use wxRibbon library
#  --enable-stc            use wxStyledTextCtrl library
#  --enable-constraints    use layout-constraints system
#  --enable-loggui         use standard GUI logger
#  --enable-logwin         use wxLogWindow
#  --enable-logdialog      use wxLogDialog
#  --enable-mdi            use multiple document interface architecture
#  --enable-mdidoc         use docview architecture with MDI
#  --enable-mediactrl      use wxMediaCtrl class
#  --enable-gstreamer8     force GStreamer 0.8 instead of 0.10 with the wxMediaCtrl class on unix
#  --enable-richtext       use wxRichTextCtrl
#  --enable-postscript     use wxPostscriptDC device context (default for gtk+)
#  --enable-printarch      use printing architecture
#  --enable-svg            use wxSVGFileDC device context
#  --enable-webkit         use wxWebKitCtrl (Mac-only, use wxWebView instead)
#  --enable-webview        use wxWebView library
#  --enable-graphics_ctx   use graphics context 2D drawing API
#  --disable-qtkit         force AVFoundation instead of QTKit (requires OS X 10.7+)
#  --enable-clipboard      use wxClipboard class
#  --enable-dnd            use Drag'n'Drop classes
#  --disable-controls      disable compilation of all standard controls
#  --enable-markup         support wxControl::SetLabelMarkup
#  --enable-accel          use accelerators
#  --enable-animatectrl    use wxAnimationCtrl class
#  --enable-bannerwindow   use wxBannerWindow class
#  --enable-artstd         use standard XPM icons in wxArtProvider
#  --enable-arttango       use Tango icons in wxArtProvider
#  --enable-bmpbutton      use wxBitmapButton class
#  --enable-bmpcombobox    use wxBitmapComboBox class
#  --enable-button         use wxButton class
#  --enable-calendar       use wxCalendarCtrl class
#  --enable-caret          use wxCaret class
#  --enable-checkbox       use wxCheckBox class
#  --enable-checklst       use wxCheckListBox (listbox with checkboxes) class
#  --enable-choice         use wxChoice class
#  --enable-choicebook     use wxChoicebook class
#  --enable-collpane       use wxCollapsiblePane class
#  --enable-colourpicker   use wxColourPickerCtrl class
#  --enable-combobox       use wxComboBox class
#  --enable-comboctrl      use wxComboCtrl class
#  --enable-commandlinkbutton      use wxCommmandLinkButton class
#  --enable-dataviewctrl   use wxDataViewCtrl class
#  --enable-datepick       use wxDatePickerCtrl class
#  --enable-detect_sm      use code to detect X11 session manager
#  --enable-dirpicker      use wxDirPickerCtrl class
#  --enable-display        use wxDisplay class
#  --enable-editablebox    use wxEditableListBox class
#  --enable-filectrl       use wxFileCtrl class
#  --enable-filepicker     use wxFilePickerCtrl class
#  --enable-fontpicker     use wxFontPickerCtrl class
#  --enable-gauge          use wxGauge class
#  --enable-grid           use wxGrid class
#  --enable-headerctrl     use wxHeaderCtrl class
#  --enable-hyperlink      use wxHyperlinkCtrl class
#  --enable-imaglist       use wxImageList class
#  --enable-infobar        use wxInfoBar class
#  --enable-listbook       use wxListbook class
#  --enable-listbox        use wxListBox class
#  --enable-listctrl       use wxListCtrl class
#  --enable-notebook       use wxNotebook class
#  --enable-notifmsg       use wxNotificationMessage class
#  --enable-odcombobox     use wxOwnerDrawnComboBox class
#  --enable-popupwin       use wxPopUpWindow class
#  --enable-prefseditor    use wxPreferencesEditor class
#  --enable-radiobox       use wxRadioBox class
#  --enable-radiobtn       use wxRadioButton class
#  --enable-richmsgdlg     use wxRichMessageDialog class
#  --enable-richtooltip    use wxRichToolTip class
#  --enable-rearrangectrl  use wxRearrangeList/Ctrl/Dialog
#  --enable-sash           use wxSashWindow class
#  --enable-scrollbar      use wxScrollBar class and scrollable windows
#  --enable-searchctrl     use wxSearchCtrl class
#  --enable-slider         use wxSlider class
#  --enable-spinbtn        use wxSpinButton class
#  --enable-spinctrl       use wxSpinCtrl class
#  --enable-splitter       use wxSplitterWindow class
#  --enable-statbmp        use wxStaticBitmap class
#  --enable-statbox        use wxStaticBox class
#  --enable-statline       use wxStaticLine class
#  --enable-stattext       use wxStaticText class
#  --enable-statusbar      use wxStatusBar class
#  --enable-taskbaricon    use wxTaskBarIcon class
#  --enable-tbarnative     use native wxToolBar class
#  --enable-textctrl       use wxTextCtrl class
#  --enable-timepick       use wxTimePickerCtrl class
#  --enable-tipwindow      use wxTipWindow class
#  --enable-togglebtn      use wxToggleButton class
#  --enable-toolbar        use wxToolBar class
#  --enable-toolbook       use wxToolbook class
#  --enable-treebook       use wxTreebook class
#  --enable-treectrl       use wxTreeCtrl class
#  --enable-treelist       use wxTreeListCtrl class
#  --enable-commondlg      use all common dialogs
#  --enable-aboutdlg       use wxAboutBox
#  --enable-choicedlg      use wxChoiceDialog
#  --enable-coldlg         use wxColourDialog
#  --enable-filedlg        use wxFileDialog
#  --enable-finddlg        use wxFindReplaceDialog
#  --enable-fontdlg        use wxFontDialog
#  --enable-dirdlg         use wxDirDialog
#  --enable-msgdlg         use wxMessageDialog
#  --enable-numberdlg      use wxNumberEntryDialog
#  --enable-splash         use wxSplashScreen
#  --enable-textdlg        use wxTextDialog
#  --enable-tipdlg         use startup tips
#  --enable-progressdlg    use wxProgressDialog
#  --enable-wizarddlg      use wxWizard
#  --enable-menus          use wxMenu/wxMenuBar/wxMenuItem classes
#  --enable-miniframe      use wxMiniFrame class
#  --enable-tooltips       use wxToolTip class
#  --enable-splines        use spline drawing code
#  --enable-mousewheel     use mousewheel
#  --enable-validators     use wxValidator and derived classes
#  --enable-busyinfo       use wxBusyInfo
#  --enable-hotkey         use wxWindow::RegisterHotKey()
#  --enable-joystick       use wxJoystick
#  --enable-metafiles      use wxMetaFile
#  --enable-dragimage      use wxDragImage
#  --enable-accessibility  enable accessibility support
#  --enable-uiactionsim    use wxUIActionSimulator (experimental)
#  --enable-dctransform    use wxDC::SetTransformMatrix and related
#  --enable-webviewwebkit  use wxWebView WebKit backend
#  --enable-palette        use wxPalette class
#  --enable-image          use wxImage class
#  --enable-gif            use gif images (GIF file format)
#  --enable-pcx            use pcx images (PCX file format)
#  --enable-tga            use tga images (TGA file format)
#  --enable-iff            use iff images (IFF file format)
#  --enable-pnm            use pnm images (PNM file format)
#  --enable-xpm            use xpm images (XPM file format)
#  --enable-ico_cur        use Windows ICO and CUR formats
#  --enable-dccache        cache temporary wxDC objects (Win32 only)
#  --enable-ps-in-msw      use PS printing in wxMSW (Win32 only)
#  --enable-ownerdrawn     use owner drawn controls (Win32 and OS/2 only)
#  --enable-uxtheme        enable support for Windows XP themed look (Win32 only)
#  --enable-wxdib          use wxDIB class (Win32 only)
#  --enable-webviewie      use wxWebView IE backend (Win32 only)
#  --enable-autoidman      use automatic ids management
#  --disable-largefile     omit support for large files
#  --disable-gtktest       do not try to compile and run a test GTK+ program
#  --disable-gtktest       Do not try to compile and run a test GTK program
#  --disable-sdltest       Do not try to compile and run a test SDL program
#  --enable-omf            use OMF object format (OS/2)
#  --disable-dependency-tracking don't use dependency tracking even if the compiler

#  --disable-precomp-headers don't use precompiled headers even if compiler can

#  --with-PACKAGE[=ARG]    use PACKAGE [ARG=yes]
#  --without-PACKAGE       do not use PACKAGE (same as --with-PACKAGE=no)
#  --without-subdirs       don't generate makefiles for samples/demos/...
#  --with-flavour=NAME     specify a name to identify this build
#  --with-themes=all|list  use only the specified comma-separated list of wxUniversal themes
#  --with-gtk[=VERSION]    use GTK+, VERSION can be 3, 2 (default), 1 or "any"
#  --with-motif            use Motif/Lesstif
#  --with-osx_carbon       use Mac OS X (Carbon)
#  --with-osx_cocoa        use Mac OS X (Cocoa)
#  --with-osx_iphone       use iPhone OS X port
#  --with-osx              use Mac OS X (default port, Cocoa)
#  --with-carbon           same as --with-osx_carbon
#  --with-cocoa            same as --with-osx_cocoa
#  --with-iphone           same as --with-osx_iphone
#  --with-mac              same as --with-osx
#  --with-old_cocoa        use old, deprecated, Cocoa port
#  --with-wine             use Wine
#  --with-msw              use MS-Windows
#  --with-pm               use OS/2 Presentation Manager
#  --with-directfb         use DirectFB
#  --with-microwin         use MicroWindows
#  --with-x11              use X11
#  --with-libpng           use libpng (PNG image format)
#  --with-libjpeg          use libjpeg (JPEG file format)
#  --with-libtiff          use libtiff (TIFF file format)
#  --without-libjbig       don't use libjbig in libtiff even if available)
#  --without-liblzma       don't use liblzma in libtiff even if available)
#  --with-libxpm           use libxpm (XPM file format)
#  --with-libiconv         use libiconv (character conversion)
#  --with-libmspack        use libmspack (CHM help files loading)
#  --without-gtkprint      don't use GTK printing support
#  --with-gnomevfs         use GNOME VFS for associating MIME types
#  --with-libnotify        use libnotify for notifications
#  --with-hildon           use Hildon framework for Nokia 770/800/810
#  --with-opengl           use OpenGL (or Mesa)
#  --with-dmalloc          use dmalloc library (http://dmalloc.com/)
#  --with-sdl              use SDL for audio on Unix
#  --with-regex            enable support for wxRegEx class
#  --with-zlib             use zlib for LZW compression
#  --with-expat            enable XML support using expat parser
#  --with-macosx-sdk=PATH  use an OS X SDK at PATH
#  --with-macosx-version-min=VER   build binaries which require at least this OS X version
#  --with-gtk-prefix=PFX   Prefix where GTK is installed (optional)
#  --with-gtk-exec-prefix=PFX Exec prefix where GTK is installed (optional)
#  --with-x                use the X Window System
#  --with-libiconv-prefix=DIR  search for libiconv in DIR/include and DIR/lib
#  --with-sdl-prefix=PFX   Prefix where SDL is installed (optional)
#  --with-sdl-exec-prefix=PFX Exec prefix where SDL is installed (optional)
#  --with-cppunit-prefix=PFX   Prefix where CppUnit is installed (optional)
#  --with-cppunit-exec-prefix=PFX  Exec prefix where CppUnit is installed (optional)



