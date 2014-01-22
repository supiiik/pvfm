
#include <Streaming.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>
#include <SPI.h>


#include "pvfm_ui.h"
#include "pvfm_ui_dfs.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM);

void pvfm_ui::begin()
{
    TFT_BL_ON;          // turn on the background light
    Tft.TFTinit();      // init TFT library
    
    
    num_set = 0;
    
   /*
    int color_g = make_color(COLOR_TEMP_SET_R, COLOR_TEMP_SET_G, COLOR_TEMP_SET_B);
    Tft.fillRectangle(LOCA_TEMP_SET_X, LOCA_TEMP_SET_Y, 240, 64, color_g);
    
    color_g = make_color(COLOR_TEMP_NOW_R, COLOR_TEMP_NOW_G, COLOR_TEMP_NOW_B);
    Tft.fillRectangle(LOCA_TEMP_NOW_X, LOCA_TEMP_NOW_Y, 240, 64, color_g);
    
    color_g = make_color(COLOR_TIME1_R, COLOR_TIME1_G, COLOR_TIME1_B);
    Tft.fillRectangle(LOCA_TIME1_X, LOCA_TIME1_Y, 240, 64, color_g);
    
    color_g = make_color(COLOR_TIME2_R, COLOR_TIME2_G, COLOR_TIME2_B);
    Tft.fillRectangle(LOCA_TIME2_X, LOCA_TIME2_Y, 240, 64, color_g);
    
    color_g = make_color(COLOR_STATUS_R, COLOR_STATUS_G, COLOR_STATUS_B);
    Tft.fillRectangle(LOCA_STATUS_X, LOCA_STATUS_Y, 240, 64, color_g);
*/
}

unsigned int pvfm_ui::make_color(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int color_ = r;
    
    color_ = r>>3;                              // red
    color_ = color_<<6 | (g>>2);                // green
    color_ = color_<<5 | (b>>3);                // blue
    
    return color_;           
}

bool pvfm_ui::isTouch()
{
    Point p = ts.getPoint();
    if(p.z > __PRESURE)return 1;
    else return 0;
}

bool pvfm_ui::getTouchRect(int XL, int YU, int XR, int YD)
{
    if(!isTouch())return 0;
    Point p = ts.getPoint();
    
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
    
    if(p.z < __PRESURE)return 0;
    
    if(p.x > XL && p.x < XR && p.y > YU && p.y < YD)return 1;
    return 0;
}

unsigned char pvfm_ui::getTouchItem()
{
    if(!isTouch())return 0;
    
    Point p = ts.getPoint();
    
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
    
    if(p.z < __PRESURE)return 0;
    

    if(p.y < LOCA_TEMP_NOW_Y)
    {
        return 1;
    }
    else if(p.y < LOCA_TIME1_Y)
    {
        return 2;
    }
    else if(p.y < LOCA_TIME2_Y)
    {
        return 3;
    }
    else if(p.y < LOCA_STATUS_Y)
    {
        return 4;
    }
    else return 5;

}

void pvfm_ui::dispSetMode()
{
    int x_t  = 50;
    int y_t  = 120;
    
    int h_t  = 40;
    
    int len_r = 35;
    int wid_r = 40;
    
    unsigned int color_up   = make_color(213, 43, 43);
    unsigned int color_down = make_color(87, 170, 85);
    
    unsigned int color_bk   = make_color(162, 134, 94);
    
    Tft.fillRectangle(0, 0, 239, 319, color_bk);
    
    for(int i=0; i<h_t; i++)
    {
        Tft.drawHorizontalLine(x_t-i, y_t+i, 2*i+1, color_up);                      // up
        
        Tft.drawHorizontalLine(240-x_t-i, y_t+h_t+wid_r-i, 2*i+1, color_down);      // down
    }
    
    Tft.fillRectangle(x_t-(len_r/2), y_t+h_t, len_r, wid_r, color_up);              // up
    Tft.fillRectangle(240-x_t-len_r/2, y_t, len_r, wid_r, color_down);              // down
    
    int wid_ok  = 60;
    int fsize   = 4;
    
    Tft.fillRectangle(0, 320-wid_ok, 240, wid_ok, make_color(192, 192, 105));        // ok
    
    Tft.drawChar('O', 65, 320-wid_ok+(8*fsize/2),fsize, make_color(208, 47, 112));
    Tft.drawChar('K', 145, 320-wid_ok+(8*fsize/2),fsize, make_color(208, 47, 112));
    
    Tft.fillRectangle(0, 0, 240, 60, make_color(255, 255, 255));        // display

}


unsigned int pvfm_ui::setNum(int num_input)
{
    if(num_input < 0)return -1;
    
    unsigned long timer_tmp = millis();
    
    
    int num_buf = num_input+111;
    
    Tft.fillRectangle(0, 0, 240, 60, make_color(255, 255, 255));            // display
    dispNum(num_input, num_buf, SET_MODE_NUM_X, SET_MODE_NUM_Y, 4, RED, UI.make_color(255, 255, 255));
    
    while(1)
    {
        if(getTouchRect(10, 120, 90, 200))            // up
        {
            cout << "up" << endl;
            
            timer_tmp = millis();
            
            num_buf = num_input;
            num_input++;
            
            dispNum(num_input, num_buf, SET_MODE_NUM_X, SET_MODE_NUM_Y, 4, RED, UI.make_color(255, 255, 255));
            
            
            cout << num_input << endl;
            
            

            for(;;)
            {
                if(getTouchRect(10, 120, 90, 200))
                {
                    timer_tmp = millis();
                    delay(1);
                }
                
                
                if(millis()-timer_tmp > 100)break;
                
            }
        }
        else if(getTouchRect(150, 120, 230, 200))           // down
        {
            cout << "down" << endl;
            
            timer_tmp =  millis();
            //timer_tmp1 = millis();
            
            num_buf = num_input;
            num_input--;
            
            dispNum(num_input, num_buf, SET_MODE_NUM_X, SET_MODE_NUM_Y, 4, RED, UI.make_color(255, 255, 255));
            
            if(num_input < 0)num_input = 0;
            cout << num_input << endl;
            
            for(;;)
            {
                if(getTouchRect(150, 120, 230, 200))
                {
                    timer_tmp = millis();
                    delay(1);
                }
                
                if(millis()-timer_tmp > 50)break;
            }
        }
        else if(getTouchRect(0, 320-60, 239, 320))
        {
            cout << "return" << endl;
            
            timer_tmp = millis();

            for(;;)
            {
                if(getTouchRect(0, 320-60, 239, 320))
                {
                    timer_tmp = millis();
                    delay(1);
                }
                
                if(millis()-timer_tmp > 100)break;
            }
            
            return num_input;
        }
    }
}

void pvfm_ui::dispNum(int num, int num_buf, int x, int y, int size, int color, int color_bk)
{
    if(num > 1000)return;
    
    if(num_buf == -1)       // no buf
    {
        num_buf = num+111;
    }
    
    char tmp[5];
    
    int pow__[3] = {1, 10, 100};
    
    for(int i=0; i<3; i++)
    {
        if((num/pow__[2-i])%10 != (num_buf/pow__[2-i])%10)
        {
            Tft.drawNumber((num_buf/pow__[2-i])%10, x+i*size*FONT_SPACE, y, size, color_bk);
            Tft.drawNumber((num/pow__[2-i])%10, x+i*size*FONT_SPACE, y, size, color);
        }
    }
}


pvfm_ui UI;