/**
 * @file iic_loadcell.hpp
 * @author lee kil hun (pba7189@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#ifndef __IIC_LOADCELL_HPP__
#define __IIC_LOADCELL_HPP__

#include "ap_def.hpp"

#ifdef _USE_APP_IICLOADCELL

/* i2c 통신
https://docs.m5stack.com/en/unit/Unit-Mini%20Scales
*/
struct iicLoadcell
{
  // I2C
static constexpr uint8_t DEVICE_ADDR = 0x26;
// SCALES REGISTER
static constexpr uint8_t UNIT_SCALES_RAW_ADC_REG = 0x00;
static constexpr uint8_t UNIT_SCALES_CAL_DATA_REG = 0x10;
static constexpr uint8_t UNIT_SCALES_BUTTON_REG = 0x20;
static constexpr uint8_t UNIT_SCALES_RGB_LED_REG = 0x30;
static constexpr uint8_t UNIT_SCALES_SET_GAP_REG = 0x40;
static constexpr uint8_t UNIT_SCALES_SET_OFFESET_REG = 0x50;
static constexpr uint8_t UNIT_SCALES_CAL_DATA_INT_REG = 0x60;
static constexpr uint8_t UNIT_SCALES_CAL_DATA_STRING_REG = 0x70;
static constexpr uint8_t UNIT_SCALES_FILTER_REG = 0x80;
static constexpr uint8_t JUMP_TO_BOOTLOADER_REG = 0xFD;
static constexpr uint8_t FIRMWARE_VERSION_REG = 0xFE;
static constexpr uint8_t I2C_ADDRESS_REG = 0xFF;

 struct cfg_t
  {
    uint8_t i2c_ch{};
    uint16_t device_addr{DEVICE_ADDR};

    cfg_t() = default;
    // copy constructor
    cfg_t(const cfg_t& rhs) = default;
    // copy assignment operator
    cfg_t& operator=(const cfg_t& rhs) = default;
    // move constructor
    cfg_t(cfg_t&& rhs) = default;
    // move assignment operator
    cfg_t& operator=(cfg_t&& rhs) = default;

    ~cfg_t() = default;

  } m_cfg{};

  bool is_opened{false};

public:
  iicLoadcell() = default;
  ~iicLoadcell()= default;
  inline error_t Init(cfg_t& cfg)
  {
    if (i2cBegin(cfg.i2c_ch, i2c_freq_t::I2C_FREQ_400KHz))
    {
      LOG_PRINT("Init Success! i2c_ch [%d]", cfg.i2c_ch);
    }
    else
    {
      LOG_PRINT("Init Failed! i2c_ch [%d]", cfg.i2c_ch);
      return ERROR_FAIL;
    }
    is_opened = true;
    m_cfg     = cfg;
    return ERROR_SUCCESS;
  }

  

  inline bool ReadByte(uint8_t reg_addr, uint8_t *p_data, uint32_t timeout){
    return i2cReadByte(m_cfg.i2c_ch, m_cfg.device_addr, reg_addr, p_data, timeout);
  }

  inline bool ReadBytes(uint8_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout){
    return i2cReadBytes(m_cfg.i2c_ch, m_cfg.device_addr, reg_addr, p_data, length, timeout);
  }

  inline bool WriteByte(uint8_t reg_addr, uint8_t data, uint32_t timeout){
    return i2cWriteByte(m_cfg.i2c_ch, m_cfg.device_addr, reg_addr, data, timeout);
  }

  inline bool WriteBytes(uint8_t reg_addr, uint8_t *p_data, uint32_t length, uint32_t timeout){
    return i2cWriteBytes(m_cfg.i2c_ch, m_cfg.device_addr, reg_addr, p_data, length, timeout);
  }


  inline int32_t get_raw_adc()
  {
    uint8_t data[4]{};
    int32_t ret = 0;
    if (ReadBytes(UNIT_SCALES_RAW_ADC_REG, data, 4, 100))
      ret = (int32_t)(data[0]  | data[1] << 8 | data[2] << 16 | data[3] << 24);
    return ret;
  }

  inline float get_weight()
  {
    uint8_t data[4]{};
    float ret{};
    uint8_t *p;
    if (ReadBytes(UNIT_SCALES_CAL_DATA_REG, data, 4, 100))
    {
      p = (uint8_t *)&ret;
      memcpy(p, data, 4);
    }
    return ret;
  }

