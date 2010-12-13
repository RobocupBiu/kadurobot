#include "KHR2_Interface.h"

#ifdef WX_INTERFACE
#include "wx/artprov.h"
#include "wx/graphics.h"
#endif

#include "KHR2_Data.h"
#include "rcb3_simulator.h"

KHR2_Interface::KHR2_Interface(void)
{
#ifdef WX_INTERFACE
    m_frame = new MyFrame("KHR2 MOTION/SCENARIO Interface",wxDefaultPosition,wxSize(500,500));
    m_frame->Show();
#endif
}

#ifdef WX_INTERFACE
wxWindow *
KHR2_Interface::GetWindow()
{
    return m_frame;
}
#endif

KHR2_Interface::~KHR2_Interface(void)
{
}

void KHR2_Interface::Refresh()
{
#ifdef WX_INTERFACE
    wxSafeYield(m_frame);
#ifdef WIN32
    //Problem with WIN32, it seems that the messages are not processed if I don't do that.
    //Normally it's the job of wxSafeYield...

    MSG msg;

    //Process 20 messages if there are available 
    int i = 0;
    while( PeekMessage( &msg, NULL, 0, 0,PM_NOREMOVE ) )
    { 
        GetMessage( &msg, NULL, 0, 0 );
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
        i++;
        if ( i == 20 )
            break;
    } 
#endif
#endif
}

#ifdef WX_INTERFACE
BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT  (MyCanvas::OnPaint)
    EVT_MOTION (MyCanvas::OnMouseMove)
END_EVENT_TABLE()

MyCanvas::MyCanvas(MyFrame *parent)
        : wxScrolledWindow(parent, wxID_ANY, wxPoint(60,0), wxSize(430,440),
                           wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
    m_owner = parent;
    m_motion = NULL;
    m_bitmap = NULL;
    m_std_icon = wxArtProvider::GetIcon(wxART_INFORMATION);
}

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC pdc(this);
    wxDC &dc = pdc ;

    PrepareDC(dc);

    m_owner->PrepareDC(dc);
    if ( m_bitmap )
        dc.DrawBitmap(*m_bitmap, 0, 0);
}

void MyCanvas::DisplayMotion(RCBMotion *m)
{
    if ( m == m_motion )
        return;

    m_motion = m;
    m_active_position = -1;
    BuildBitmap();
    Update();
}

void MyCanvas::ActivatePosition(RCBMotion *m,int pos)
{
    if ( m != m_motion )
        return;

    m_active_position = pos;
    BuildBitmap();
    Update();
}

void MyCanvas::OnMouseMove(wxMouseEvent &event)
{
}

void MyCanvas::BuildBitmap()
{
    int totalWidth = 1024;
    int totalHeight = 1024;
    if ( m_bitmap )
        delete m_bitmap;
    m_bitmap = new wxBitmap(totalWidth, totalHeight);

    wxMemoryDC memdc2;
    memdc2.SelectObject(*m_bitmap);

    wxColour clr(255, 255, 255);
    wxBrush whiteBrush(clr, wxSOLID);
    memdc2.SetBackground(whiteBrush);
    memdc2.Clear();

    wxColour clrB(0, 0, 0);
    wxPen blackPen(clrB, 1, wxSOLID);

    wxColour clrRed(255, 0, 0);
    wxPen redPen(clrRed, 1, wxSOLID);

    // Now draw a white rectangle with red outline. It should
    // entirely eclipse the yellow background.
    memdc2.SetPen(blackPen);
    memdc2.SetBrush(*wxWHITE_BRUSH);

    memdc2.SetPen(blackPen);
    int links = m_motion->m_links.size();
    for(int i=0;i<links;i++)
    {
        RCBMotionLink *l = &(m_motion->m_links[i]);

        int start = l->m_origin;
        int end = l->m_final;

        RCBMotionItem *m0 = &(m_motion->m_items[start]);
        RCBMotionItem *m1 = &(m_motion->m_items[end]);

        int xs = m0->m_left + m0->m_width / 2;
        int ys = m0->m_top + m0->m_height / 2;

        int xe = m1->m_left + m1->m_width / 2;
        int ye = m1->m_top + m1->m_height / 2;

        if ( l->m_points.size() != 0 )
        {
            int xi,yi;
            for(unsigned int j=0;j<l->m_points.size()/2;j++)
            {
                xi = l->m_points[j*2];
                yi = l->m_points[j*2+1];

                memdc2.DrawLine( xs, ys, xi, yi );
                xs = xi;
                ys = yi;
            }
        }

        memdc2.DrawLine( xs, ys, xe, ye );
    }

    int items = m_motion->m_items.size();
    for(int i=0;i<items;i++)
    {
        RCBMotionItem *it = &(m_motion->m_items[i]);

        if ( i == m_active_position )
            memdc2.SetPen(redPen);
        else
            memdc2.SetPen(blackPen);

        memdc2.DrawRectangle(it->m_left,it->m_top, it->m_width,it->m_height);

        memdc2.DrawText(wxString(it->m_name), it->m_left,it->m_top);
    }


    memdc2.SetPen(wxNullPen);
    memdc2.SetBrush(wxNullBrush);
    memdc2.SelectObject(wxNullBitmap);    
}

// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------
// IDs for the controls and the menu commands
enum
{
    MOTION_LIST_BOX = 500,
    SCENARIO_LIST_BOX = 501,
    NEW_SCENARIO_LIST_BOX = 502,
    LOAD_MOTION = 510,
    LOAD_SCENARIO = 511,
    TRANSFER_SCENARIO = 512,
    NEW_SCENARIO = 513,
    ADD_MOTION,
    DEL_MOTION,
    SAVE_SCENARIO_TO_FILE,
    // menu items
    File_Quit = wxID_EXIT,
    File_About = wxID_ABOUT,
};

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_LISTBOX_DCLICK(MOTION_LIST_BOX, MyFrame::DoubleClickListBox) 
    EVT_LISTBOX_DCLICK(SCENARIO_LIST_BOX, MyFrame::DC_Scenario) 
    EVT_BUTTON(SAVE_SCENARIO_TO_FILE, MyFrame::SaveScenarioToFile)
    EVT_BUTTON(TRANSFER_SCENARIO, MyFrame::SaveScenarioToSN)
    EVT_BUTTON(LOAD_MOTION, MyFrame::LoadMotion)
    EVT_BUTTON(LOAD_SCENARIO, MyFrame::LoadScenario)
    EVT_BUTTON(NEW_SCENARIO, MyFrame::NewScenario)
    EVT_BUTTON(ADD_MOTION, MyFrame::AddMotion)
    EVT_BUTTON(DEL_MOTION, MyFrame::DelMotion)
END_EVENT_TABLE()

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, wxID_ANY, title, pos, size,
                 wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE)
{
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to Webots KHR2 SIMULATION!"));

    m_mapMode = wxMM_TEXT;
    m_xUserScale = 1.0;
    m_yUserScale = 1.0;
    m_xLogicalOrigin = 0;
    m_yLogicalOrigin = 0;
    m_xAxisReversed =
    m_yAxisReversed = false;
    m_backgroundMode = wxSOLID;
    m_colourForeground = *wxRED;
    m_colourBackground = *wxBLUE;
    m_textureBackground = false;

    //Define the canvas which will be used to draw the motion or scenario
    m_canvas = new MyCanvas( this );
    m_canvas->SetScrollbars( 10, 10, 100, 240 );

    //The lists boxes
    //wxSize(150,200)
    m_motions_lb = new wxListBox(this,MOTION_LIST_BOX,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_SINGLE|wxLB_NEEDED_SB|wxLB_HSCROLL ,wxDefaultValidator,"Motions");
    m_scenarios_lb = new wxListBox(this,SCENARIO_LIST_BOX,wxDefaultPosition,wxSize(150,80),0,NULL,wxLB_SINGLE|wxLB_NEEDED_SB|wxLB_HSCROLL ,wxDefaultValidator,"Scenarios");
    m_new_scenario = new wxListBox(this,NEW_SCENARIO_LIST_BOX,wxDefaultPosition,wxSize(400,80),0,NULL,wxLB_MULTIPLE|wxLB_NEEDED_SB|wxLB_HSCROLL ,wxDefaultValidator,"New Scenario");

    //The buttons
    wxButton *load_motion = new wxButton(this, LOAD_MOTION, "Load Motion",wxDefaultPosition,wxSize(80,30));
    wxButton *load_scenario = new wxButton(this, LOAD_SCENARIO, "Load Scenario",wxDefaultPosition,wxSize(80,30));
    wxButton *transfer_scenario_to_rcb = new wxButton(this, TRANSFER_SCENARIO, "<-",wxDefaultPosition,wxSize(60,30));
    wxButton *new_scenario = new wxButton(this, NEW_SCENARIO, "New Scenario",wxDefaultPosition,wxSize(80,30));
    wxButton *add_motion = new wxButton(this, ADD_MOTION, "Add Motion",wxDefaultPosition,wxSize(80,30));
    wxButton *del_motion = new wxButton(this, DEL_MOTION, "Delete Motion",wxDefaultPosition,wxSize(80,30));
    wxButton *save_scenario_to_file = new wxButton(this, SAVE_SCENARIO_TO_FILE, "Save to File",wxDefaultPosition,wxSize(80,30));

    m_buttons.push_back(load_motion);
    m_buttons.push_back(load_scenario);
    m_buttons.push_back(transfer_scenario_to_rcb);
    m_buttons.push_back(new_scenario);
    m_buttons.push_back(add_motion);
    m_buttons.push_back(del_motion);
    m_buttons.push_back(save_scenario_to_file);

    //Now define the layout using sizer.

    
    //First Horizontal 
    // Left motions and scenario List box + load buttons
    // Right canvas and scenario edition
    wxBoxSizer *sizer0 = new wxBoxSizer(wxHORIZONTAL);

    //Left part everything vertical here
    wxBoxSizer *sizer1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizer2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizer3 = new wxBoxSizer(wxVERTICAL);
    sizer2->Add(load_motion,0,wxEXPAND);
    sizer2->Add(m_motions_lb,1,wxEXPAND);
    sizer3->Add(load_scenario,0,wxEXPAND);
    sizer3->Add(m_scenarios_lb,0,wxEXPAND);
    sizer1->Add(sizer2,1,wxEXPAND);
    sizer1->Add(sizer3,0,wxEXPAND);
    sizer0->Add(sizer1,0,wxEXPAND);

    m_sizers.push_back(sizer0);
    m_sizers.push_back(sizer1);
    m_sizers.push_back(sizer2);
    m_sizers.push_back(sizer3);

    //Right part everything vertical here
    wxBoxSizer *sizer4 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizer5 = new wxBoxSizer(wxVERTICAL);
    wxGridSizer *sizer6 = new wxGridSizer(5);

    sizer4->Add(m_canvas,1,wxEXPAND);
    sizer5->Add(m_new_scenario,1,wxEXPAND);
    sizer6->Add(transfer_scenario_to_rcb);
    sizer6->Add(new_scenario);
    sizer6->Add(add_motion);
    sizer6->Add(del_motion);
    sizer6->Add(save_scenario_to_file);

    sizer5->Add(sizer6);
    sizer4->Add(sizer5,0,wxEXPAND);
    sizer0->Add(sizer4,1,wxEXPAND);

    m_sizers.push_back(sizer4);
    m_sizers.push_back(sizer5);
    m_sizers.push_back(sizer6);

    SetSizer(sizer0);
}

