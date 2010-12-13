
#include <webots/robot.h>
#include <webots/servo.h>
#include <webots/supervisor.h>
#include <webots/receiver.h>

#include <vector>
#include <list>
#include "KHR2_Data.h"

class KHR2_Interface;


class rcb3_simulator
{
public:

    rcb3_simulator();
    ~rcb3_simulator();

    void init();
    
    void play_motion(unsigned int store_index);
    void play_scenario(unsigned int store_index);
    void run();

    bool load_motion(const char *filename,unsigned int store_index);
    bool load_scenario(const char *filename,unsigned int store_index);

    bool load_motion(RCBMotion *m,unsigned int store_index);
    bool load_scenario(RCBMotion *s,unsigned int store_index);

    int get_next_scenario_slot();
    int get_next_motion_slot();
private:
    //GUI
    void create_motion_widgets();
    void activate_widget(int sc_pos,int m_pos);

    //RCB3
    bool apply_mixing(bool realtime);
    void fill_mixing_data(int channel,int ad,int mixing_value,bool realtime);
    void get_ad_values();
    bool read_rcb_file(const char *filename,RCBMotion &motion);
    int convert_to_remote_code(int key);
    void search_key_activated_motion(int key);
    
    WbDeviceTag m_joint[JOINT_COUNT];     /* all the servos */
    WbDeviceTag m_accelerometerTag;
    WbDeviceTag m_gyroRollTag;
    WbDeviceTag m_gyroPitchTag;

    float m_joint_position[CHANNEL_COUNT_RCB3];
    float m_desired_joint_position[CHANNEL_COUNT_RCB3];

    const char *m_robot_name;    
    std::vector<RCBMotion *> m_motions;
    std::vector<RCBMotion *> m_scenarios;

    //RCB3 Internal data
    unsigned int m_current_scenario;
    unsigned int m_current_scenario_position;
    unsigned int m_current_motion;
    int m_loop_counter;
    int m_compare_register;
    int m_ad[AD_INPUT_COUNT];
    int m_button;
    bool m_run_current_motion_only;

    bool m_ad_mixing[CHANNEL_COUNT_RCB3][AD_INPUT_COUNT];
    int m_ad_mixing_factor[CHANNEL_COUNT_RCB3][AD_INPUT_COUNT];
    int m_motion_mixing_count;    

    int m_ad_origin[AD_INPUT_COUNT];

    bool m_realtime_ad_mixing[CHANNEL_COUNT_RCB3][AD_INPUT_COUNT];
    int  m_realtime_ad_mixing_factor[CHANNEL_COUNT_RCB3][AD_INPUT_COUNT];
    int  m_realtime_mixing_count;

    int m_simulation_delay;

    //WXGUI
    KHR2_Interface *m_window;
    RCBMotion *m_displayed_motion;
public:
    void SetInterface(KHR2_Interface *window);
};

