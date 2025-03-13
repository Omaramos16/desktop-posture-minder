#ifndef CLOCK_FACE_H
#define CLOCK_FACE_H

#include <lvgl.h>

// Function to create the clock face
void create_clock_face();

// Function to update the clock time
void update_time(lv_timer_t *timer);

#endif