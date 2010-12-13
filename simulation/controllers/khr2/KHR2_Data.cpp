#include "KHR2_Data.h"
#include <stdio.h>

bool RCBMotion::SaveToFile(RCBMotion *m,char *filename)
{
    FILE *f = fopen(filename,"wt");

    if ( f == NULL )
        return false;

    fprintf(f,"[GraphicalEdit]\n");
    fprintf(f,"Type=%d\n",m->m_type);
    fprintf(f,"Width=500\n");
    fprintf(f,"Height=%d\n",30*((m->m_item_count/8) + 1));
    fprintf(f,"Items=%d\n",m->m_item_count);
    fprintf(f,"Links=%d\n",m->m_link_count);
    fprintf(f,"Start=%d\n",m->m_start);
    fprintf(f,"Name=%s\n",m->m_name);
    fprintf(f,"Port=0\n");
    fprintf(f,"Ctrl=%d\n",m->m_control);
    fprintf(f,"\n");

    for(int i=0;i<m->m_item_count;i++)
    {
        const RCBMotionItem &item = m->m_items[i];
        fprintf(f,"[Item%d]\n",i);
        fprintf(f,"Name=%s\n",item.m_name);
        fprintf(f,"Width=%d\n",item.m_width);
        fprintf(f,"Height=%d\n",item.m_height);
        fprintf(f,"Left=%d\n",item.m_left);
        fprintf(f,"Top=%d\n",item.m_top);
        fprintf(f,"Color=%d\n",item.m_color);
        fprintf(f,"Type=%d\n",item.m_type);
        fprintf(f,"Prm=");
        std::list<int>::const_iterator it = item.m_params.begin();
        for(int j=0;j<24;j++,it++)
        {
            fprintf(f,"%d",*it);
            if ( j != 23 )
                fprintf(f,",");
            else
                fprintf(f,"\n");
        }
        fprintf(f,"\n");
    }


    for(int i=0;i<m->m_link_count;i++)
    {
        const RCBMotionLink &item = m->m_links[i];
        fprintf(f,"[Link%d]\n",i);
        fprintf(f,"Main=%d\n",item.m_main);
        fprintf(f,"Origin=%d\n",item.m_origin);
        fprintf(f,"Final=%d\n",item.m_final);
        fprintf(f,"Point=");
        for(unsigned int j=0;j<item.m_points.size();j++)
        {
            fprintf(f,"%d",item.m_points[j]);
            if ( j != item.m_points.size()-1 )
                fprintf(f,",");                
        }
        fprintf(f,"\n");
        fprintf(f,"\n");
    }

    fclose(f);
    return true;
}
