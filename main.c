#define TIME_SCALE (1000000)
#define HZ (60)
#define T (TIME_SCALE * 1.0f / HZ)
#define GRANULARITY (200) /*Minimum Allowable Sleep Time*/

int timer()
{

    return 0;
}

void sleep()
{
}

int startup()
{

    return 0;
}

int shutdown()
{

    return 0;
}

void initialize_sensors(float *sensors)
{
}

void process_sensors(float *sensors)
{
}

int control_logic(float *sensors)
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

    int success = startup(); // 0 is success

    if (!success)
    {
        float *sensors;
        initialize_sensors(sensors);
        int loop = 1;
        int delta, curr, prev = timer();
        while (loop)
        {

            process_sensors(sensors);
            loop = control_logic(sensors);

            curr = timer();
            if (loop && ((delta = (curr - prev)) < (T - GRANULARITY)))
                sleep(delta);
            else
                handle_missed_frame();
            prev = curr;
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