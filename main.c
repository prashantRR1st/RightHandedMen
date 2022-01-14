#include "platform.h"

#define TIME_SCALE (1000000)
#define HZ (60)
#define T (TIME_SCALE * 1.0f / HZ)
#define GRANULARITY (200) /*Minimum Allowable Sleep Time*/
#define SECOND (1)
#define MINUTE (SECOND * 60)
#define HOUR (MINUTE * 60)
#define MAX_DURATION (HOUR)

typedef struct Sensors
{
    float x;
} Sensors;

typedef struct State
{
    float x;
} State;

typedef struct Program
{
    int size;
    State input[MAX_DURATION];
} Program;

typedef struct SensorHistory
{
    int size;
    Sensors history[MAX_DURATION];
} SensorHistory;

/**
 * @brief Phases of operation
 * 
 */
typedef enum Phase
{
    CALIBRATION,
    PROGRAM,
    CLOSING
} Phase;

/**
 * @brief System struct
 * 
 */
typedef struct System
{
    Program program;
    SensorHistory history;
    State state;
    Sensors sensors;
    Phase phase;
    Time time;
} System;

/**
 * @brief System initialization
 * 
 * @param system 
 * @return int 0 is success
 */
int startup(System *system)
{
    system->program.size = 0;
    system->history.size = 0;
    system->phase = CALIBRATION;
    system->time = 0;
    initialize_sensors(&system->sensors);
    return 0;
}

/**
 * @brief System shutdown
 * Write all acutators to safe states
 * 
 * @return int 0 is success
 */
int shutdown()
{

    return 0;
}

/**
 * @brief Initialization and calibration of sensors
 * 
 * @param sensors 
 */
void initialize_sensors(Sensors *sensors)
{
    sensors = 0;
}

/**
 * @brief Polling/Processing of sensors
 * 
 * @param system 
 */
void process_sensors(System *system)
{
}

/**
 * @brief Control loop logic and writing to actuators
 * 
 * @param system 
 * @return int 
 */
int control_logic(System *system)
{
    system->state; // x+ = Ax + Bu
                   // y  = Cx + Du
    return 1;
}

/**
 * @brief Logic for handling different phases of the system (state machine)
 * 
 * @param system 
 * @return int 
 */
int handle_phase_machine(System *system)
{
    switch (system->phase)
    {
    case CALIBRATION:
    {
    }
    break;
    case PROGRAM:
    {
    }
    break;
    case CLOSING:
    {
    }
    break;
    }

    return 1;
}

/**
 * @brief PANIC AND SHOOT THE SHERRIF
 * 
 */
void handle_missed_frame()
{
}

/**
 * @brief Mission failed
 * 
 * @param success 
 * @return int 
 */
int handle_startup_failure(int success)
{
    return 0;
}

/**
 * @brief Cant even shutdown correctly, kill me
 * 
 * @param success 
 * @return int 
 */
int handle_shutdown_failure(int success)
{
    return 0;
}

/**
 * @brief pain
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int *argc, char **argv)
{
    System system = {0};
    int success = startup(&system); // 0 is success

    if (!success)
    {

        int loop = 1;
        Time delta, work, prev = timer();
        while (loop)
        {

            process_sensors(&system);
            loop = control_logic(&system);
            loop = handle_phase_machine(&system);
            
            work = timer();
            Time elapsed = work;
            if (loop && ((delta = (elapsed - prev)) < (T - GRANULARITY)))
            {

                sleep(delta); //shhhhhhhhhhhhhhhhh

                while (loop && ((delta = (elapsed - prev)) < T))
                {
                    elapsed = timer();
                }
            }
            else
            {
                handle_missed_frame();
            }
            //Log delta and work
            prev = elapsed;
        }
        return 0;
    }
    else
    {
        success = handle_startup_failure(success);
    }

    success = shutdown(); // 0 is success
    while (!success)
    {
        success = handle_shutdown_failure(success);
    }

    return success;
}