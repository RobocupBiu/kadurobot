#ifndef __KHR2_Interface_h
#define __KHR2_Interface_h

#ifdef WX_INTERFACE
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/notebook.h>
#endif

#include <list>

class RCBMotion;
class MyCanvas;
class rcb3_simulator;

#ifdef WX_INTERFACE
// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    virtual ~MyFrame();

    // event handlers (these functions should _not_ be virtual)
    void DoubleClickListBox(wxCommandEvent& event);
    void DC_Scenario(wxCommandEvent& event);
    void SaveScenarioToFile(wxCommandEvent& event);
    void SaveScenarioToSN(wxCommandEvent& event);
    void LoadMotion(wxCommandEvent& event);
    void LoadScenario(wxCommandEvent& event);
    void NewScenario(wxCommandEvent& event);
    void AddMotion(wxCommandEvent& event);
    void DelMotion(wxCommandEvent& event);

    void PrepareDC(wxDC& dc);

    int         m_backgroundMode;
    int         m_textureBackground;
    int         m_mapMode;
    double      m_xUserScale;
    double      m_yUserScale;
    int         m_xLogicalOrigin;
    int         m_yLogicalOrigin;
    bool        m_xAxisReversed,
                m_yAxisReversed;
    wxColour    m_colourForeground,    // these are _text_ colours
                m_colourBackground;
    wxBrush     m_backgroundBrush;

    MyCanvas   *m_canvas;

    wxListBox  *m_motions_lb;
    wxListBox  *m_scenarios_lb;
    wxListBox  *m_new_scenario;

    std::list<wxSizer *> m_sizers;
    std::list<wxButton *> m_buttons;

    rcb3_simulator *m_simulator;
    
private:

    RCBMotion *StoreCreatedMotionToClass();

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// define a scrollable canvas for drawing onto
class MyCanvas: public wxScrolledWindow
{
public:
    MyCanvas( MyFrame *parent);

    void OnPaint(wxPaintEvent &event);
    void OnMouseMove(wxMouseEvent &event);

    void DisplayMotion(RCBMotion *m);
    void ActivatePosition(RCBMotion *m,int pos);

protected:
    void BuildBitmap();

private:
    MyFrame     *m_owner;
    wxIcon      m_std_icon;
    wxBitmap    *m_bitmap;

    RCBMotion *m_motion;
    int        m_active_position;
    DECLARE_EVENT_TABLE()
};
#endif

class RCBMotion;

class KHR2_Interface
{
public:
    KHR2_Interface(void);
    ~KHR2_Interface(void);

    void Update();
    void Refresh();

    void DisplayMotion(RCBMotion *m);
    void ActivatePosition(RCBMotion *m,int pos);

    void RegisterMotion(int index,char *name);
    void RegisterScenario(int index,char *name);

    void SetSimulator(rcb3_simulator *simulator);
#ifdef WX_INTERFACE
    wxWindow * GetWindow();
private:
    MyFrame *m_frame;
#endif
};

#endif // __KHR2_Interface_h
