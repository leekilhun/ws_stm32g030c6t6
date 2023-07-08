/*
 * ap_dat.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef AP__INC_AP_DAT_HPP_
#define AP__INC_AP_DAT_HPP_




/**
 * @brief Define the address of eeprom
 *
 * at24c64c - 32byte 256 page  0x0000  ~ 0x2000 *
 * 64K bit (8192 x 8)
 */

#define APDAT_START_ADDRESS                0x0000

#define APDAT_START_ADDR                   APDAT_START_ADDRESS
#define APDAT_LENGTH                       4
#define APDAT_ADDR(x)                      APDAT_START_ADDR + ((x) * APDAT_LENGTH)
#define APDAT_CNT_MAX                      8
#define APDAT_END_ADDR                     APDAT_START_ADDR + (APDAT_LENGTH * APDAT_CNT_MAX)


/*

sequece_idx_data_st idx_data size = 32
TaskEdit tasks m_step size = 72

*/
#define APDAT_SEQ_START_ADDR                   APDAT_END_ADDR
#define APDAT_SEQ_LENGTH                       32
#define APDAT_SEQ_ADDR(x)                      APDAT_SEQ_START_ADDR + ((x) * APDAT_SEQ_LENGTH)
#define APDAT_SEQ_CNT_MAX                      100
#define APDAT_SEQ_END_ADDR                     APDAT_SEQ_START_ADDR + (APDAT_SEQ_LENGTH * APDAT_SEQ_CNT_MAX)



// use machine test (2 byte * 16 data)
#define APDAT_SETUP_START_ADDR             APDAT_SEQ_END_ADDR
#define APDAT_SETUP_DATA_LENGTH            2
#define APDAT_SETUP_DATA_ADDR(x)           APDAT_SETUP_START_ADDR + ((x) * APDAT_SETUP_DATA_LENGTH)
#define APDAT_SETUP_DATA_CNT_MAX           16
#define APDAT_SETUP_END_ADDR               APDAT_SETUP_START_ADDR + (APDAT_SETUP_DATA_LENGTH * APDAT_SETUP_DATA_CNT_MAX)




struct ap_dat
{
  struct dat_t
  {
    uint16_t  parm1;
    uint16_t  parm2;
  };

  enum addr_e:uint8_t //
  {
    mt_x_turn_dist, mt_y_turn_dist, cfg_3, cfg_4 ,sycn_rate, default_accdec, cfg_7, cfg_8,
    _max
  };

  std::array<dat_t,APDAT_CNT_MAX> apcfg_dat;

  inline void WriteData(addr_e addr, dat_t& data){
    apcfg_dat[addr]=data;
    uint16_t rcv_data[2] = {data.parm1, data.parm2};
    at24c64Write(APDAT_ADDR(addr), (uint8_t *)&rcv_data[0], APDAT_LENGTH);
  }

  inline dat_t* ReadData(addr_e addr){
    return &apcfg_dat[addr];
  }

  inline dat_t* LoadData(addr_e addr){
    uint16_t rcv_data[2] = {0,};
    uint8_t idx = addr;
    at24c64Read(APDAT_ADDR(idx), (uint8_t*)&rcv_data, APDAT_LENGTH);
    apcfg_dat[idx].parm1 =rcv_data[0];
    apcfg_dat[idx].parm2 =rcv_data[1];
    return &apcfg_dat[idx];
  }

  inline dat_t* GetData(addr_e addr){
    return &apcfg_dat[addr];
  }

  inline bool LoadRomData(){
    uint16_t rcv_data[2] = {0,};
    bool ret;
    for (uint8_t i = 0; i < APDAT_CNT_MAX ;  i++) {
      ret = at24c64Read(APDAT_ADDR(i), (uint8_t*)&rcv_data, APDAT_LENGTH);
      apcfg_dat[i].parm1 =rcv_data[0];
      apcfg_dat[i].parm2 =rcv_data[1];
    }
    return ret;
  }

  inline bool ClearRomData(){
    uint8_t data[APDAT_LENGTH] = {0,};
    bool ret = false;
    for (uint8_t i = 0; i < APDAT_CNT_MAX ;  i++) {
      ret = at24c64Write(APDAT_ADDR(i), (uint8_t*)&data, APDAT_LENGTH);
    }
    return ret;
  }
};



struct AxisSystemXYZ
{
  uint16_t pos_x{}; // max position data 650mm
  uint16_t pos_y{}; // max position data 650mm
  uint16_t pos_z{}; // max position data 650mm

