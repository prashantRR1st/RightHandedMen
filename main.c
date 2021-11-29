#define TIME_SCALE (1000000)
#define HZ (60)
#define T (TIME_SCALE * 1.0f / HZ)
#define GRANULARITY (200) /*Minimum Allowable Sleep Time*/

typedef struct Sensors
{
    float *data;
} Sensors;

typedef struct System
{
    Sensors *sensors;
} System;

typedef int Time;

// System time
int timer()
{

    return 0;
}

// OS/MCU call to sleep
void sleep()
{
}

// What needs to occur on system startup
int startup(System *system)
{
    initialize_sensors(system->sensors);
    return 0;
}

// What needs to occur on system shutdown
int shutdown()
{

    return 0;
}

void initialize_sensors(float *sensors)
{
}

void process_sensors(System *system)
{
}

int control_logic(System *system)
{
    return 0;
}

void handle_missed_frame()
{
}

int handle_startup_failure(int success)
{
    return 0;
}

int handle_shutdown_failure(int success)
{
    return 0;
}

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