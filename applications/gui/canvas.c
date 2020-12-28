#include "canvas_i.h"
#include "icon.h"
#include "icon_i.h"

#include <flipper.h>
#include <flipper_v2.h>

struct Canvas {
    u8g2_t fb;
    uint8_t offset_x;
    uint8_t offset_y;
    uint8_t width;
    uint8_t height;
};

uint8_t u8g2_gpio_and_delay_stm32(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr);
uint8_t u8x8_hw_spi_stm32(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr);

Canvas* canvas_init() {
    Canvas* canvas = furi_alloc(sizeof(Canvas));

    u8g2_Setup_st7565_erc12864_alt_f(
        &canvas->fb, U8G2_R0, u8x8_hw_spi_stm32, u8g2_gpio_and_delay_stm32);

    // send init sequence to the display, display is in sleep mode after this
    u8g2_InitDisplay(&canvas->fb);
    u8g2_SetContrast(&canvas->fb, 36);
    // wake up display
    u8g2_SetPowerSave(&canvas->fb, 0);
    u8g2_SendBuffer(&canvas->fb);

    return canvas;
}

void canvas_free(Canvas* canvas) {
    furi_assert(canvas);
    free(canvas);
}

void canvas_reset(Canvas* canvas) {
    furi_assert(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);
}

void canvas_commit(Canvas* canvas) {
    furi_assert(canvas);
    u8g2_SetPowerSave(&canvas->fb, 0); // wake up display
    u8g2_SendBuffer(&canvas->fb);
}

void canvas_frame_set(
    Canvas* canvas,
    uint8_t offset_x,
    uint8_t offset_y,
    uint8_t width,
    uint8_t height) {
    furi_assert(canvas);
    canvas->offset_x = offset_x;
    canvas->offset_y = offset_y;
    canvas->width = width;
    canvas->height = height;
}

uint8_t canvas_width(Canvas* canvas) {
    furi_assert(canvas);
    return canvas->width;
}

uint8_t canvas_height(Canvas* canvas) {
    furi_assert(canvas);
    return canvas->height;
}

void canvas_clear(Canvas* canvas) {
    furi_assert(canvas);
    u8g2_ClearBuffer(&canvas->fb);
}

void canvas_set_color(Canvas* canvas, Color color) {
    furi_assert(canvas);
    u8g2_SetDrawColor(&canvas->fb, color);
}

void canvas_set_font(Canvas* canvas, Font font) {
    furi_assert(canvas);
    u8g2_SetFontMode(&canvas->fb, 1);
    if(font == FontPrimary) {
        u8g2_SetFont(&canvas->fb, u8g2_font_helvB08_tf);
    } else if(font == FontSecondary) {
        u8g2_SetFont(&canvas->fb, u8g2_font_haxrcorp4089_tr);
    } else if(font == FontGlyph) {
        u8g2_SetFont(&canvas->fb, u8g2_font_unifont_t_symbols);
    } else {
        furi_check(0);
    }
}

void canvas_draw_str(Canvas* canvas, uint8_t x, uint8_t y, const char* str) {
    furi_assert(canvas);
    if(!str) return;
    x += canvas->offset_x;
    y += canvas->offset_y;
    u8g2_DrawStr(&canvas->fb, x, y, str);
}

void canvas_draw_icon(Canvas* canvas, uint8_t x, uint8_t y, Icon* icon) {
    furi_assert(canvas);
    if(!icon) return;
    x += canvas->offset_x;
    y += canvas->offset_y;
    u8g2_DrawXBM(
        &canvas->fb, x, y, icon_get_width(icon), icon_get_height(icon), icon_get_data(icon));
}

void canvas_draw_dot(Canvas* canvas, uint8_t x, uint8_t y) {
    furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    u8g2_DrawPixel(&canvas->fb, x, y);
}

void canvas_draw_box(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    u8g2_DrawBox(&canvas->fb, x, y, width, height);
}

void canvas_draw_frame(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    u8g2_DrawFrame(&canvas->fb, x, y, width, height);
}

void canvas_draw_line(Canvas* canvas, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    furi_assert(canvas);
    x1 += canvas->offset_x;
    y1 += canvas->offset_y;
    x2 += canvas->offset_x;
    y2 += canvas->offset_y;
    u8g2_DrawLine(&canvas->fb, x1, y1, x2, y2);
}

void canvas_draw_xbm(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t w,
    uint8_t h,
    const uint8_t* bitmap) {
    furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    u8g2_DrawXBM(&canvas->fb, x, y, w, h, bitmap);
}

void canvas_draw_glyph(Canvas* canvas, uint8_t x, uint8_t y, uint16_t ch) {
    furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    u8g2_DrawGlyph(&canvas->fb, x, y, ch);
}