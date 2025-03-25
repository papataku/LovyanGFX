/*----------------------------------------------------------------------------/
 *  Lovyan GFX - Graphics library for embedded devices.
 *
 * Original Source:
 * https://github.com/lovyan03/LovyanGFX/
 *
 * Licence:
 * [FreeBSD](https://github.com/lovyan03/LovyanGFX/blob/master/license.txt)
 *
 * Author:
 * [lovyan03](https://twitter.com/lovyan03)
 *
 * Contributors:
 * [ciniml](https://github.com/ciniml)
 * [mongonta0716](https://github.com/mongonta0716)
 * [tobozo](https://github.com/tobozo)
 * /----------------------------------------------------------------------------*/

#if defined (ESP_PLATFORM)

#include "Panel_ST77916v2.hpp"
#include "../Bus.hpp"
#include "../platforms/common.hpp"
#include "../misc/pixelcopy.hpp"
#include "../misc/colortype.hpp"
#include "esp_log.h"

#include <ESP_IOExpander.h>
#include <ESP_Panel_Library.h>
#include "freertos/semphr.h"

/**
 * @brief Bug list (inherited from Panel_SH8601Z)
 *
 *  > Write image (pushSprite) works fine, bugs down below are from writing directly
 *
 *  1> Write function is block even with DMA (manual CS wait data)
 *  2> In spi 40MHz draw vertical line incomplete, but 10MHz OK (Likely because my dupont line connection)
 *  3> After implement write/draw pixel funcs, "testFilledRects" stucks sometime, acts differently to the different sck freq
 *  4> Haven't found the way to set rotation by reg
 */


