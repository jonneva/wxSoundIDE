/***************************************************************
 * Name:      wxSoundIDEMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Jonne Valola (jonnev@gmail.com)
 * Created:   2014-10-13
 * Copyright: Jonne Valola ()
 * License:
 **************************************************************/

#include "wxSoundIDEMain.h"
#include <wx/msgdlg.h>
#include "led.h"
#include "Synth.h"
#include "oscillator.h"

//(*InternalHeaders(wxSoundIDEFrame)
#include <wx/settings.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//Global variables
uint32_t playhead = 0;
wxPoint A0Pos;
uint16_t scopew = 32727;
int16_t attax, decayx, sustx, relex;
uint8_t updateadsr = false;

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(wxSoundIDEFrame)
const long wxSoundIDEFrame::ID_BUTTON3 = wxNewId();
const long wxSoundIDEFrame::ID_BUTTON11 = wxNewId();
const long wxSoundIDEFrame::ID_BUTTON9 = wxNewId();
const long wxSoundIDEFrame::ID_BUTTON5 = wxNewId();
const long wxSoundIDEFrame::ID_BUTTON4 = wxNewId();
const long wxSoundIDEFrame::ID_BUTTON1 = wxNewId();
const long wxSoundIDEFrame::ID_CUSTOM1 = wxNewId();
const long wxSoundIDEFrame::ID_PANEL1 = wxNewId();
const long wxSoundIDEFrame::ID_SLIDER1 = wxNewId();
const long wxSoundIDEFrame::ID_STATICTEXT3 = wxNewId();
const long wxSoundIDEFrame::ID_STATICTEXT4 = wxNewId();
const long wxSoundIDEFrame::ID_SLIDER3 = wxNewId();
const long wxSoundIDEFrame::ID_SLIDER2 = wxNewId();
const long wxSoundIDEFrame::ID_RADIOBOX1 = wxNewId();
const long wxSoundIDEFrame::ID_BUTTON6 = wxNewId();
const long wxSoundIDEFrame::ID_LED2 = wxNewId();
const long wxSoundIDEFrame::ID_PANEL3 = wxNewId();
const long wxSoundIDEFrame::ID_BUTTON7 = wxNewId();
const long wxSoundIDEFrame::ID_LED3 = wxNewId();
const long wxSoundIDEFrame::ID_PANEL4 = wxNewId();
const long wxSoundIDEFrame::ID_BUTTON2 = wxNewId();
const long wxSoundIDEFrame::ID_LED1 = wxNewId();
const long wxSoundIDEFrame::ID_PANEL2 = wxNewId();
const long wxSoundIDEFrame::ID_CHECKBOX1 = wxNewId();
const long wxSoundIDEFrame::ID_CHECKBOX2 = wxNewId();
const long wxSoundIDEFrame::ID_STATICTEXT1 = wxNewId();
const long wxSoundIDEFrame::ID_BUTTON8 = wxNewId();
const long wxSoundIDEFrame::idMenuQuit = wxNewId();
const long wxSoundIDEFrame::idMenuAbout = wxNewId();
const long wxSoundIDEFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxSoundIDEFrame,wxFrame)
    //(*EventTable(wxSoundIDEFrame)
    //*)
END_EVENT_TABLE()

