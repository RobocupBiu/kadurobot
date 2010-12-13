#include "khr2_physics.h"


KHR2_Physics::KHR2_Physics(dWorldID worldID, dSpaceID spaceID,dJointGroupID jointID) :
    m_worldID(worldID),
    m_spaceID(spaceID),
    m_jointID(jointID)
{
    m_bodyID = dWebotsGetBodyFromDEF("KHR2_0");

    //Fix me
    m_buffer_size = 100;

    m_previousRoll.resize(m_buffer_size,0);
    m_previousPitch.resize(m_buffer_size,0);
    m_step = 0;
}

KHR2_Physics::~KHR2_Physics()
{
}

//We do not allow a rotation of more than 180deg per step.
//That might happen in reality but it would be in some exceptional cases.
//In the other cases, it's just a small rotation but crossing the boundary of M_PI/-M_PI
dReal check_quadrant_change(dReal d)
{
    if ( d > M_PI )
        d -= 2*M_PI;
    else if ( d < -M_PI )
        d += 2*M_PI;

    return d;
}

void get_roll_pitch_from_matrix(const dReal *matrix,dReal *roll,dReal *pitch)
{
    *roll  = atan2(matrix[9], matrix[10]);     //phi
    *pitch = asin(-matrix[8]);            //theta
    //dReal yaw1   = atan2(matrix[4], matrix[0]);      //greek Y
}


//Information for Gyro
//range: +-300deg/sec
//standby value: 1.35v
//sensitivity: 0.67mv / deg / sec
//linearity: +-550 %FS
//answer: 0.4Hz

dReal gyro_angle_to_voltage(dReal angle)
{
    dReal v = angle * 180.0f / M_PI;
    v *= (0.67f / 1000.0f); //scale it
    v += 1.35f; //Add standby value

    return v;
}

void KHR2_Physics::Step()
{
    if ( m_bodyID == NULL)
        return;

    const dReal *matrix = (dReal*) dBodyGetRotation(m_bodyID);

    if ( matrix )
    {
        //Channel 3 is the accelerometer measuring in the Z direction
        //Simulate it with just the orientation.    
        //Except that Z is Y in Webots..that's why we have a 5 here and not a 10.
        dReal accelerometerZ = matrix[5];
        dWebotsSend(3,(void *)&accelerometerZ,sizeof(dReal));
    }

    //Information for Gyro
    //range: +-300deg/sec
    //standby value: 1.35v
    //sensitivity: 0.67mv / deg / sec
    //linearity: +-550 %FS
    //answer: 0.4Hz

    //Calculate the Gyro values
    dReal roll1,pitch1;
    get_roll_pitch_from_matrix(matrix,&roll1,&pitch1);

    m_previousRoll[m_step] = roll1;
    m_previousPitch[m_step] = pitch1;

    int prev_val = m_step - 10;
    if ( prev_val < 0 )
        prev_val += m_buffer_size;

    m_step++;
    if ( m_step >= m_buffer_size )
        m_step = 0;


    dReal roll = 0;
    dReal pitch = 0;

    dReal roll0  = m_previousRoll[prev_val];
    dReal pitch0 = m_previousPitch[prev_val];
    //dReal yaw0   = atan2(prev_mat[4], prev_mat[0]);      //greek Y

    prev_val++;
    if ( prev_val >= (int)m_buffer_size )
        prev_val = 0;

    //Accumulate all the rotation that happened during the sampling period
    //We do that instead of checking directly betwen prev_val and m_step
    //to allow rotation of more than 180 deg.
    while(prev_val != (int)m_step )
    {
        dReal roll1  = m_previousRoll[prev_val];
        dReal pitch1 = m_previousPitch[prev_val];


        dReal d = check_quadrant_change(roll1 - roll0);
        roll += d;
        roll0 = roll1;

        d = check_quadrant_change(pitch1 - pitch0);
        pitch += d;
        pitch0 = pitch1;

        prev_val++;
        if ( prev_val >= (int)m_buffer_size )
            prev_val = 0;
    }

    //Channel 2 is Roll
    dReal roll_voltage = gyro_angle_to_voltage(roll);
    dWebotsSend(2,(void *)&roll_voltage,sizeof(dReal));

    //Channel 1 is Pitch
    dReal pitch_voltage = gyro_angle_to_voltage(pitch);
    dWebotsSend(1,(void *)&pitch_voltage,sizeof(dReal));

}
