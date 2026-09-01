// main/demo_poetry.c —— 唐诗展示
// 功能：显示10首唐诗、支持暂停/继续、上下翻页。10分钟自动更新一首
#include "demo.h"
#include "bsp_display.h"
#include "ui_pixel.h"
#include "poetry_data.h"
#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include <time.h>

static const char *TAG = "demo_poetry";

// 10分钟（600秒）自动翻页
#define AUTO_FLIP_INTERVAL_MS   (10 * 60 * 1000)

static lv_obj_t *s_scr;
static lv_obj_t *s_title_label;       // 诗名
static lv_obj_t *s_author_label;      // 作者
static lv_obj_t *s_content_label;     // 诗文内容
static lv_obj_t *s_index_label;       // 页码指示
static lv_obj_t *s_status_label;      // 暂停/继续状态
static lv_obj_t *s_mascot;

static int s_current_idx = 0;         // 当前展示诗的索引
static bool s_paused = false;         // 是否暂停
static TimerHandle_t s_timer;         // 10分钟定时器

// 格式化诗文显示：将所有行拼接成一个字符串，行间用换行符分隔
static void format_poetry_text(int idx, char *buf, size_t buf_size) {
    if (idx < 0 || idx >= (int)POETRY_COUNT) return;
    
    const poetry_t *p = &POETRY_LIST[idx];
    buf[0] = '\0';
    
    for (int i = 0; i < p->line_count && i < 8; i++) {
        strncat(buf, p->text[i], buf_size - strlen(buf) - 1);
        if (i < p->line_count - 1) {
            strncat(buf, "\n", buf_size - strlen(buf) - 1);
        }
    }
}

// 刷新显示
static void refresh_display(void) {
    if (s_current_idx < 0 || s_current_idx >= (int)POETRY_COUNT) {
        s_current_idx = 0;
    }
    
    const poetry_t *p = &POETRY_LIST[s_current_idx];
    
    // 更新标题和作者
    lv_label_set_text_fmt(s_title_label, "%s", p->title);
    lv_label_set_text_fmt(s_author_label, "— %s", p->author);
    
    // 更新诗文内容
    static char poetry_buf[512];
    format_poetry_text(s_current_idx, poetry_buf, sizeof(poetry_buf));
    lv_label_set_text(s_content_label, poetry_buf);
    
    // 更新页码
    lv_label_set_text_fmt(s_index_label, "%d/%d", s_current_idx + 1, (int)POETRY_COUNT);
    
    // 更新状态
    const char *status = s_paused ? "[PAUSED]" : "[RUNNING]";
    lv_label_set_text(s_status_label, status);
    
    // 状态显示颜色：暂停时红色，运行时绿色
    uint32_t color = s_paused ? 0xFF0000 : 0x00AA00;
    lv_obj_set_style_text_color(s_status_label, lv_color_hex(color), 0);
}

// 定时器回调：10分钟到期，切换到下一首
static void auto_flip_callback(TimerHandle_t xTimer) {
    (void)xTimer;
    
    // 定时器回调中不能直接操作LVGL，需要加锁
    if (!bsp_lvgl_lock(500)) return;
    
    if (!s_paused) {
        s_current_idx = (s_current_idx + 1) % POETRY_COUNT;
        refresh_display();
    }
    
    bsp_lvgl_unlock();
}