MyFrame::~MyFrame()
{
    std::list<wxSizer *>::iterator its = m_sizers.begin();
    while( its != m_sizers.end() )
    {
        delete *its;
        its++;
    }

    std::list<wxButton *>::iterator itb = m_buttons.begin();
    while( itb != m_buttons.end() )
    {
        delete *itb;
        itb++;
    }

    delete m_motions_lb;
    delete m_scenarios_lb;
    delete m_new_scenario;

    delete m_canvas;
}
// event handlers

void MyFrame::LoadMotion(wxCommandEvent& event)
{
    wxFileDialog loadDlg(NULL,"Loading Motion","","","*.RCB",wxFD_OPEN|wxFD_FILE_MUST_EXIST );

    if ( loadDlg.ShowModal() == wxID_OK )
    {
        wxString filename = loadDlg.GetFilename();
        m_simulator->load_motion((char *)filename.c_str(),m_simulator->get_next_motion_slot());
    }
}

void MyFrame::DoubleClickListBox(wxCommandEvent& event)
{
    int sel = event.GetInt();
    m_simulator->play_motion(sel);

    wxString str;
    str.Printf( wxT("Playing motion %d"), sel );
    SetStatusText( str );

}

void MyFrame::NewScenario(wxCommandEvent& event)
{
    m_new_scenario->Clear();
}

void MyFrame::AddMotion(wxCommandEvent& event)
{
    wxArrayInt motion_selections;
    int input_nb = m_motions_lb->GetSelections(motion_selections);
    if (input_nb == 0)
        return;
    wxString st = m_motions_lb->GetString(motion_selections[0]);

    wxArrayInt selections;
    int nb = m_new_scenario->GetSelections(selections);
    if ( nb == 0 )
        m_new_scenario->InsertItems(1,&st,m_new_scenario->GetCount());
    else
        m_new_scenario->InsertItems(1,&st,selections[0]);
}

void MyFrame::DelMotion(wxCommandEvent& event)
{
    wxArrayInt selections;
    int nb = m_new_scenario->GetSelections(selections);
    for(int i=nb-1;i>=0;i--)
        m_new_scenario->Delete(selections[i]);
}

void MyFrame::LoadScenario(wxCommandEvent& event)
{
    wxFileDialog loadDlg(NULL,"Loading Scenario","","","*.RCB",wxFD_OPEN|wxFD_FILE_MUST_EXIST );

    if ( loadDlg.ShowModal() == wxID_OK )
    {
        wxString filename = loadDlg.GetFilename();
        m_simulator->load_scenario((char *)filename.c_str(),m_simulator->get_next_scenario_slot());
    }
}