  inline int32_t get_weight_int()
  {
    uint8_t data[4]{};
    int32_t ret = 0;
    if (ReadBytes(UNIT_SCALES_CAL_DATA_INT_REG, data, 4, 100))
    {
      ret = (int32_t)(data[0] << 0 | data[1] << 8 | data[2] << 16 | data[3] << 24);
    }
    else
    {
      is_opened = false;
    }
    return ret;
  }

  inline std::string get_weight_string()
  {
    uint8_t data[16]{};
    if (ReadBytes(UNIT_SCALES_CAL_DATA_STRING_REG, data, 16, 100))
    {
      return std::string((char *)data);
    }
    return std::string{};
  }

  inline float get_gap_value(){
    uint8_t data[4]{};
    float ret{};
    uint8_t *p;
    if (ReadBytes(UNIT_SCALES_SET_GAP_REG, data, 4, 100))
    {
      p = (uint8_t *)&ret;
      memcpy(p, data, 4);
    }
    return ret;
  }

  inline void set_gap_value(float offset){
    uint8_t data[4]{};
    uint8_t *p;
    p = (uint8_t *)&offset;
    memcpy(data, p, 4);
    WriteBytes(UNIT_SCALES_SET_GAP_REG, data, 4, 100);
    // delay(100);
  }

  inline void set_offset(void){
    uint8_t data[4]{};
    data[0] = 0x01;
    WriteBytes(UNIT_SCALES_SET_OFFESET_REG, data, 1, 100);
  }

  inline bool set_led_color(uint32_t set_color){
   uint8_t color[4] = {0};
    // RED
    color[0] = (set_color >> 16) & 0xff;
    // GREEN
    color[1] = (set_color >> 8) & 0xff;
    // BLUE
    color[2] = set_color & 0xff;
    return WriteBytes(UNIT_SCALES_RGB_LED_REG, color, 3, 100);
  }

  inline uint32_t get_led_color(){
     uint8_t color[4]  = {0};
    uint32_t color_hex = 0;
    if (ReadBytes(UNIT_SCALES_RGB_LED_REG, color, 3, 100)) 
    {
        color_hex = color[0];
        color_hex = (color_hex << 8) | color[1];
        color_hex = (color_hex << 8) | color[2];
    };
    return color_hex;
  }

  inline uint8_t get_btn_status(){
    uint8_t data = 0;
    ReadByte(UNIT_SCALES_BUTTON_REG, &data, 100);
    return data;
  }

  inline uint8_t get_firmware_version(void)
  {
    uint8_t data = 0;
    ReadByte(FIRMWARE_VERSION_REG, &data, 100);
    return data;
  }

  inline uint8_t set_i2c_address(uint8_t addr)
  {
    uint8_t data = addr;
    if(WriteByte(I2C_ADDRESS_REG, data, 100))
    {
      if (get_firmware_version() == data)
      {
        m_cfg.device_addr = addr;
      }
      else
      {
        data = 0;
      }
    }
    return data;
  }

  inline uint8_t get_i2c_address(void)
  {
    uint8_t data = 0;
    ReadByte(I2C_ADDRESS_REG, &data, 100);
    return data;
  }

  inline bool set_lp_filter(uint8_t en)
  {
    uint8_t data = en;
    return WriteByte(UNIT_SCALES_FILTER_REG, data, 100);
  }

  inline uint8_t get_lp_filter(void)
  {
    uint8_t data = 0;
    ReadByte(UNIT_SCALES_FILTER_REG, &data, 100);
    return data;
  }

  inline bool set_avg_filter(uint8_t avg)
  {
    uint8_t data = avg;
    return WriteByte(UNIT_SCALES_FILTER_REG, data, 100);
  }

  inline uint8_t get_avg_filter(void)
  {
    uint8_t data = 0;
    ReadByte(UNIT_SCALES_FILTER_REG, &data, 100);
    return data;
  }

  inline bool set_ema_filter(uint8_t ema)
  {
    uint8_t data = ema;
    return WriteByte(UNIT_SCALES_FILTER_REG, data, 100);
  }

  inline uint8_t get_ema_filter(void)
  {
    uint8_t data = 0;
    ReadByte(UNIT_SCALES_FILTER_REG, &data, 100);
    return data;
  }

  inline void jump_bootloader(void)
  {
    uint8_t data = 0x01;
    WriteByte(JUMP_TO_BOOTLOADER_REG, data, 100);
  }

};

#endif
// end of _USE_APP_IICLOADCELL
#endif //__IIC_LOADCELL_HPP__