void demo_poetry_enter(void) {
    ESP_LOGI(TAG, "唐诗展示开始");
    
    s_scr = ui_pixel_screen_create("POETRY");
    
    // 标题面板
    lv_obj_t *title_panel = ui_pixel_panel_create(s_scr, 18, 20, 204, 40, UI_PAPER);
    s_title_label = lv_label_create(title_panel);
    lv_obj_set_style_text_font(s_title_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(s_title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(s_title_label, lv_color_hex(UI_INK), 0);
    lv_obj_center(s_title_label);
    
    s_author_label = lv_label_create(title_panel);
    lv_obj_set_style_text_font(s_author_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(s_author_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(s_author_label, lv_color_hex(0x666666), 0);
    lv_obj_align(s_author_label, LV_ALIGN_BOTTOM_MID, 0, -2);
    
    // 诗文内容面板
    lv_obj_t *content_panel = ui_pixel_panel_create(s_scr, 18, 68, 204, 128, UI_PAPER);
    s_content_label = lv_label_create(content_panel);
    lv_obj_set_style_text_font(s_content_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_align(s_content_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(s_content_label, lv_color_hex(UI_INK), 0);
    lv_label_set_long_mode(s_content_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(s_content_label, 188);
    lv_obj_center(s_content_label);
    
    // 底部信息栏
    lv_obj_t *info_panel = ui_pixel_panel_create(s_scr, 18, 204, 204, 32, UI_PAPER);
    
    s_index_label = lv_label_create(info_panel);
    lv_obj_set_style_text_font(s_index_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(s_index_label, lv_color_hex(UI_INK), 0);
    lv_obj_align(s_index_label, LV_ALIGN_LEFT_MID, 10, 0);
    
    s_status_label = lv_label_create(info_panel);
    lv_obj_set_style_text_font(s_status_label, &lv_font_montserrat_12, 0);
    lv_obj_align(s_status_label, LV_ALIGN_CENTER, 0, 0);
    
    lv_obj_t *help_label = lv_label_create(info_panel);
    lv_obj_set_style_text_font(help_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(help_label, lv_color_hex(0x888888), 0);
    lv_label_set_text(help_label, "UP/DOWN: PREV/NEXT  OK: PAUSE/RESUME");
    lv_obj_align(help_label, LV_ALIGN_RIGHT_MID, -10, 0);
    
    s_mascot = ui_pixel_mascot_create(s_scr, 101, 238);
    
    // 初始化状态
    s_current_idx = 0;
    s_paused = false;
    
    // 创建10分钟定时器
    if (s_timer == NULL) {
        s_timer = xTimerCreate(
            "poetry_timer",
            pdMS_TO_TICKS(AUTO_FLIP_INTERVAL_MS),
            pdTRUE,  // 自动重复
            NULL,
            auto_flip_callback
        );
    }
    if (s_timer != NULL) {
        xTimerStart(s_timer, 0);
    }
    
    refresh_display();
    lv_screen_load(s_scr);
}

void demo_poetry_exit(void) {
    ESP_LOGI(TAG, "唐诗展示退出");
    
    // 停止定时器
    if (s_timer != NULL) {
        xTimerStop(s_timer, 0);
        xTimerDelete(s_timer, 0);
        s_timer = NULL;
    }
    
    if (s_scr) {
        lv_obj_delete(s_scr);
        s_scr = NULL;
        s_title_label = NULL;
        s_author_label = NULL;
        s_content_label = NULL;
        s_index_label = NULL;
        s_status_label = NULL;
        s_mascot = NULL;
    }
}

void demo_poetry_key(bsp_btn_t btn, bsp_btn_ev_t ev) {
    if (ev != BSP_BTN_CLICK) return;
    
    if (btn == BSP_BTN_UP) {
        // 上一首
        s_current_idx = (s_current_idx + POETRY_COUNT - 1) % POETRY_COUNT;
        refresh_display();
        ui_pixel_mascot_jump(s_mascot);
    } else if (btn == BSP_BTN_DOWN) {
        // 下一首
        s_current_idx = (s_current_idx + 1) % POETRY_COUNT;
        refresh_display();
        ui_pixel_mascot_jump(s_mascot);
    } else if (btn == BSP_BTN_OK) {
        // 暂停/继续
        s_paused = !s_paused;
        if (s_paused) {
            xTimerStop(s_timer, 0);
        } else {
            xTimerStart(s_timer, 0);
        }
        refresh_display();
        ui_pixel_mascot_jump(s_mascot);
    }
}