void MyFrame::DC_Scenario(wxCommandEvent& event)
{
    int sel = event.GetInt();
    m_simulator->play_scenario(sel);

    wxString str;
    str.Printf( wxT("Playing scenario %d"), sel );
    SetStatusText( str );
}
void MyFrame::PrepareDC(wxDC& dc)
{
    dc.SetLogicalOrigin( m_xLogicalOrigin, m_yLogicalOrigin );
    dc.SetAxisOrientation( !m_xAxisReversed, m_yAxisReversed );
    dc.SetUserScale( m_xUserScale, m_yUserScale );
    dc.SetMapMode( m_mapMode );
}
#endif // WX_INTERFACE

void KHR2_Interface::DisplayMotion(RCBMotion *m)
{
#ifdef WX_INTERFACE
    m_frame->m_canvas->DisplayMotion(m);
    m_frame->m_canvas->Refresh();
#endif
}
void KHR2_Interface::Update()
{
#ifdef WX_INTERFACE
    m_frame->m_canvas->Refresh();
#endif
}

void KHR2_Interface::ActivatePosition(RCBMotion *m,int pos)
{
#ifdef WX_INTERFACE
    m_frame->m_canvas->ActivatePosition(m,pos);
    m_frame->m_canvas->Refresh();
#endif
}

void KHR2_Interface::RegisterScenario(int index,char *name)
{
#ifdef WX_INTERFACE
    wxString st(name);
    m_frame->m_scenarios_lb->InsertItems(1,&st,index);
#endif
}

void KHR2_Interface::RegisterMotion(int index,char *name)
{
#ifdef WX_INTERFACE
    wxString st(name);
    m_frame->m_motions_lb->InsertItems(1,&st,index);
#endif
}

void KHR2_Interface::SetSimulator(rcb3_simulator *simulator)
{
#ifdef WX_INTERFACE
    m_frame->m_simulator = simulator;
#endif
}

#ifdef WX_INTERFACE
RCBMotion *MyFrame::StoreCreatedMotionToClass()
{
    RCBMotion *m = new RCBMotion;
   
    m->m_control = 0;
    m->m_item_count = m_new_scenario->GetCount();
    m->m_start = 0;
    m->m_link_count = m->m_item_count-1;
    m->m_type = 7;
    m->m_name = strdup("NONAME");

    for(unsigned int i=0;i<(unsigned int)m->m_item_count;i++)
    {
        //Build the Item.
        RCBMotionItem item;
        item.m_color = 0xFFFFFF;
        item.m_width = 40;
        item.m_height = 20;
        item.m_index = i;
        item.m_left = (i % 8) * 50;   //arrange them neatly on a grid.
        item.m_top = i/8 * 30 + 10;

        wxString name = m_new_scenario->GetString(i);
        item.m_name = strdup(name.c_str());

        int pos = m_motions_lb->FindString(name);
        if ( pos != wxNOT_FOUND )
            item.m_params.push_back(pos);
        else
            item.m_params.push_back(0);
        for(int j=0;j<23;j++)
            item.m_params.push_back(0);

        item.m_type = 7;
     
        m->m_items.push_back(item);
    }

    for(unsigned int i=0;i<(unsigned int)m->m_link_count;i++)
    {
        //Build the Item.
        RCBMotionLink link;

        link.m_index = i;
        link.m_origin = i;
        link.m_final = i + 1;
        link.m_main = 0;
        
        m->m_links.push_back(link);
    }

    return m;
}

void MyFrame::SaveScenarioToFile(wxCommandEvent& event)
{
    RCBMotion * s = StoreCreatedMotionToClass();
    wxFileDialog saveDlg(NULL,"Saving Scenario","","","*.RCB",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if ( saveDlg.ShowModal() == wxID_OK )
    {
        wxString filename = saveDlg.GetFilename();
        if ( RCBMotion::SaveToFile(s,(char *)filename.c_str()) )
        {
            wxMessageDialog msg(NULL,"File Saved.","All good",wxOK);
            msg.ShowModal();
        }
        else
        {
            wxMessageDialog msg(NULL,"CANNOT SAVE FILE!!","ERROR",wxICON_ERROR|wxOK);
            msg.ShowModal();
        }
    }
    delete s;
}

void MyFrame::SaveScenarioToSN(wxCommandEvent& event)
{
    RCBMotion * s = StoreCreatedMotionToClass();
    int n = m_simulator->get_next_scenario_slot();
    if ( n != -1 )
    {
        m_simulator->load_scenario(s,n);
        wxMessageDialog msg(NULL,"Scenario added","All good.",wxOK);
        msg.ShowModal();
    }
    else
    {
        delete s;
        wxMessageDialog msg(NULL,"NO SLOT AVAILABLE!!","WARNING",wxICON_ERROR|wxOK);
        msg.ShowModal();
    }
}

#endif // WX_INTERFACE
