/*
 * enCyl.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */
#pragma once
#ifndef AP__INC_ENCYL_HPP_
#define AP__INC_ENCYL_HPP_

#include "ap_def.hpp"

#ifdef _USE_APP_ENCYL

constexpr uint16_t en_cyl_default_latency_max = 2000;
constexpr uint16_t en_cyl_default_latency_min = 100;

struct enCyl
{
public:
  enum time_e
  {
    move,
    settle
  };
  // solvalve on state / off state
  enum type_e
  {
    unknwn,
    up_down,
    left_right,
    forward_backward,
    turn_return,
    lock_unlock,
    open_close,
    up_mid_down,
    left_mid_right,
    front_mid_back,
  };
  enum sol_e
  {
    one,
    reserse_one,
    two,
    reverse_two
  };

  enum dir_e
  {
    up,
    down,
    _max
  };

  struct cfg_t
  {
  	uint8_t cyl_id{};
  	std::array<uint32_t, dir_e::_max> sol_io{};
  	std::array<uint32_t, dir_e::_max> sensor_io{};
  	type_e cyl_type{};
  	sol_e sol_type{};
  	ap_io *pApIo{};
  	cyl_dat::dat_t *pCylDat{};

  	cfg_t() = default;
  	// copy constructor
  	cfg_t(const cfg_t& other) = default;
  	// copy assignment
  	cfg_t& operator=(const cfg_t& other) = default;
  	// move constructor
  	cfg_t(cfg_t&& other) = default;
  	// move assignment
  	cfg_t& operator=(cfg_t&& other) = default;
  };

private:
  cfg_t m_cfg;

public:
  enCyl() = default;
  enCyl(cfg_t &cfg) : m_cfg(cfg)
  {
  }
  ~enCyl() = default;

  enCyl &operator=(const enCyl &obj)
  {
    this->m_cfg = obj.m_cfg;
    return *this;
  }

  void SetConfigData(cfg_t &cfg)
  {
    m_cfg = cfg;
  }
  /*----------- 실린더 상태 확인  -----------------------*/
  bool IsUp()
  {
    bool ret[dir_e::_max] = {
        false,
    };

    // UP Sensor가 On인지 확인
    if (m_cfg.sensor_io[dir_e::up])
      ret[dir_e::up] = m_cfg.pApIo->IsOn(m_cfg.sensor_io[dir_e::up]);

    // Down Sensor가 Off인지 확인
    if (m_cfg.sensor_io[dir_e::down])
      ret[dir_e::down] = m_cfg.pApIo->IsOff(m_cfg.sensor_io[dir_e::down]);

    //
    if (!ret[dir_e::up] || !ret[dir_e::down])
      return false;

    return true;
  }

  bool IsDown()
  {
    bool ret[dir_e::_max] = {
        false,
    };

    // UP Sensor가 On인지 확인
    if (m_cfg.sensor_io[dir_e::up])
      ret[dir_e::up] = m_cfg.pApIo->IsOff(m_cfg.sensor_io[dir_e::up]);

    // Down Sensor가 Off인지 확인
    if (m_cfg.sensor_io[dir_e::down])
      ret[dir_e::down] = m_cfg.pApIo->IsOn(m_cfg.sensor_io[dir_e::down]);

    //
    if (!ret[dir_e::up] || !ret[dir_e::down])
      return false;

    return true;
  }

  bool IsLeft()
  {
    return IsUp();
  }

  bool IsRight()
  {
    return IsDown();
  }

  bool IsForward()
  {
    return IsUp();
  }

  bool IsBackward()
  {
    return IsDown();
  }
  bool IsOpen()
  {
    return IsUp();
  }

  bool IsClose()
  {
    return IsDown();
  }

  bool IsLock()
  {
    return IsUp();
  }

  bool IsUnlock()
  {
    return IsDown();
  }

  bool IsTurn()
  {
    return IsUp();
  }

  bool IsReturn()
  {
    return IsDown();
  }
  /*----------- 실린더 이동   -----------------------*/

  int Up(bool skip_sensor = false)
  {
    switch (m_cfg.sol_type)
    {
    case one:
      m_cfg.pApIo->OutputOn(m_cfg.sol_io[dir_e::up]);
      break;
    case reserse_one:
      m_cfg.pApIo->OutputOff(m_cfg.sol_io[dir_e::up]);
      break;
    case two:
      m_cfg.pApIo->OutputOn(m_cfg.sol_io[dir_e::up]);
      m_cfg.pApIo->OutputOff(m_cfg.sol_io[dir_e::down]);
      break;
    default:
      break;
    }

    if (skip_sensor)
    {
      return 0;
    }
    return Wait4UpCplt();
  }

  int Down(bool skip_sensor = false)
  {
    switch (m_cfg.sol_type)
    {
    case one:
      m_cfg.pApIo->OutputOn(m_cfg.sol_io[dir_e::down]);
      break;
    case reserse_one:
      m_cfg.pApIo->OutputOff(m_cfg.sol_io[dir_e::down]);
      break;
    case two:
      m_cfg.pApIo->OutputOff(m_cfg.sol_io[dir_e::up]);
      m_cfg.pApIo->OutputOn(m_cfg.sol_io[dir_e::down]);
      break;
    default:
      break;
    }

    if (skip_sensor)
    {
      return 0;
    }
    return Wait4DownCplt();
  }

  int Left(bool skip_sensor = false)
  {
    return Up(skip_sensor);
  }

  int Right(bool skip_sensor = false)
  {
    return Down(skip_sensor);
  }

  int Forward(bool skip_sensor = false)
  {
    return Up(skip_sensor);
  }

  int Backward(bool skip_sensor = false)
  {
    return Down(skip_sensor);
  }

  int Open(bool skip_sensor = false)
  {
    return Up(skip_sensor);
  }

  int Close(bool skip_sensor = false)
  {
    return Down(skip_sensor);
  }

  int Lock(bool skip_sensor = false)
  {
    return Up(skip_sensor);
  }

  int Unlock(bool skip_sensor = false)
  {
    return Down(skip_sensor);
  }

  int Return(bool skip_sensor = false)
  {
    return Up(skip_sensor);
  }

  int Turn(bool skip_sensor = false)
  {
    return Down(skip_sensor);
  }

  /*----------- 실린더 이동 시작  -----------------------*/
  int Start()
  {
    return Up(true);
  }

  /*---------- 실린더 동작완료때까지 Sleep   --------------*/
  int Wait4UpCplt()
  {
    int ret = -1;
    uint32_t pre_ms = millis();
    uint32_t timeout = constrain(m_cfg.pCylDat->timeout, en_cyl_default_latency_min, en_cyl_default_latency_max);
    while ((millis() - pre_ms) < timeout)
    {
      if (IsUp())
      {
        ret = 0;
        break;
      }
    }
    return ret;
  }

  int Wait4DownCplt()
  {
    int ret = -1;
    uint32_t pre_ms = millis();
    uint32_t timeout = constrain(m_cfg.pCylDat->timeout, en_cyl_default_latency_min, en_cyl_default_latency_max);

    while ((millis() - pre_ms) < timeout)
    {
      if (IsDown())
      {
        ret = 0;
        break;
      }
    }
    return ret;
  }

  /*------------------------ Common Interface -----------------------*/

  int SetData(cyl_dat::dat_t *p_data)
  {
    m_cfg.pCylDat = p_data;
    return 0;
  }

  int GetData(cyl_dat::dat_t *p_data)
  {
    p_data = m_cfg.pCylDat;
    return 0;
  }
};

#endif /* _USE_APP_ENCYL */


#endif /* AP__INC_ENCYL_HPP_ */
