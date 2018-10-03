#ifndef __DRAW_H__
#define __DRAW_H__

// defines drawing functions
//

void draw_bg(bool button);

void draw_titlebar(short status);

void draw_latlong(long lat, long lon);

void draw_speed(unsigned int speed);

void draw_date(short daymonth, short hourminute);


#endif 
