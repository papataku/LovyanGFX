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
#pragma once

#if defined (ESP_PLATFORM)

#include "Panel_LCD.hpp"


namespace lgfx
{
    inline namespace v1
    {
        //----------------------------------------------------------------------------

        struct Panel_ST77916 : public Panel_LCD
        {
        protected:

            static constexpr uint8_t CMD_RST_DELAY    = 120 ;   ///< delay ms wait for reset finish
            static constexpr uint8_t CMD_SLPIN_DELAY  = 120 ;   ///< delay ms wait for sleep in finish
            static constexpr uint8_t CMD_SLPOUT_DELAY = 120 ;   ///< delay ms wait for sleep out finish
            static constexpr uint8_t CMD_NOP          = 0x00;
            static constexpr uint8_t CMD_SWRESET      = 0x01;
            static constexpr uint8_t CMD_SLPIN        = 0x10;
            static constexpr uint8_t CMD_SLPOUT       = 0x11;
            static constexpr uint8_t CMD_INVOFF       = 0x20;
            static constexpr uint8_t CMD_INVON        = 0x21;
            static constexpr uint8_t CMD_DISPOFF      = 0x28;
            static constexpr uint8_t CMD_DISPON       = 0x29;
            static constexpr uint8_t CMD_CASET        = 0x2A;
            static constexpr uint8_t CMD_RASET        = 0x2B;
            static constexpr uint8_t CMD_RAMWR        = 0x2C;
            static constexpr uint8_t CMD_MADCTL       = 0x36;
            static constexpr uint8_t CMD_MOLMOD       = 0x3A;
            static constexpr uint8_t CMD_MADCTL_MY    = 0x80;
            static constexpr uint8_t CMD_MADCTL_MX    = 0x40;
            static constexpr uint8_t CMD_MADCTL_MV    = 0x20;
            static constexpr uint8_t CMD_MADCTL_ML    = 0x10;
            static constexpr uint8_t CMD_MADCTL_RGB   = 0x00;

            typedef struct {
                int cmd;                /*<! The specific LCD command */
                const void *data;       /*<! Buffer that holds the command specific data */
                size_t data_bytes;      /*<! Size of `data` in memory, in bytes */
                unsigned int delay_ms;  /*<! Delay in milliseconds after this command */
            } st77916_init_cmd_t;

            static constexpr st77916_init_cmd_t init_cmds[] =
            {
                {0xF0, (uint8_t []){0x08}, 1, 0},
                {0xF2, (uint8_t []){0x08}, 1, 0},
                {0x9B, (uint8_t []){0x51}, 1, 0},
                {0x86, (uint8_t []){0x53}, 1, 0},
                {0xF2, (uint8_t []){0x80}, 1, 0},
                {0xF0, (uint8_t []){0x00}, 1, 0},
                {0xF0, (uint8_t []){0x01}, 1, 0},
                {0xF1, (uint8_t []){0x01}, 1, 0},
                {0xB0, (uint8_t []){0x54}, 1, 0},
                {0xB1, (uint8_t []){0x3F}, 1, 0},
                {0xB2, (uint8_t []){0x2A}, 1, 0},
                {0xB4, (uint8_t []){0x46}, 1, 0},
                {0xB5, (uint8_t []){0x34}, 1, 0},
                {0xB6, (uint8_t []){0xD5}, 1, 0},
                {0xB7, (uint8_t []){0x30}, 1, 0},
                {0xB8, (uint8_t []){0x04}, 1, 0},
                {0xBA, (uint8_t []){0x00}, 1, 0},
                {0xBB, (uint8_t []){0x08}, 1, 0},
                {0xBC, (uint8_t []){0x08}, 1, 0},
                {0xBD, (uint8_t []){0x00}, 1, 0},
                {0xC0, (uint8_t []){0x80}, 1, 0},
                {0xC1, (uint8_t []){0x10}, 1, 0},
                {0xC2, (uint8_t []){0x37}, 1, 0},
                {0xC3, (uint8_t []){0x80}, 1, 0},
                {0xC4, (uint8_t []){0x10}, 1, 0},
                {0xC5, (uint8_t []){0x37}, 1, 0},
                {0xC6, (uint8_t []){0xA9}, 1, 0},
                {0xC7, (uint8_t []){0x41}, 1, 0},
                {0xC8, (uint8_t []){0x51}, 1, 0},
                {0xC9, (uint8_t []){0xA9}, 1, 0},
                {0xCA, (uint8_t []){0x41}, 1, 0},
                {0xCB, (uint8_t []){0x51}, 1, 0},
                {0xD0, (uint8_t []){0x91}, 1, 0},
                {0xD1, (uint8_t []){0x68}, 1, 0},
                {0xD2, (uint8_t []){0x69}, 1, 0},
                {0xF5, (uint8_t []){0x00, 0xA5}, 2, 0},
                {0xDD, (uint8_t []){0x35}, 1, 0},
                {0xDE, (uint8_t []){0x35}, 1, 0},
                {0xF1, (uint8_t []){0x10}, 1, 0},
                {0xF0, (uint8_t []){0x00}, 1, 0},
                {0xF0, (uint8_t []){0x02}, 1, 0},
                {0xE0, (uint8_t []){0x70, 0x09, 0x12, 0x0C, 0x0B, 0x27, 0x38, 0x54, 0x4E, 0x19, 0x15, 0x15, 0x2C, 0x2F}, 14, 0},
                {0xE1, (uint8_t []){0x70, 0x08, 0x11, 0x0C, 0x0B, 0x27, 0x38, 0x43, 0x4C, 0x18, 0x14, 0x14, 0x2B, 0x2D}, 14, 0},
                {0xF0, (uint8_t []){0x10}, 1, 0},
                {0xF3, (uint8_t []){0x10}, 1, 0},
                {0xE0, (uint8_t []){0x0A}, 1, 0},
                {0xE1, (uint8_t []){0x00}, 1, 0},
                {0xE2, (uint8_t []){0x0B}, 1, 0},
                {0xE3, (uint8_t []){0x00}, 1, 0},
                {0xE4, (uint8_t []){0xE0}, 1, 0},
                {0xE5, (uint8_t []){0x06}, 1, 0},
                {0xE6, (uint8_t []){0x21}, 1, 0},
                {0xE7, (uint8_t []){0x00}, 1, 0},
                {0xE8, (uint8_t []){0x05}, 1, 0},
                {0xE9, (uint8_t []){0x82}, 1, 0},
                {0xEA, (uint8_t []){0xDF}, 1, 0},
                {0xEB, (uint8_t []){0x89}, 1, 0},
                {0xEC, (uint8_t []){0x20}, 1, 0},
                {0xED, (uint8_t []){0x14}, 1, 0},
                {0xEE, (uint8_t []){0xFF}, 1, 0},
                {0xEF, (uint8_t []){0x00}, 1, 0},
                {0xF8, (uint8_t []){0xFF}, 1, 0},
                {0xF9, (uint8_t []){0x00}, 1, 0},
                {0xFA, (uint8_t []){0x00}, 1, 0},
                {0xFB, (uint8_t []){0x30}, 1, 0},
                {0xFC, (uint8_t []){0x00}, 1, 0},
                {0xFD, (uint8_t []){0x00}, 1, 0},
                {0xFE, (uint8_t []){0x00}, 1, 0},
                {0xFF, (uint8_t []){0x00}, 1, 0},
                {0x60, (uint8_t []){0x42}, 1, 0},
                {0x61, (uint8_t []){0xE0}, 1, 0},
                {0x62, (uint8_t []){0x40}, 1, 0},
                {0x63, (uint8_t []){0x40}, 1, 0},
                {0x64, (uint8_t []){0x02}, 1, 0},
                {0x65, (uint8_t []){0x00}, 1, 0},
                {0x66, (uint8_t []){0x40}, 1, 0},
                {0x67, (uint8_t []){0x03}, 1, 0},
                {0x68, (uint8_t []){0x00}, 1, 0},
                {0x69, (uint8_t []){0x00}, 1, 0},
                {0x6A, (uint8_t []){0x00}, 1, 0},
                {0x6B, (uint8_t []){0x00}, 1, 0},
                {0x70, (uint8_t []){0x42}, 1, 0},
                {0x71, (uint8_t []){0xE0}, 1, 0},
                {0x72, (uint8_t []){0x40}, 1, 0},
                {0x73, (uint8_t []){0x40}, 1, 0},
                {0x74, (uint8_t []){0x02}, 1, 0},
                {0x75, (uint8_t []){0x00}, 1, 0},
                {0x76, (uint8_t []){0x40}, 1, 0},
                {0x77, (uint8_t []){0x03}, 1, 0},
                {0x78, (uint8_t []){0x00}, 1, 0},
                {0x79, (uint8_t []){0x00}, 1, 0},
                {0x7A, (uint8_t []){0x00}, 1, 0},
                {0x7B, (uint8_t []){0x00}, 1, 0},
                {0x80, (uint8_t []){0x38}, 1, 0},
                {0x81, (uint8_t []){0x00}, 1, 0},
                {0x82, (uint8_t []){0x04}, 1, 0},
                {0x83, (uint8_t []){0x02}, 1, 0},
                {0x84, (uint8_t []){0xDC}, 1, 0},
                {0x85, (uint8_t []){0x00}, 1, 0},
                {0x86, (uint8_t []){0x00}, 1, 0},
                {0x87, (uint8_t []){0x00}, 1, 0},
                {0x88, (uint8_t []){0x38}, 1, 0},
                {0x89, (uint8_t []){0x00}, 1, 0},
                {0x8A, (uint8_t []){0x06}, 1, 0},
                {0x8B, (uint8_t []){0x02}, 1, 0},
                {0x8C, (uint8_t []){0xDE}, 1, 0},
                {0x8D, (uint8_t []){0x00}, 1, 0},
                {0x8E, (uint8_t []){0x00}, 1, 0},
                {0x8F, (uint8_t []){0x00}, 1, 0},
                {0x90, (uint8_t []){0x38}, 1, 0},
                {0x91, (uint8_t []){0x00}, 1, 0},
                {0x92, (uint8_t []){0x08}, 1, 0},
                {0x93, (uint8_t []){0x02}, 1, 0},
                {0x94, (uint8_t []){0xE0}, 1, 0},
                {0x95, (uint8_t []){0x00}, 1, 0},
                {0x96, (uint8_t []){0x00}, 1, 0},
                {0x97, (uint8_t []){0x00}, 1, 0},
                {0x98, (uint8_t []){0x38}, 1, 0},
                {0x99, (uint8_t []){0x00}, 1, 0},
                {0x9A, (uint8_t []){0x0A}, 1, 0},
                {0x9B, (uint8_t []){0x02}, 1, 0},
                {0x9C, (uint8_t []){0xE2}, 1, 0},
                {0x9D, (uint8_t []){0x00}, 1, 0},
                {0x9E, (uint8_t []){0x00}, 1, 0},
                {0x9F, (uint8_t []){0x00}, 1, 0},
                {0xA0, (uint8_t []){0x38}, 1, 0},
                {0xA1, (uint8_t []){0x00}, 1, 0},
                {0xA2, (uint8_t []){0x03}, 1, 0},
                {0xA3, (uint8_t []){0x02}, 1, 0},
                {0xA4, (uint8_t []){0xDB}, 1, 0},
                {0xA5, (uint8_t []){0x00}, 1, 0},
                {0xA6, (uint8_t []){0x00}, 1, 0},
                {0xA7, (uint8_t []){0x00}, 1, 0},
                {0xA8, (uint8_t []){0x38}, 1, 0},
                {0xA9, (uint8_t []){0x00}, 1, 0},
                {0xAA, (uint8_t []){0x05}, 1, 0},
                {0xAB, (uint8_t []){0x02}, 1, 0},
                {0xAC, (uint8_t []){0xDD}, 1, 0},
                {0xAD, (uint8_t []){0x00}, 1, 0},
                {0xAE, (uint8_t []){0x00}, 1, 0},
                {0xAF, (uint8_t []){0x00}, 1, 0},
                {0xB0, (uint8_t []){0x38}, 1, 0},
                {0xB1, (uint8_t []){0x00}, 1, 0},
                {0xB2, (uint8_t []){0x07}, 1, 0},
                {0xB3, (uint8_t []){0x02}, 1, 0},
                {0xB4, (uint8_t []){0xDF}, 1, 0},
                {0xB5, (uint8_t []){0x00}, 1, 0},
                {0xB6, (uint8_t []){0x00}, 1, 0},
                {0xB7, (uint8_t []){0x00}, 1, 0},
                {0xB8, (uint8_t []){0x38}, 1, 0},
                {0xB9, (uint8_t []){0x00}, 1, 0},
                {0xBA, (uint8_t []){0x09}, 1, 0},
                {0xBB, (uint8_t []){0x02}, 1, 0},
                {0xBC, (uint8_t []){0xE1}, 1, 0},
                {0xBD, (uint8_t []){0x00}, 1, 0},
                {0xBE, (uint8_t []){0x00}, 1, 0},
                {0xBF, (uint8_t []){0x00}, 1, 0},
                {0xC0, (uint8_t []){0x22}, 1, 0},
                {0xC1, (uint8_t []){0xAA}, 1, 0},
                {0xC2, (uint8_t []){0x65}, 1, 0},
                {0xC3, (uint8_t []){0x74}, 1, 0},
                {0xC4, (uint8_t []){0x47}, 1, 0},
                {0xC5, (uint8_t []){0x56}, 1, 0},
                {0xC6, (uint8_t []){0x00}, 1, 0},
                {0xC7, (uint8_t []){0x88}, 1, 0},
                {0xC8, (uint8_t []){0x99}, 1, 0},
                {0xC9, (uint8_t []){0x33}, 1, 0},
                {0xD0, (uint8_t []){0x11}, 1, 0},
                {0xD1, (uint8_t []){0xAA}, 1, 0},
                {0xD2, (uint8_t []){0x65}, 1, 0},
                {0xD3, (uint8_t []){0x74}, 1, 0},
                {0xD4, (uint8_t []){0x47}, 1, 0},
                {0xD5, (uint8_t []){0x56}, 1, 0},
                {0xD6, (uint8_t []){0x00}, 1, 0},
                {0xD7, (uint8_t []){0x88}, 1, 0},
                {0xD8, (uint8_t []){0x99}, 1, 0},
                {0xD9, (uint8_t []){0x33}, 1, 0},
                {0xF3, (uint8_t []){0x01}, 1, 0},
                {0xF0, (uint8_t []){0x00}, 1, 0},
                {0xF0, (uint8_t []){0x01}, 1, 0},
                {0xF1, (uint8_t []){0x01}, 1, 0},
                {0xA0, (uint8_t []){0x0B}, 1, 0},
                {0xA3, (uint8_t []){0x2A}, 1, 0},
                {0xA5, (uint8_t []){0xC3}, 1, 1},
                {0xA3, (uint8_t []){0x2B}, 1, 0},
                {0xA5, (uint8_t []){0xC3}, 1, 1},
                {0xA3, (uint8_t []){0x2C}, 1, 0},
                {0xA5, (uint8_t []){0xC3}, 1, 1},
                {0xA3, (uint8_t []){0x2D}, 1, 0},
                {0xA5, (uint8_t []){0xC3}, 1, 1},
                {0xA3, (uint8_t []){0x2E}, 1, 0},
                {0xA5, (uint8_t []){0xC3}, 1, 1},
                {0xA3, (uint8_t []){0x2F}, 1, 0},
                {0xA5, (uint8_t []){0xC3}, 1, 1},
                {0xA3, (uint8_t []){0x30}, 1, 0},
                {0xA5, (uint8_t []){0xC3}, 1, 1},
                {0xA3, (uint8_t []){0x31}, 1, 0},
                {0xA5, (uint8_t []){0xC3}, 1, 1},
                {0xA3, (uint8_t []){0x32}, 1, 0},
                {0xA5, (uint8_t []){0xC3}, 1, 1},
                {0xA3, (uint8_t []){0x33}, 1, 0},
                {0xA5, (uint8_t []){0xC3}, 1, 1},
                {0xA0, (uint8_t []){0x09}, 1, 0},
                {0xF1, (uint8_t []){0x10}, 1, 0},
                {0xF0, (uint8_t []){0x00}, 1, 0},
                {0x2A, (uint8_t []){0x00, 0x00, 0x01, 0x67}, 4, 0},
                {0x2B, (uint8_t []){0x01, 0x68, 0x01, 0x68}, 4, 0},
                {0x4D, (uint8_t []){0x00}, 1, 0},
                {0x4E, (uint8_t []){0x00}, 1, 0},
                {0x4F, (uint8_t []){0x00}, 1, 0},
                {0x4C, (uint8_t []){0x01}, 1, 10},
                {0x4C, (uint8_t []){0x00}, 1, 0},
                {0x2A, (uint8_t []){0x00, 0x00, 0x01, 0x67}, 4, 0},
                {0x2B, (uint8_t []){0x00, 0x00, 0x01, 0x67}, 4, 0},
                {0x21, (uint8_t []){0x00}, 1, 0},
                {0x11, (uint8_t []){0x00}, 1, 120},

            };

        public:
            Panel_ST77916(void)
            {
                _cfg.memory_width  = _cfg.panel_width  = 360;
                _cfg.memory_height = _cfg.panel_height = 360;
            }

            bool init(bool use_reset) override;
            void beginTransaction(void) override;
            void endTransaction(void) override;

            color_depth_t setColorDepth(color_depth_t depth) override;
            //void setRotation(uint_fast8_t r) override;
            void setInvert(bool invert) override;
            void setSleep(bool flg) override;
            void setPowerSave(bool flg) override;

            void waitDisplay(void) override;
            bool displayBusy(void) override;

            void writePixels(pixelcopy_t* param, uint32_t len, bool use_dma) override;
            void writeBlock(uint32_t rawcolor, uint32_t len) override;

            void setWindow(uint_fast16_t xs, uint_fast16_t ys, uint_fast16_t xe, uint_fast16_t ye) override;
            void drawPixelPreclipped(uint_fast16_t x, uint_fast16_t y, uint32_t rawcolor) override;
            void writeFillRectPreclipped(uint_fast16_t x, uint_fast16_t y, uint_fast16_t w, uint_fast16_t h, uint32_t rawcolor) override;
            void writeImage(uint_fast16_t x, uint_fast16_t y, uint_fast16_t w, uint_fast16_t h, pixelcopy_t* param, bool use_dma) override;

            uint32_t readCommand(uint_fast16_t cmd, uint_fast8_t index, uint_fast8_t len) override;
            uint32_t readData(uint_fast8_t index, uint_fast8_t len) override;
            void readRect(uint_fast16_t x, uint_fast16_t y, uint_fast16_t w, uint_fast16_t h, void* dst, pixelcopy_t* param) override;

        protected:
            bool _in_transaction = false;

            void update_madctl(void);
            void write_cmd(uint8_t cmd);
            void start_qspi();
            void end_qspi();
            void write_bytes(const uint8_t* data, uint32_t len, bool use_dma);
        };

        //----------------------------------------------------------------------------
    }
}


#endif