wxSoundIDEFrame::wxSoundIDEFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(wxSoundIDEFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, _("wxSoundIde - Copyright 2014 Jonne Valola"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(800,589));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(144,80), wxSize(632,152), 0, _T("ID_PANEL1"));
    Panel1->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));
    Decay = new MovableButton(Panel1, ID_BUTTON3, _("D"), wxPoint(56,48), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    Decay->SetMinSize(wxSize(20,20));
    Decay->SetMaxSize(wxSize(0,0));
    P2 = new MovableButton(Panel1, ID_BUTTON11, _("2"), wxPoint(96,56), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON11"));
    P2->SetMinSize(wxSize(20,20));
    P2->SetMaxSize(wxSize(0,0));
    P2->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
    P2->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));
    P1 = new MovableButton(Panel1, ID_BUTTON9, _("1"), wxPoint(80,16), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
    P1->SetMinSize(wxSize(20,20));
    P1->SetMaxSize(wxSize(0,0));
    P1->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
    P1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));
    Rele = new MovableButton(Panel1, ID_BUTTON5, _("R"), wxPoint(208,24), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    Rele->SetMinSize(wxSize(20,20));
    Rele->SetMaxSize(wxSize(0,0));
    Sust = new MovableButton(Panel1, ID_BUTTON4, _("S"), wxPoint(144,72), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    Sust->SetMinSize(wxSize(20,20));
    Sust->SetMaxSize(wxSize(0,0));
    A0 = new MovableButton(Panel1, ID_BUTTON1, _("A"), wxPoint(64,88), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    A0->SetMinSize(wxSize(20,20));
    A0->SetMaxSize(wxSize(0,0));
    Osc1DC = new wxClientDC(Panel1);
    VolSlider1 = new wxSlider(this, ID_SLIDER1, 255, 0, 255, wxPoint(144,48), wxSize(144,24), wxSL_BOTH, wxDefaultValidator, _T("ID_SLIDER1"));
    VolSlider1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    VolLabel1 = new wxStaticText(this, ID_STATICTEXT3, _("Volume: 255"), wxPoint(144,24), wxSize(72,13), wxALIGN_LEFT, _T("ID_STATICTEXT3"));
    PitchLabel1 = new wxStaticText(this, ID_STATICTEXT4, _("Pitch: 100"), wxPoint(304,24), wxSize(56,13), wxALIGN_LEFT, _T("ID_STATICTEXT4"));
    PitchSlider1 = new wxSlider(this, ID_SLIDER3, 100, 1, 1000, wxPoint(296,48), wxSize(488,24), wxSL_BOTH, wxDefaultValidator, _T("ID_SLIDER3"));
    LengthSlider1 = new wxSlider(this, ID_SLIDER2, 10000, 1, 32727, wxPoint(144,240), wxSize(632,24), wxSL_BOTH, wxDefaultValidator, _T("ID_SLIDER2"));
    wxString __wxRadioBoxChoices_1[6] =
    {
    	_("OFF"),
    	_("SQUARE"),
    	_("SAW"),
    	_("TRIANGLE"),
    	_("NOISE"),
    	_("SAMPLE")
    };
    RadioBox1 = new wxRadioBox(this, ID_RADIOBOX1, _("Oscillator 1"), wxPoint(32,88), wxDefaultSize, 6, __wxRadioBoxChoices_1, 1, wxNO_BORDER, wxDefaultValidator, _T("ID_RADIOBOX1"));
    Panel2 = new wxPanel(this, ID_PANEL3, wxPoint(8,48), wxSize(104,24), wxNO_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    Panel2->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    PWMBtn = new wxButton(Panel2, ID_BUTTON6, _("PWM"), wxPoint(24,0), wxSize(80,24), 0, wxDefaultValidator, _T("ID_BUTTON6"));
    PWMLed = new wxLed(Panel2,ID_LED2,wxColour(128,128,128),wxColour(11,125,181),wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE),wxDefaultPosition,wxDefaultSize);
    PWMLed->SwitchOn();
    Panel3 = new wxPanel(this, ID_PANEL4, wxPoint(8,16), wxSize(104,24), wxNO_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    Panel3->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    Play = new wxButton(Panel3, ID_BUTTON7, _("PLAY"), wxPoint(24,0), wxSize(80,24), 0, wxDefaultValidator, _T("ID_BUTTON7"));
    PlayLed = new wxLed(Panel3,ID_LED3,wxColour(128,128,128),wxColour(11,125,181),wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE),wxDefaultPosition,wxDefaultSize);
    PlayLed->SwitchOff();
    Osc1BtnPanel = new wxPanel(this, ID_PANEL2, wxPoint(8,256), wxSize(104,24), wxNO_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    Osc1BtnPanel->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    Loop = new wxButton(Osc1BtnPanel, ID_BUTTON2, _("LOOP"), wxPoint(24,0), wxSize(80,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    LoopLed = new wxLed(Osc1BtnPanel,ID_LED1,wxColour(128,128,128),wxColour(11,125,181),wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE),wxDefaultPosition,wxDefaultSize);
    LoopLed->SwitchOn();
    ADSRCheckBox = new wxCheckBox(this, ID_CHECKBOX1, _("ADSR"), wxPoint(224,24), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    ADSRCheckBox->SetValue(false);
    PitchCheckBox = new wxCheckBox(this, ID_CHECKBOX2, _("Envelope"), wxPoint(368,24), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    PitchCheckBox->SetValue(false);
    LengthLabel = new wxStaticText(this, ID_STATICTEXT1, _("Length: 0 sec"), wxPoint(152,272), wxSize(104,16), 0, _T("ID_STATICTEXT1"));
    OutputFile = new wxButton(this, ID_BUTTON8, _("ADSR to FILE"), wxPoint(32,296), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON8"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Panel1->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxSoundIDEFrame::OnPanel1Paint1,0,this);
    Connect(ID_SLIDER1,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&wxSoundIDEFrame::OnVolSlider1CmdSliderUpdated);
    Connect(ID_SLIDER3,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&wxSoundIDEFrame::OnPitchSlider1CmdSliderUpdated);
    Connect(ID_SLIDER2,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&wxSoundIDEFrame::OnLengthSlider1CmdSliderUpdated);
    Connect(ID_RADIOBOX1,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&wxSoundIDEFrame::OnRadioBox1Select);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxSoundIDEFrame::OnPWMBtnClick);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxSoundIDEFrame::OnPlayClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxSoundIDEFrame::OnLoopClick);
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&wxSoundIDEFrame::OnADSRCheckBoxClick);
    Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&wxSoundIDEFrame::OnPitchCheckBoxClick);
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxSoundIDEFrame::OnOutputFileClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxSoundIDEFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxSoundIDEFrame::OnAbout);
    //*)

    A0->SetMinSize(wxSize(20,20));
    A0->SetMaxSize(wxSize(0,0));
    A0->SetLabel(_("A")); A0->SetSize(20,20);
    A0->SetPos(1300,255);
    Decay->SetMinSize(wxSize(20,20));
    Decay->SetMaxSize(wxSize(0,0));
    Decay->SetLabel(_("D")); Decay->SetSize(20,20);
    Decay->SetPos(2600,127);
    Sust->SetMinSize(wxSize(20,20));
    Sust->SetMaxSize(wxSize(0,0));
    Sust->SetLabel(_("S")); Sust->SetSize(20,20);
    Sust->SetPos(3900,127);
    Rele->SetMinSize(wxSize(20,20));
    Rele->SetMaxSize(wxSize(0,0));
    Rele->SetLabel(_("R")); Rele->SetSize(20,20);
    Rele->SetPos(10000,0);
    P1->SetMinSize(wxSize(20,20));
    P1->SetMaxSize(wxSize(0,0));
    P1->SetLabel(_("1"));
    P1->SetSize(20,20);
    P1->SetPos(0,127);
    P2->SetMinSize(wxSize(20,20));
    P2->SetMaxSize(wxSize(0,0));
    P2->SetLabel(_("2"));
    P2->SetSize(20,20);
    P2->SetPos(scopew/2,127);
    timer = new wxTimer(this, 3);
    Connect(wxEVT_TIMER, wxTimerEventHandler(wxSoundIDEFrame::OnTimer));
    Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(wxSoundIDEFrame::OnCloseWindow));
}

wxSoundIDEFrame::~wxSoundIDEFrame()
{
    //(*Destroy(wxSoundIDEFrame)

    //*)
}

void wxSoundIDEFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void wxSoundIDEFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void wxSoundIDEFrame::OnRadioBox1Select(wxCommandEvent& event)
{
    setOSC(&osc1,osc1.on,RadioBox1->GetSelection(),PitchSlider1->GetValue(),VolSlider1->GetValue());
}

void wxSoundIDEFrame::OnVolSlider1CmdSliderUpdated(wxScrollEvent& event)
{
    wxString newlabel;
    newlabel << "Volume: " << VolSlider1->GetValue();
    VolLabel1->SetLabelText(newlabel);
    setOSC(&osc1,osc1.on,osc1.wave,PitchSlider1->GetValue(),VolSlider1->GetValue());
}

void wxSoundIDEFrame::OnPitchSlider1CmdSliderUpdated(wxScrollEvent& event)
{
    wxString newlabel;
    newlabel << "Pitch: " << PitchSlider1->GetValue();
    PitchLabel1->SetLabelText(newlabel);
    //setPitch(PitchSlider1->GetValue());
    setOSC(&osc1,osc1.on,osc1.wave,PitchSlider1->GetValue(),VolSlider1->GetValue());
}

void wxSoundIDEFrame::OnLengthSlider1CmdSliderUpdated(wxScrollEvent& event)
{
    wxString newlabel;
    newlabel << "Length: " << LengthSlider1->GetValue() << " ticks, " << (float(LengthSlider1->GetValue())*PWMLEVELS/SAMPLE_RATE) << " sec";
    LengthLabel->SetLabelText(newlabel);

    if (patch.playing) {
            timer->Stop();
            stopSound();
            PlayLed->SwitchOn();
            playSound(LoopLed->IsOn(), LengthSlider1->GetValue());
            timer->Start();
    }
}


void wxSoundIDEFrame::OnLoopClick(wxCommandEvent& event)
{
    LoopLed->Switch();
    patch.loop = LoopLed->IsOn();
}

BEGIN_EVENT_TABLE(MovableButton,wxButton)
EVT_LEFT_DOWN(MovableButton::onMouseDown)
EVT_LEFT_UP(MovableButton::onMouseUp)
EVT_MOTION(MovableButton::onMove)
END_EVENT_TABLE()

void wxSoundIDEFrame::OnPanel1Paint1(wxPaintEvent& event)
{
    UpdateScope();
}

void wxSoundIDEFrame::OnPWMBtnClick(wxCommandEvent& event)
{
    PWMLed->Switch();
    PWMemulation = PWMLed->IsOn();
}


void wxSoundIDEFrame::OnPlayClick(wxCommandEvent& event)
{
    timer->Stop();

    if (!patch.playing) {
            PlayLed->SwitchOn();
            playSound(LoopLed->IsOn(), LengthSlider1->GetValue());
            timer->Start();
    }
    else {
            PlayLed->SwitchOff();
            stopSound();
    }
}

void wxSoundIDEFrame::OnTimer(wxTimerEvent& event)
{
	//timer->Stop();
	UpdateScope();

    if (updateadsr) {
        UpdateADSR();
        updateadsr = false;
    }

    if (patch.playing) {
            PlayLed->Switch();

            //timer->Start();
    } else {
            PlayLed->SwitchOff();
        }
}

void wxSoundIDEFrame::UpdateScope()
{
    int w,h;
    w = Osc1DC->GetSize().GetWidth();
    h = Osc1DC->GetSize().GetHeight();
    uint16_t prevplayhead = playhead;
    playhead = (w*patch.count)/(32727-22)+11;
    Osc1DC->SetPen(wxPen(*wxWHITE_PEN));
    Osc1DC->SetBrush(wxBrush(wxColour(255,255,255),wxBRUSHSTYLE_SOLID));
    Osc1DC->DrawRectangle(0,0,w,h);
    //Osc1DC->SetBrush(wxBrush(wxColour(220,220,220),wxBRUSHSTYLE_SOLID));
    Osc1DC->SetPen(wxPen(*wxGREY_PEN));
    Osc1DC->DrawRectangle(10,10,w-20,h-20);
    Osc1DC->SetPen(wxPen(*wxRED_PEN));
    Osc1DC->DrawLine(P1->GetPosition()+wxPoint(10,10),P2->GetPosition()+wxPoint(10,10));
    Osc1DC->SetPen(wxPen(*wxBLACK_PEN));
    Osc1DC->DrawLine(wxPoint(11,h-11),A0->GetPosition()+wxPoint(10,10));

    Osc1DC->DrawLine(A0->GetPosition()+wxPoint(10,10),Decay->GetPosition()+wxPoint(10,10));
    Osc1DC->DrawLine(Sust->GetPosition()+wxPoint(10,10),Decay->GetPosition()+wxPoint(10,10));
    Osc1DC->DrawLine(Sust->GetPosition()+wxPoint(10,10),Rele->GetPosition()+wxPoint(10,10));
    //Osc1DC->DrawLine(wxPoint(w-11,h-10),Rele->GetPosition()+wxPoint(10,10));
    if (patch.playing) {
        Osc1DC->SetPen(wxPen(*wxWHITE_PEN));
        Osc1DC->DrawLine(wxPoint(prevplayhead,h-11),wxPoint(prevplayhead,11));
        Osc1DC->SetPen(wxPen(*wxBLUE_PEN));
        Osc1DC->DrawLine(wxPoint(playhead,h-11),wxPoint(playhead,11));
        if (ADSRCheckBox->IsChecked()) VolSlider1->SetValue(osc1.vol>>8);
        if (PitchCheckBox->IsChecked()) PitchSlider1->SetValue(osc1.pitch);
    }
    attax = A0->GetPosition().x;
    decayx = Decay->GetPosition().x;
    sustx = Sust->GetPosition().x;
    relex = Rele->GetPosition().x;
}

void wxSoundIDEFrame::OnCloseWindow(wxCloseEvent& event)
{
    timer->Stop();
    killSound();
    this->Destroy();
}

void wxSoundIDEFrame::UpdateADSR()
{
    ADSR adsr;
    uint16_t w,h;
    w = Panel1->GetSize().GetWidth()-20;
    h = Panel1->GetSize().GetHeight()-20;
    adsr.on = ADSRCheckBox->IsChecked();
    if (patch.count == 0) osc1.adsr.level=0;
    adsr.Apos = float((A0->GetPosition().x))/w*scopew;
    adsr.Aval = 255-float(A0->GetPosition().y)/h*255;
    adsr.Dpos = float((Decay->GetPosition().x))/w*scopew;
    adsr.Dval = 255-float(Decay->GetPosition().y)/h*255;
    adsr.Spos = float((Sust->GetPosition().x))/w*scopew;
    adsr.Sval = 255-float(Sust->GetPosition().y)/h*255;
    adsr.Rpos = float((Rele->GetPosition().x))/w*scopew;
    adsr.Rval = 255-float(Rele->GetPosition().y)/h*255;
    setADSR(&osc1,adsr);
    //P2->Move(Rele->GetPosition().x, P2->GetPosition().y);
}

void wxSoundIDEFrame::UpdatePitchEnv()
{
    PITCHENV pitchenv;
    uint16_t w,h;
    w = Panel1->GetSize().GetWidth()-20;
    h = Panel1->GetSize().GetHeight()-20;
    pitchenv.on = PitchCheckBox->IsChecked();
    if (patch.count == 0) osc1.pitchenv.level=0;
    pitchenv.P1pos = float((P1->GetPosition().x))/w*scopew;
    pitchenv.P1val = 255-float(P1->GetPosition().y)/h*255;
    pitchenv.P2pos = float((P2->GetPosition().x))/w*scopew;
    pitchenv.P2val = 255-float(P2->GetPosition().y)/h*255;
    setPITCHENV(&osc1,pitchenv);
    //P2->Move(Rele->GetPosition().x, P2->GetPosition().y);
}

void wxSoundIDEFrame::OnADSRCheckBoxClick(wxCommandEvent& event)
{
    if (patch.playing) {
                timer->Stop();
                stopSound();
                UpdateADSR();
                osc1.vol = 0;
                timer->Start();
                playSound(LoopLed->IsOn(), LengthSlider1->GetValue());
        } else {
                UpdateADSR();
    }
}

void wxSoundIDEFrame::OnPitchCheckBoxClick(wxCommandEvent& event)
{
        if (patch.playing) {
                timer->Stop();
                stopSound();
                UpdatePitchEnv();
                timer->Start();
                playSound(LoopLed->IsOn(), LengthSlider1->GetValue());
        } else {
                UpdatePitchEnv();
    }
}

void wxSoundIDEFrame::OnOutputFileClick(wxCommandEvent& event)
{
    if (patch.playing) {
                timer->Stop();
                stopSound();
                outputADSR(LengthSlider1->GetValue());
        } else {
                outputADSR(LengthSlider1->GetValue());
    }
}