namespace lgfx
{
    inline namespace v1
    {
        //----------------------------------------------------------------------------
        const esp_lcd_panel_vendor_init_cmd_t lcd_init_cmd[] = {
            {0xF0, (uint8_t[]){0x08}, 1, 0},
            {0xF2, (uint8_t[]){0x08}, 1, 0},
            {0x9B, (uint8_t[]){0x51}, 1, 0},
            {0x86, (uint8_t[]){0x53}, 1, 0},
            {0xF2, (uint8_t[]){0x80}, 1, 0},
            {0xF0, (uint8_t[]){0x00}, 1, 0},
            {0xF0, (uint8_t[]){0x01}, 1, 0},
            {0xF1, (uint8_t[]){0x01}, 1, 0},
            {0xB0, (uint8_t[]){0x54}, 1, 0},
            {0xB1, (uint8_t[]){0x3F}, 1, 0},
            {0xB2, (uint8_t[]){0x2A}, 1, 0},
            {0xB4, (uint8_t[]){0x46}, 1, 0},
            {0xB5, (uint8_t[]){0x34}, 1, 0},
            {0xB6, (uint8_t[]){0xD5}, 1, 0},
            {0xB7, (uint8_t[]){0x30}, 1, 0},
            {0xBA, (uint8_t[]){0x00}, 1, 0},
            {0xBB, (uint8_t[]){0x08}, 1, 0},
            {0xBC, (uint8_t[]){0x08}, 1, 0},
            {0xBD, (uint8_t[]){0x00}, 1, 0},
            {0xC0, (uint8_t[]){0x80}, 1, 0},
            {0xC1, (uint8_t[]){0x10}, 1, 0},
            {0xC2, (uint8_t[]){0x37}, 1, 0},
            {0xC3, (uint8_t[]){0x80}, 1, 0},
            {0xC4, (uint8_t[]){0x10}, 1, 0},
            {0xC5, (uint8_t[]){0x37}, 1, 0},
            {0xC6, (uint8_t[]){0xA9}, 1, 0},
            {0xC7, (uint8_t[]){0x41}, 1, 0},
            {0xC8, (uint8_t[]){0x51}, 1, 0},
            {0xC9, (uint8_t[]){0xA9}, 1, 0},
            {0xCA, (uint8_t[]){0x41}, 1, 0},
            {0xCB, (uint8_t[]){0x51}, 1, 0},
            {0xD0, (uint8_t[]){0x91}, 1, 0},
            {0xD1, (uint8_t[]){0x68}, 1, 0},
            {0xD2, (uint8_t[]){0x69}, 1, 0},
            {0xF5, (uint8_t[]){0x00, 0xA5}, 2, 0},
            {0xDD, (uint8_t[]){0x3F}, 1, 0},
            {0xDE, (uint8_t[]){0x3F}, 1, 0},
            {0xF1, (uint8_t[]){0x10}, 1, 0},
            {0xF0, (uint8_t[]){0x00}, 1, 0},
            {0xF0, (uint8_t[]){0x02}, 1, 0},
            {0xE0, (uint8_t[]){0x70, 0x09, 0x12, 0x0C, 0x0B, 0x27, 0x38, 0x54, 0x4E, 0x19, 0x15, 0x15, 0x2C, 0x2F}, 14, 0},
            {0xE1, (uint8_t[]){0x70, 0x08, 0x11, 0x0C, 0x0B, 0x27, 0x38, 0x43, 0x4C, 0x18, 0x14, 0x14, 0x2B, 0x2D}, 14, 0},
            {0xF0, (uint8_t[]){0x10}, 1, 0},
            {0xF3, (uint8_t[]){0x10}, 1, 0},
            {0xE0, (uint8_t[]){0x08}, 1, 0},
            {0xE1, (uint8_t[]){0x00}, 1, 0},
            {0xE2, (uint8_t[]){0x00}, 1, 0},
            {0xE3, (uint8_t[]){0x00}, 1, 0},
            {0xE4, (uint8_t[]){0xE0}, 1, 0},
            {0xE5, (uint8_t[]){0x06}, 1, 0},
            {0xE6, (uint8_t[]){0x21}, 1, 0},
            {0xE7, (uint8_t[]){0x00}, 1, 0},
            {0xE8, (uint8_t[]){0x05}, 1, 0},
            {0xE9, (uint8_t[]){0x82}, 1, 0},
            {0xEA, (uint8_t[]){0xDF}, 1, 0},
            {0xEB, (uint8_t[]){0x89}, 1, 0},
            {0xEC, (uint8_t[]){0x20}, 1, 0},
            {0xED, (uint8_t[]){0x14}, 1, 0},
            {0xEE, (uint8_t[]){0xFF}, 1, 0},
            {0xEF, (uint8_t[]){0x00}, 1, 0},
            {0xF8, (uint8_t[]){0xFF}, 1, 0},
            {0xF9, (uint8_t[]){0x00}, 1, 0},
            {0xFA, (uint8_t[]){0x00}, 1, 0},
            {0xFB, (uint8_t[]){0x30}, 1, 0},
            {0xFC, (uint8_t[]){0x00}, 1, 0},
            {0xFD, (uint8_t[]){0x00}, 1, 0},
            {0xFE, (uint8_t[]){0x00}, 1, 0},
            {0xFF, (uint8_t[]){0x00}, 1, 0},
            {0x60, (uint8_t[]){0x42}, 1, 0},
            {0x61, (uint8_t[]){0xE0}, 1, 0},
            {0x62, (uint8_t[]){0x40}, 1, 0},
            {0x63, (uint8_t[]){0x40}, 1, 0},
            {0x64, (uint8_t[]){0x02}, 1, 0},
            {0x65, (uint8_t[]){0x00}, 1, 0},
            {0x66, (uint8_t[]){0x40}, 1, 0},
            {0x67, (uint8_t[]){0x03}, 1, 0},
            {0x68, (uint8_t[]){0x00}, 1, 0},
            {0x69, (uint8_t[]){0x00}, 1, 0},
            {0x6A, (uint8_t[]){0x00}, 1, 0},
            {0x6B, (uint8_t[]){0x00}, 1, 0},
            {0x70, (uint8_t[]){0x42}, 1, 0},
            {0x71, (uint8_t[]){0xE0}, 1, 0},
            {0x72, (uint8_t[]){0x40}, 1, 0},
            {0x73, (uint8_t[]){0x40}, 1, 0},
            {0x74, (uint8_t[]){0x02}, 1, 0},
            {0x75, (uint8_t[]){0x00}, 1, 0},
            {0x76, (uint8_t[]){0x40}, 1, 0},
            {0x77, (uint8_t[]){0x03}, 1, 0},
            {0x78, (uint8_t[]){0x00}, 1, 0},
            {0x79, (uint8_t[]){0x00}, 1, 0},
            {0x7A, (uint8_t[]){0x00}, 1, 0},
            {0x7B, (uint8_t[]){0x00}, 1, 0},
            {0x80, (uint8_t[]){0x48}, 1, 0},
            {0x81, (uint8_t[]){0x00}, 1, 0},
            {0x82, (uint8_t[]){0x05}, 1, 0},
            {0x83, (uint8_t[]){0x02}, 1, 0},
            {0x84, (uint8_t[]){0xDD}, 1, 0},
            {0x85, (uint8_t[]){0x00}, 1, 0},
            {0x86, (uint8_t[]){0x00}, 1, 0},
            {0x87, (uint8_t[]){0x00}, 1, 0},
            {0x88, (uint8_t[]){0x48}, 1, 0},
            {0x89, (uint8_t[]){0x00}, 1, 0},
            {0x8A, (uint8_t[]){0x07}, 1, 0},
            {0x8B, (uint8_t[]){0x02}, 1, 0},
            {0x8C, (uint8_t[]){0xDF}, 1, 0},
            {0x8D, (uint8_t[]){0x00}, 1, 0},
            {0x8E, (uint8_t[]){0x00}, 1, 0},
            {0x8F, (uint8_t[]){0x00}, 1, 0},
            {0x90, (uint8_t[]){0x48}, 1, 0},
            {0x91, (uint8_t[]){0x00}, 1, 0},
            {0x92, (uint8_t[]){0x09}, 1, 0},
            {0x93, (uint8_t[]){0x02}, 1, 0},
            {0x94, (uint8_t[]){0xE1}, 1, 0},
            {0x95, (uint8_t[]){0x00}, 1, 0},
            {0x96, (uint8_t[]){0x00}, 1, 0},
            {0x97, (uint8_t[]){0x00}, 1, 0},
            {0x98, (uint8_t[]){0x48}, 1, 0},
            {0x99, (uint8_t[]){0x00}, 1, 0},
            {0x9A, (uint8_t[]){0x0B}, 1, 0},
            {0x9B, (uint8_t[]){0x02}, 1, 0},
            {0x9C, (uint8_t[]){0xE3}, 1, 0},
            {0x9D, (uint8_t[]){0x00}, 1, 0},
            {0x9E, (uint8_t[]){0x00}, 1, 0},
            {0x9F, (uint8_t[]){0x00}, 1, 0},
            {0xA0, (uint8_t[]){0x48}, 1, 0},
            {0xA1, (uint8_t[]){0x00}, 1, 0},
            {0xA2, (uint8_t[]){0x04}, 1, 0},
            {0xA3, (uint8_t[]){0x02}, 1, 0},
            {0xA4, (uint8_t[]){0xDC}, 1, 0},
            {0xA5, (uint8_t[]){0x00}, 1, 0},
            {0xA6, (uint8_t[]){0x00}, 1, 0},
            {0xA7, (uint8_t[]){0x00}, 1, 0},
            {0xA8, (uint8_t[]){0x48}, 1, 0},
            {0xA9, (uint8_t[]){0x00}, 1, 0},
            {0xAA, (uint8_t[]){0x06}, 1, 0},
            {0xAB, (uint8_t[]){0x02}, 1, 0},
            {0xAC, (uint8_t[]){0xDE}, 1, 0},
            {0xAD, (uint8_t[]){0x00}, 1, 0},
            {0xAE, (uint8_t[]){0x00}, 1, 0},
            {0xAF, (uint8_t[]){0x00}, 1, 0},
            {0xB0, (uint8_t[]){0x48}, 1, 0},
            {0xB1, (uint8_t[]){0x00}, 1, 0},
            {0xB2, (uint8_t[]){0x08}, 1, 0},
            {0xB3, (uint8_t[]){0x02}, 1, 0},
            {0xB4, (uint8_t[]){0xE0}, 1, 0},
            {0xB5, (uint8_t[]){0x00}, 1, 0},
            {0xB6, (uint8_t[]){0x00}, 1, 0},
            {0xB7, (uint8_t[]){0x00}, 1, 0},
            {0xB8, (uint8_t[]){0x48}, 1, 0},
            {0xB9, (uint8_t[]){0x00}, 1, 0},
            {0xBA, (uint8_t[]){0x0A}, 1, 0},
            {0xBB, (uint8_t[]){0x02}, 1, 0},
            {0xBC, (uint8_t[]){0xE2}, 1, 0},
            {0xBD, (uint8_t[]){0x00}, 1, 0},
            {0xBE, (uint8_t[]){0x00}, 1, 0},
            {0xBF, (uint8_t[]){0x00}, 1, 0},
            {0xC0, (uint8_t[]){0x12}, 1, 0},
            {0xC1, (uint8_t[]){0xAA}, 1, 0},
            {0xC2, (uint8_t[]){0x65}, 1, 0},
            {0xC3, (uint8_t[]){0x74}, 1, 0},
            {0xC4, (uint8_t[]){0x47}, 1, 0},
            {0xC5, (uint8_t[]){0x56}, 1, 0},
            {0xC6, (uint8_t[]){0x00}, 1, 0},
            {0xC7, (uint8_t[]){0x88}, 1, 0},
            {0xC8, (uint8_t[]){0x99}, 1, 0},
            {0xC9, (uint8_t[]){0x33}, 1, 0},
            {0xD0, (uint8_t[]){0x21}, 1, 0},
            {0xD1, (uint8_t[]){0xAA}, 1, 0},
            {0xD2, (uint8_t[]){0x65}, 1, 0},
            {0xD3, (uint8_t[]){0x74}, 1, 0},
            {0xD4, (uint8_t[]){0x47}, 1, 0},
            {0xD5, (uint8_t[]){0x56}, 1, 0},
            {0xD6, (uint8_t[]){0x00}, 1, 0},
            {0xD7, (uint8_t[]){0x88}, 1, 0},
            {0xD8, (uint8_t[]){0x99}, 1, 0},
            {0xD9, (uint8_t[]){0x33}, 1, 0},
            {0xF3, (uint8_t[]){0x01}, 1, 0},
            {0xF0, (uint8_t[]){0x00}, 1, 0},
            {0xF0, (uint8_t[]){0x01}, 1, 0},
            {0xF1, (uint8_t[]){0x01}, 1, 0},
            {0xA0, (uint8_t[]){0x0B}, 1, 0},
            {0xA3, (uint8_t[]){0x2A}, 1, 0},
            {0xA5, (uint8_t[]){0xC3}, 1, 1},
            {0xA3, (uint8_t[]){0x2B}, 1, 0},
            {0xA5, (uint8_t[]){0xC3}, 1, 1},
            {0xA3, (uint8_t[]){0x2C}, 1, 0},
            {0xA5, (uint8_t[]){0xC3}, 1, 1},
            {0xA3, (uint8_t[]){0x2D}, 1, 0},
            {0xA5, (uint8_t[]){0xC3}, 1, 1},
            {0xA3, (uint8_t[]){0x2E}, 1, 0},
            {0xA5, (uint8_t[]){0xC3}, 1, 1},
            {0xA3, (uint8_t[]){0x2F}, 1, 0},
            {0xA5, (uint8_t[]){0xC3}, 1, 1},
            {0xA3, (uint8_t[]){0x30}, 1, 0},
            {0xA5, (uint8_t[]){0xC3}, 1, 1},
            {0xA3, (uint8_t[]){0x31}, 1, 0},
            {0xA5, (uint8_t[]){0xC3}, 1, 1},
            {0xA3, (uint8_t[]){0x32}, 1, 0},
            {0xA5, (uint8_t[]){0xC3}, 1, 1},
            {0xA3, (uint8_t[]){0x33}, 1, 0},
            {0xA5, (uint8_t[]){0xC3}, 1, 1},
            {0xA0, (uint8_t[]){0x09}, 1, 0},
            {0xF1, (uint8_t[]){0x10}, 1, 0},
            {0xF0, (uint8_t[]){0x00}, 1, 0},
            {0x2A, (uint8_t[]){0x00, 0x00, 0x01, 0x67}, 4, 0},
            {0x2B, (uint8_t[]){0x01, 0x68, 0x01, 0x68}, 4, 0},
            {0x4D, (uint8_t[]){0x00}, 1, 0},
            {0x4E, (uint8_t[]){0x00}, 1, 0},
            {0x4F, (uint8_t[]){0x00}, 1, 0},
            {0x4C, (uint8_t[]){0x01}, 1, 10},
            {0x4C, (uint8_t[]){0x00}, 1, 0},
            {0x2A, (uint8_t[]){0x00, 0x00, 0x01, 0x67}, 4, 0},
            {0x2B, (uint8_t[]){0x00, 0x00, 0x01, 0x67}, 4, 0},
            {0x21, (uint8_t[]){0x00}, 1, 0},
            //{0x3A, (uint8_t[]){0x55}, 1, 0}, // color=16
            {0x11, (uint8_t[]){0x00}, 1, 120},
            {0x29, (uint8_t[]){0x00}, 1, 0},
        };

        #define TFT_BLK 15

        #define TFT_RST 47
        #define TFT_CS 10
        #define TFT_SCK 9
        #define TFT_SDA0 11
        #define TFT_SDA1 12
        #define TFT_SDA2 13
        #define TFT_SDA3 14

        #define TFT_SPI_FREQ_HZ (50 * 1000 * 1000)
        ESP_PanelLcd *lcd = NULL;
        ESP_PanelBacklight *backlight = NULL;

        QueueHandle_t _refresh_finish_queue;

        uint_fast16_t x_start, y_start, x_end, y_end;
        
        FlipBuffer _flip_buffer;

        #define MAX_TRANSBUFFER_SIZE    (64*1024)
        #define MAX_TRANS_LINE          (MAX_TRANSBUFFER_SIZE / (360 * 2))

        /* Panel init */
        bool Panel_ST77916v2::init(bool use_reset)
        {
            ESP_LOGD("ST77916v2","pannel init %d", use_reset);

            ledc_timer_config_t ledc_timer = {
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .duty_resolution = LEDC_TIMER_13_BIT,
                .timer_num = LEDC_TIMER_0,
                .freq_hz = 5000,
                .clk_cfg = LEDC_AUTO_CLK};
            ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
          
            ledc_channel_config_t ledc_channel = {
                .gpio_num = (TFT_BLK),
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .channel = LEDC_CHANNEL_0,
                .intr_type = LEDC_INTR_DISABLE,
                .timer_sel = LEDC_TIMER_0,
                .duty = 0,
                .hpoint = 0};
          
            ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
          
            backlight = new ESP_PanelBacklight(ledc_timer, ledc_channel);
            backlight->begin();
            backlight->off();

            ESP_PanelBus_QSPI *panel_bus = new ESP_PanelBus_QSPI(TFT_CS, TFT_SCK, TFT_SDA0, TFT_SDA1, TFT_SDA2, TFT_SDA3);
            panel_bus->configQspiFreqHz(TFT_SPI_FREQ_HZ);
            panel_bus->begin();
          
            lcd = new ESP_PanelLcd_ST77916(panel_bus, 16, TFT_RST);

            lcd->init();
            lcd->reset();
            lcd->begin();
          
            lcd->invertColor(true);
            // setRotation(0);  //设置屏幕方向
            lcd->displayOn();

            backlight->on();

            // メッセージキューを作成
            _refresh_finish_queue = xQueueCreate(1, sizeof(uint8_t));
            // メッセージキューにデータを送信
            uint8_t data = 0;
            xQueueSend(_refresh_finish_queue, &data, portMAX_DELAY);

            lcd->attachRefreshFinishCallback(&Panel_ST77916v2::onRefreshFinishCallback, NULL);

            return true;
        }


        void Panel_ST77916v2::update_madctl(void)
        {
            uint8_t r = _internal_rotation;
            switch (r)
            {
                case 1:
                    lcd->swapXY(true);
                    lcd->mirrorX(true);
                    lcd->mirrorY(false);
                    break;
                case 2:
                    lcd->swapXY(false);
                    lcd->mirrorX(true);
                    lcd->mirrorY(true);
                    break;
                case 3:
                    lcd->swapXY(true);
                    lcd->mirrorX(false);
                    lcd->mirrorY(true);
                    break;
                default: // case 0:
                    lcd->swapXY(false);
                    lcd->mirrorX(false);
                    lcd->mirrorY(false);
                    break;
            }
        }



        void Panel_ST77916v2::setInvert(bool invert)
        {
            ESP_LOGD("ST77916v2","setInvert %d", invert);

        }


        void Panel_ST77916v2::setSleep(bool flg)
        {
            //ESP_LOGD("ST77916v2","setSleep %d", flg);

        }


        void Panel_ST77916v2::setPowerSave(bool flg)
        {
            //ESP_LOGD("ST77916v2","setPowerSave");
        }


        void Panel_ST77916v2::waitDisplay(void)
        {
            //ESP_LOGD("ST77916v2","waitDisplay");
        }


        bool Panel_ST77916v2::displayBusy(void)
        {
            //ESP_LOGD("ST77916v2","displayBusy");
            return false;
        }


        color_depth_t Panel_ST77916v2::setColorDepth(color_depth_t depth)
        {
            //ESP_LOGD("ST77916v2","setColorDepth %d", depth);
            return depth;
        }

        void Panel_ST77916v2::beginTransaction(void)
        {
            //ESP_LOGD("ST77916v2","beginTransaction");
            if (_in_transaction) return;
            _in_transaction = true;
        }


        void Panel_ST77916v2::endTransaction(void)
        {
            //ESP_LOGD("ST77916v2","endTransaction");
            // if (!_in_transaction) return;
            // _in_transaction = false;
            // _bus->endTransaction();

            if (!_in_transaction) return;
            _in_transaction = false;

        }

        void Panel_ST77916v2::setWindow(uint_fast16_t xs, uint_fast16_t ys, uint_fast16_t xe, uint_fast16_t ye)
        {
            ESP_LOGD("ST77916v2","setWindow %d %d %d %d", xs, ys, xe, ye);
            x_start = xs; 
            y_start = ys;
            x_end = xe;
            y_end = ye;
        }


        void Panel_ST77916v2::writeBlock(uint32_t rawcolor, uint32_t len)
        {
            ESP_LOGD("ST77916v2","writeBlock 0x%lx %ld", rawcolor, len);

        }


        void Panel_ST77916v2::writePixels(pixelcopy_t* param, uint32_t len, bool use_dma)
        {
            ESP_LOGD("ST77916v2","writePixels %ld %d", len, use_dma);
        }


        void Panel_ST77916v2::drawPixelPreclipped(uint_fast16_t x, uint_fast16_t y, uint32_t rawcolor)
        {
            ESP_LOGD("ST77916v2","drawPixelPreclipped %d %d 0x%lX", x, y, rawcolor);

        }


        void Panel_ST77916v2::writeFillRectPreclipped(uint_fast16_t x, uint_fast16_t y, uint_fast16_t w, uint_fast16_t h, uint32_t rawcolor)
        {
            ESP_LOGD("ST77916v2","writeFillRectPreclipped %d %d %d %d 0x%lX", x, y, w, h, rawcolor);

            auto bytes = 2;
            uint32_t buffer_size = MAX_TRANSBUFFER_SIZE < (w * h * bytes) ? MAX_TRANSBUFFER_SIZE : (w * h * bytes);
            uint8_t *buf = _flip_buffer.getBuffer(buffer_size);
            uint16_t *buf16 = (uint16_t *)buf;
            auto wb = w * bytes;
            uint16_t trans_line = buffer_size / wb;

            for(uint32_t i = 0; i < w; i++)
            {
                buf16[i] = (uint16_t)(rawcolor & 0xFFFF);
            }

            for(uint32_t i = 0; i < trans_line; i++)
            {
                memcpy(&buf[wb * i], buf, wb);
            }

            for(uint32_t i = 0; i < (h / trans_line); i++)
            {
                // キューを受信
                uint8_t data;
                xQueueReceive(_refresh_finish_queue, &data, portMAX_DELAY);
                lcd->drawBitmap(x, y + (i * trans_line), w, trans_line, (const uint8_t *)buf);
            }
            if (0 != (h % trans_line)) {
                // キューを受信
                uint8_t data;
                xQueueReceive(_refresh_finish_queue, &data, portMAX_DELAY);
                lcd->drawBitmap(x, y + (((uint16_t)(h / trans_line)) * trans_line), w, (h % trans_line), (const uint8_t *)buf);
            }

        }



        void Panel_ST77916v2::writeImage(uint_fast16_t x, uint_fast16_t y, uint_fast16_t w, uint_fast16_t h, pixelcopy_t* param, bool use_dma)
        {
            ESP_LOGD("ST77916v2","writeImage %d %d %d %d %d", x, y, w, h, use_dma);
            // use_dma = false;

            auto bytes = param->dst_bits >> 3;
            auto src_x = param->src_x;

            if (param->transp == pixelcopy_t::NON_TRANSP)
            {
                if (param->no_convert)
                {
                    auto wb = w * bytes;
                    uint32_t i = (src_x + param->src_y * param->src_bitwidth) * bytes;
                    uint8_t *src = (uint8_t *)&((const uint8_t*)param->src_data)[i];
                    setWindow(x, y, x + w - 1, y + h - 1);
                    if (param->src_bitwidth == w || h == 1)
                    {
                        int_fast16_t local_h = h;
                        do {
                            // キューを受信
                            uint8_t data;
                            xQueueReceive(_refresh_finish_queue, &data, portMAX_DELAY);
                            lcd->drawBitmap(x, y + (h - local_h), w, (MAX_TRANS_LINE < local_h) ? MAX_TRANS_LINE : local_h, (const uint8_t *)src);
                            src += wb * MAX_TRANS_LINE;
                            local_h -= MAX_TRANS_LINE;
                        } while (local_h > 0);

                    }
                    else
                    {
                        auto add = param->src_bitwidth * bytes;
                        uint_fast16_t local_h = h;
                        do
                        {
                            //write_bytes(src, wb, false);
                            // キューを受信
                            uint8_t data;
                            xQueueReceive(_refresh_finish_queue, &data, portMAX_DELAY);
                            lcd->drawBitmap(x, y + (h - local_h), w, 1, (const uint8_t *)src);
                            src += add;
                        } while (--local_h);
                    }
                }
                else
                {
                    size_t wb = w * bytes;
                    auto buf = _flip_buffer.getBuffer(wb);
                    param->fp_copy(buf, 0, w, param);
                    setWindow(x, y, x + w - 1, y + h - 1);
                    //write_bytes(buf, wb, true);
                    // キューを受信
                    uint8_t data;
                    xQueueReceive(_refresh_finish_queue, &data, portMAX_DELAY);
                    lcd->drawBitmap(x, y, w, 1, (const uint8_t *)buf);
                    _has_align_data = (_cfg.dlen_16bit && (_write_bits & 15) && (w & h & 1));
                    uint_fast16_t local_h = h;
                    while (--h)
                    {
                        param->src_x = src_x;
                        param->src_y++;
                        buf = _flip_buffer.getBuffer(wb);
                        param->fp_copy(buf, 0, w, param);
                        //write_bytes(buf, wb, true);
                        // キューを受信
                        uint8_t data;
                        xQueueReceive(_refresh_finish_queue, &data, portMAX_DELAY);
                        lcd->drawBitmap(x, y+(local_h - h), w, 1, (const uint8_t *)buf);
                    }

                }
            }
            else
            {
                h += y;
                uint32_t wb = w * bytes;
                do
                {
                    uint32_t i = 0;
                    while (w != (i = param->fp_skip(i, w, param)))
                    {
                        auto buf = _flip_buffer.getBuffer(wb);
                        int32_t len = param->fp_copy(buf, 0, w - i, param);
                        //setWindow(x + i, y, x + i + len - 1, y);
                        //write_bytes(buf, len * bytes, true);
                        // キューを受信
                        uint8_t data;
                        xQueueReceive(_refresh_finish_queue, &data, portMAX_DELAY);
                        lcd->drawBitmap(x+i, y, len, 1, (const uint8_t *)buf);
                        if (w == (i += len)) break;
                    }
                    param->src_x = src_x;
                    param->src_y++;
                } while (++y != h);
            }
        }




        uint32_t Panel_ST77916v2::readCommand(uint_fast16_t cmd, uint_fast8_t index, uint_fast8_t len)
        {
            //ESP_LOGD("ST77916v2","readCommand");
            return 0;
        }

        uint32_t Panel_ST77916v2::readData(uint_fast8_t index, uint_fast8_t len)
        {
            //ESP_LOGD("ST77916v2","readData");
            return 0;
        }

        void Panel_ST77916v2::readRect(uint_fast16_t x, uint_fast16_t y, uint_fast16_t w, uint_fast16_t h, void* dst, pixelcopy_t* param)
        {
            //ESP_LOGD("ST77916v2","readRect");
        }

        bool Panel_ST77916v2::onRefreshFinishCallback(void *user_data)
        {
            BaseType_t need_yield = pdFALSE;
            //xSemaphoreGiveFromISR(_refresh_finish_sem, &need_yield);
            // キューにデータを送信
            uint8_t data = 0;
            xQueueSendFromISR(_refresh_finish_queue, &data, &need_yield);
            return need_yield;
        }

        void Panel_ST77916v2::setBrightness(uint8_t brightness)
        {
            backlight->setBrightness(brightness * 100 / 255);
        } 

        void Panel_ST77916v2::rst_control(bool level)
        {
            
        }
        //----------------------------------------------------------------------------
    }
}


#endif
