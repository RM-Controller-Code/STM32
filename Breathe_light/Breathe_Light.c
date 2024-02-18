#include "Breathe_Light.h"

static void aRGB_led_show(uint32_t RGB)
{
    double alpha;
    double red,green,blue;
//    alpha = (aRGB & 0xFF000000) >> 24;
    red = ((RGB & 0x00FF0000) >> 16);
    green = ((RGB & 0x0000FF00) >> 8);
    blue = ((RGB & 0x000000FF) >> 0);

    // ��ɫͨ����ֵ(0-10000) ��Ҫ������ֵת��
    htim5.Instance->CCR1=(blue/255)*PWM_MAX;
    htim5.Instance->CCR2=(green/255)*PWM_MAX;
    htim5.Instance->CCR3=(red/255)*PWM_MAX;
}
// ģʽ(DOWN UP) Select_Colour��ɫ(RED GREEN BLUE)
/*************************
 * breathe_light
 * @param Mode DOWN:�������� UP:��������
 * @param Select_Colour
 *          0xrgb ��:0x111 0x000 �ֱ��ӦRGB  0:���� 1:�ı�
 * ʵ�ֺ�����
 */
void breathe_light(int Mode,uint32_t Select_Colour) {
    double delta = 255.0/RGB_FLOW_COLOR_CHANGE_TIME;    // ����
    double R, G, B;
    // ��ȡaRGB����
    R = ((aRGB & 0x00FF0000) >> 16);
    G = ((aRGB & 0x0000FF00) >> 8);
    B = ((aRGB & 0x000000FF) >> 0);

    // �����ɫ
    uint8_t Select_RED,Select_GREEN,Select_BLUE;
    Select_RED = (Select_Colour & 0xF00) >> 8;
    Select_GREEN = (Select_Colour & 0x0F0) >> 4;
    Select_BLUE = (Select_Colour & 0x00F) >> 0;
    for (int i = 0; i < RGB_FLOW_COLOR_CHANGE_TIME; ++i) {
        if(Select_RED == 1) R += Mode*delta;
        if(Select_GREEN == 1) G += Mode*delta;
        if(Select_BLUE == 1) B += Mode*delta;
        // �쳣����,����Ƿ�����
        if(R < 0) R = 0;
        if(G < 0) G = 0;
        if(B < 0) B = 0;
        aRGB = ((uint32_t)(0xFF)) << 24 | ((uint32_t)(R)) << 16 | ((uint32_t)(G)) << 8 | ((uint32_t)(B)) << 0;
        aRGB_led_show(aRGB);
        HAL_Delay(1);
    }
}