  AxisSystemXYZ() = default;
  ~AxisSystemXYZ() = default;
  // copy constructor
  AxisSystemXYZ(const AxisSystemXYZ& rhs) = default;
  // copy assignment operator
  AxisSystemXYZ& operator=(const AxisSystemXYZ& rhs) = default;
  // move constructor
  AxisSystemXYZ(AxisSystemXYZ&& rhs) = default;
  // move assignment operator
  AxisSystemXYZ& operator=(AxisSystemXYZ&& rhs) = default;

  void Init()
  {
    pos_x = 0;
    pos_y = 0;
    pos_z = 0;
  }
};

struct pos_data_st
{
  using type = AxisSystemXYZ;
  enum idx_e : uint8_t
  {
    mdi_null,
    mdi_0, //matrix data index
    mdi_1,
    mdi_2,
    mdi_3,

    mdi_253,
    mdi_max // max 255
  };

  /*position unit = 10um*/
  /*velocity unit =rpm */
  /*acc dec unit =m/ss */
  // max position data 650mm
  type data{};
  uint16_t rpm{};
  uint16_t acc{};
  uint16_t dec{};

  pos_data_st() = default;
  ~pos_data_st() = default;
  // copy constructor
  pos_data_st(const pos_data_st& rhs) = default;
  // copy assignment operator
  pos_data_st& operator=(const pos_data_st& rhs) = default;
  // move constructor
  pos_data_st(pos_data_st&& rhs) = default;
  // move assignment operator
  pos_data_st& operator=(pos_data_st&& rhs) = default;

  void Init()
  {
    data.Init();
    rpm = 0;
    acc = 0;
    dec = 0;
  }
};


/*
constexpr auto flash_total_size_kb = 128;
constexpr auto flash_boot_size_kb = 20;
constexpr auto flash_tag_size_kb = 1;
constexpr auto flash_data_size_kb = 5;
*/
constexpr auto flash_data_start_addr = 0x0801C000;
//int(FLASH_ADDR_START) + ( ( (flash_total_size_kb - flash_data_size_kb) - (flash_boot_size_kb + flash_tag_size_kb)) * 1024);
//#define flash_data_start_addr              (FLASH_ADDR_START + ((128 - 5)-(20 + 1) * 1024) 0x801ec00
#define flash_data_get_addr(x)                      flash_data_start_addr + ((x) * APDAT_SEQ_LENGTH)
struct sequece_idx_data_st
{
  enum linetype_e : uint8_t
  {
    lt_sequence,
    lt_pos,
  };

  uint16_t idx{};
  uint16_t next_line{};

  linetype_e line_type{};
  pos_data_st::idx_e pos_data_idx{}; //

  uint32_t entry_setout{};
  uint32_t exit_setout{};

  uint32_t entry_delay{}; // ms
  uint32_t exit_delay{};  // ms

  uint32_t condition_in{};
  uint16_t condition_pass_line{};
  uint16_t condition_fail_line{};

  sequece_idx_data_st() = default;
  ~sequece_idx_data_st() = default;
  // copy constructor
  sequece_idx_data_st(const sequece_idx_data_st& rhs) = default;
  // copy assignment operator
  sequece_idx_data_st& operator=(const sequece_idx_data_st& rhs) = default;
  // move constructor
  sequece_idx_data_st(sequece_idx_data_st&& rhs) = default;
  // move assignment operator
  sequece_idx_data_st& operator=(sequece_idx_data_st&& rhs) = default;
};
// end of sequece_idx_data_st


struct taskDat
{

  struct dat_t
  {
    uint16_t  index;
    sequece_idx_data_st  line_data;
  };

  enum idx_e :uint8_t //
  {
    idx_curr, idx_frnt1, idx_frnt2, idx_frnt3, idx_frnt4, _max
  };

  std::array<dat_t, _max> task_dat;

  inline bool WriteData(uint16_t index, sequece_idx_data_st& data) {

    if (index < APDAT_SEQ_CNT_MAX)
    {
      //LOG_PRINT("index [%d] addr [%d]", index,  flash_data_get_addr(index));
      //return at24c64Write(APDAT_SEQ_ADDR(index), (uint8_t*)&data, APDAT_SEQ_LENGTH);
      return flashWrite(flash_data_get_addr(index), (uint8_t*)&data, APDAT_SEQ_LENGTH);

    }
    return false;
  }

  inline bool ReadData() {
    return flashRead(flash_data_get_addr(0), (uint8_t*)task_dat.data(), task_dat.size() * APDAT_SEQ_LENGTH);
  }

};







#endif /* AP__INC_AP_DAT_HPP_ */
