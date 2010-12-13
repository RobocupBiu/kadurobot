/*
 * File:         khr2.c
 * Date:         October 30th, 2006
 * Description:  
 * Author:       Laurent Lessieux
 *
 * Copyright (c) 2006 Cyberbotics - www.cyberbotics.com
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rcb3_simulator.h"
#include "KHR2_Interface.h"

rcb3_simulator simulator;
KHR2_Interface *m_interface;

#ifdef WX_INTERFACE
class MyApp:public wxApp {
  public:
    virtual bool OnInit() {
        wxgui_main();

        return true;
    } 

  private:
    void wxgui_main();    

    DECLARE_EVENT_TABLE()
};

IMPLEMENT_APP_CONSOLE(MyApp)
    BEGIN_EVENT_TABLE(MyApp, wxApp)
    END_EVENT_TABLE()

#endif // WX_INTERFACE

const char *motion_files[] = {
    "2HV001RC_Walk_Forward.RCB",            // 0
    "2HV002RC_Step_Back.RCB",               // 1
    "2HV003RC_Walk_Left_Diag.RCB",          // 2
    "2HV004RC_Walk_Right_Diag.RCB",         // 3
    "2HV005RC_Side_Step_Left.RCB",          // 4
    "2HV006RC_Side_Step_Right.RCB",         // 5
    "2HV007RC_Turn_Left.RCB",               // 6
    "2HV008RC_Turn_Right.RCB",              // 7
    "2HV009AC_Standup_based_on_sensor.RCB", // 8
    "2HV010RC_StandUp_Face_Down.RCB",       // 9
    "2HV011RC_Bow.RCB",                     // 10
    "2HV012RC_Kick_Left.RCB",               // 11
    "2HV013RC_Kick_Right.RCB",              // 12
    "2HV014RC_Wheel_Front.RCB",             // 13
    "2HV015RC_Wheel_Back.RCB",              // 14
    "2HV016RC_Wheel_Left.RCB",              // 15
    "2HV017RC_Wheel_Right.RCB",             // 16
    "2HV018RC_PushUps.RCB",                 // 17
    "2HV019RC_Punch_Right.RCB",             // 18
    "2HV020RC_Punch_Left.RCB",              // 19
    "2HV021RC_Free_all.RCB",                // 20
    "2HV022RC_Happy.RCB",                   // 21
    "2HV023RC_Unhappy.RCB",                 // 22
    "2HV024RC_Startup2.RCB",                // 23
    NULL
};

#ifdef WX_INTERFACE
void MyApp::wxgui_main()
{
    wb_robot_init();
    m_interface = new KHR2_Interface();
    simulator.SetInterface(m_interface);
    SetTopWindow(m_interface->GetWindow());

    for(int i=0;i<24;i++)
        simulator.load_motion(motion_files[i],i);

    simulator.load_scenario("demo_all.RCB",0);
    //simulator.load_motion("2HV000_StartupMotion.RCB",79);
    simulator.init();

    
    /* We wait a little bit before starting. */
    robot_step(1500);
    m_interface->Refresh();
    simulator.play_motion(23);
    m_interface->Refresh();
    simulator.play_motion(17);
    m_interface->Refresh();
    simulator.play_motion(22);
    m_interface->Refresh();
    simulator.play_motion(13);
    m_interface->Refresh();
    

    for (;;) {
      simulator.run();
      m_interface->Refresh();
      wb_robot_step(5);
    }
}
#else // WX_INTERFACE
int main(int argc,char *argv[]) {
  wb_robot_init();
  simulator.init();
  m_interface = new KHR2_Interface();
  simulator.SetInterface(m_interface);
  for(int i=0;i<24;i++) simulator.load_motion(motion_files[i],i);
  simulator.load_scenario("demo_all.RCB",0);
  //simulator.play_motion(23);
  printf("Hello, my name is KHR-3HV!\n");
  //simulator.play_motion(21);
  //printf("I can walk\n");
  //simulator.play_motion(0);
  //printf("I can do some gym\n");
  //wb_robot_step(1000); // wait 1 second to stabilize
  //simulator.play_motion(13);
  //simulator.play_motion(0);
  printf("I am yours\n\n");
  //simulator.play_motion(10);
  static const char *commands[] ={
    "Select the main window and use the keyboard:\n",
    "7 = Kick Left, 9 = Kick Right, 4 = Punch Left, 6 = Punch Right\n",
    "1 = Zannen (deception), 2 = Pushups, 3 = Happy, 8 = Bow\n",
    "R = Walk foward, V = Walk backwards\n",
    "C = Turn Left, B = Turn Right\n",
    "E = Walk Diag Left, T = Walk Diag Right, D = Side Step Left, G = Side Step Right\n",
    "Alt-R = Stand Up (from face down or up), Ctrl-V = Stand up (face down)\n",
    "Shift-8 = Forward Wheel, Shift-2 = Backward Wheel\n",
    "Shift-4 = Left Wheel, Shift-6 = Right Wheel\n",
    NULL
  };
  for(int c=0;commands[c]!=NULL;c++)
    printf("%s",commands[c]);

  for (;;) {
    simulator.run();
    m_interface->Refresh();
    wb_robot_step(5);
  }

  return 0;
}
#endif // WX_INTERFACE
