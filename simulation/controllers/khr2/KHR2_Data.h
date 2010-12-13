#ifndef __KHR2_Data_h
#define __KHR2_Data_h

#include <list>
#include <vector>

#define JOINT_COUNT 17
#define CHANNEL_COUNT_RCB3 24
#define NO_MOTION_INDEX 1000
#define AD_INPUT_COUNT 3

class RCBMotionLink
{
public:
    int m_index;
    int m_origin;
    int m_final;
    int m_main;
    std::vector<int> m_points;
};

class RCBMotionItem
{
public:
    int m_index;
    int m_type;
    int m_width;
    int m_height;
    int m_left;
    int m_top;
    int m_color;
    char *m_name;
    std::list<int> m_params;
};


class RCBMotion
{
public:
    RCBMotion();

    static bool SaveToFile(RCBMotion *m,char *filename);
    int m_type;
    int m_start;
    int m_current_pos;
    int m_control;
    int m_item_count;
    int m_link_count;
    char *m_name;
    std::vector<RCBMotionItem> m_items;
    std::vector<RCBMotionLink> m_links;

    unsigned int GetNextItem(bool default_jump = true);
    
};

#endif // __KHR2_Data_h
