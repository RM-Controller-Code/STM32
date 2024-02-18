#include "Breathe_Light.h"

static void aRGB_led_show(uint32_t RGB)
{
    double alpha;
    double red,green,blue;
//    alpha = (aRGB & 0xFF000000) >> 24;
    red = ((RGB & 0x00FF0000) >> 16);
    green = ((RGB & 0x0000FF00) >> 8);
    blue = ((RGB & 0x000000FF) >> 0);

    // 颜色通道赋值(0-10000) 需要进行数值转换
    htim5.Instance->CCR1=(blue/255)*PWM_MAX;
    htim5.Instance->CCR2=(green/255)*PWM_MAX;
    htim5.Instance->CCR3=(red/255)*PWM_MAX;
}
// 模式(DOWN UP) Select_Colour颜色(RED GREEN BLUE)
/*************************
 * breathe_light
 * @param Mode DOWN:降低亮度 UP:升高亮度
 * @param Select_Colour
 *          0xrgb 例:0x111 0x000 分别对应RGB  0:不变 1:改变
 * 实现呼吸灯
 */
void breathe_light(int Mode,uint32_t Select_Colour) {
    double delta = 255.0/RGB_FLOW_COLOR_CHANGE_TIME;    // 增量
    double R, G, B;
    // 读取aRGB数据
    R = ((aRGB & 0x00FF0000) >> 16);
    G = ((aRGB & 0x0000FF00) >> 8);
    B = ((aRGB & 0x000000FF) >> 0);

    // 混合颜色
    uint8_t Select_RED,Select_GREEN,Select_BLUE;
    Select_RED = (Select_Colour & 0xF00) >> 8;
    Select_GREEN = (Select_Colour & 0x0F0) >> 4;
    Select_BLUE = (Select_Colour & 0x00F) >> 0;
    for (int i = 0; i < RGB_FLOW_COLOR_CHANGE_TIME; ++i) {
        if(Select_RED == 1) R += Mode*delta;
        if(Select_GREEN == 1) G += Mode*delta;
        if(Select_BLUE == 1) B += Mode*delta;
        // 异常处理,处理非法操作
        if(R < 0) R = 0;
        if(G < 0) G = 0;
        if(B < 0) B = 0;
        aRGB = ((uint32_t)(0xFF)) << 24 | ((uint32_t)(R)) << 16 | ((uint32_t)(G)) << 8 | ((uint32_t)(B)) << 0;
        aRGB_led_show(aRGB);
        HAL_Delay(1);
    }
}