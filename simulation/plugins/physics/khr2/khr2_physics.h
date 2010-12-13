/*

*/
#include <plugins/physics.h>
#include <vector>

class KHR2_Physics
{
public:
    KHR2_Physics(dWorldID worldID, dSpaceID spaceID,dJointGroupID jointID);
    ~KHR2_Physics();

    void Step();

private:

    class KMatrix34
    {
    public:
        dReal values[12];
    };

    dWorldID m_worldID;
    dSpaceID m_spaceID;
    dJointGroupID m_jointID;

    dBodyID m_bodyID;

    std::vector<dReal> m_previousRoll;
    std::vector<dReal> m_previousPitch;
    unsigned int m_buffer_size;
    unsigned int m_step;
};

