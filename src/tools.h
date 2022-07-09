#pragma once




namespace oar {
   
    void SleepMillis(long time) {
#ifdef WIN32
        _sleep(time);
#elif linux
        usleep(1000 * time);
#endif
    }
}