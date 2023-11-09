/*
 * enVac.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */
#pragma once
#ifndef AP__INC_ENVAC_HPP_
#define AP__INC_ENVAC_HPP_

#include "ap_def.hpp"

#ifdef _USE_APP_ENVAC 


constexpr uint16_t en_vac_default_latency_max = 2000;
constexpr uint16_t en_vac_default_latency_min = 100;

struct enVac
{
public:
  enum time_e
  {
    turn,
    settle
  };
  enum type_e
  {
    unkwn,
    suction,
    suction_blow,
  };
  ;

  enum act_e
  {
    on_suction,
    on_blow,
    _max
  };

  struct cfg_t
  {
  	uint8_t vac_id{};
  	std::array<uint32_t, act_e::_max> sol_io{};
  	uint32_t sensor_io{};
  	type_e vac_type{};
  	ap_io *pApIo{};
  	vac_dat::dat_t *pVacDat{};

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
  enVac() = default;
  enVac(cfg_t &cfg) : m_cfg(cfg)
  {
  }
  ~enVac() = default;

  enVac &operator=(const enVac &obj)
  {
    this->m_cfg = obj.m_cfg;
    return *this;
  }

  void SetConfigData(cfg_t &cfg)
  {
    m_cfg = cfg;
  }

  /*----------- Vacuum 상태 확인  -----------------------*/
  bool IsOn()
  {
    bool ret = false;
    if (m_cfg.sensor_io)
    {
      ret = m_cfg.pApIo->IsOn(m_cfg.sensor_io);
    }
    return ret;
  }

  bool IsOff()
  {
    bool ret = false;
    if (m_cfg.sensor_io)
    {
      ret = m_cfg.pApIo->IsOff(m_cfg.sensor_io);
    }
    return ret;
  }

  /*----------- Vacuum 동작 시작  -----------------------*/
  int BlowOn()
  {
    return m_cfg.pApIo->OutputOn(m_cfg.sol_io[act_e::on_blow]);
  }

  int BlowOff()
  {
    m_cfg.pApIo->OutputOn(m_cfg.sol_io[act_e::on_blow]);
    delay(50);
    m_cfg.pApIo->OutputOff(m_cfg.sol_io[act_e::on_blow]);
    return 0;
  }

  /*----------- Vacuum 동작  -----------------------*/
  int On(bool skip_sensor = false)
  {
    switch (m_cfg.vac_type)
    {
    case suction:
      m_cfg.pApIo->OutputOn(m_cfg.sol_io[act_e::on_suction]);
      break;
    case suction_blow:
      m_cfg.pApIo->OutputOn(m_cfg.sol_io[act_e::on_suction]);
      m_cfg.pApIo->OutputOff(m_cfg.sol_io[act_e::on_blow]);
      break;
    default:
      break;
    }

    if (skip_sensor)
    {
      return 0;
    }
    return Wait4OnCplt();
  }

  int Off(bool skip_sensor = false)
  {
    switch (m_cfg.vac_type)
    {
    case suction:
      m_cfg.pApIo->OutputOff(m_cfg.sol_io[act_e::on_suction]);
      break;
    case suction_blow:
      m_cfg.pApIo->OutputOff(m_cfg.sol_io[act_e::on_suction]);
      BlowOff();
      break;
    default:
      break;
    }

    if (skip_sensor)
    {
      return 0;
    }
    return Wait4OffCplt();
  }

  /*----------- Vacuum 동작완료시까지 Sleep  -----------------------*/
  int Wait4OnCplt()
  {
    int ret = -1;
    uint32_t pre_ms = millis();
    uint32_t timeout = constrain(m_cfg.pVacDat->timeout, en_vac_default_latency_min, en_vac_default_latency_max);

    while ((millis() - pre_ms) < timeout)
    {
      if (IsOn())
      {
        ret = 0;
        break;
      }
    }
    return ret;
  }

  int Wait4OffCplt()
  {
    int ret = -1;
    uint32_t pre_ms = millis();
    uint32_t timeout = constrain(m_cfg.pVacDat->timeout, en_vac_default_latency_min, en_vac_default_latency_max);

    while ((millis() - pre_ms) < timeout)
    {
      if (IsOff())
      {
        ret = 0;
        break;
      }
    }
    return ret;
  }
  /*------------------------ Common Interface -----------------------*/
  int SetData(vac_dat::dat_t *p_data)
  {
    m_cfg.pVacDat = p_data;
    return 0;
  }

  int GetData(vac_dat::dat_t *p_data)
  {
    p_data = m_cfg.pVacDat;
    return 0;
  }
};

#endif // _USE_APP_ENVAC

#endif /* AP__INC_ENVAC_HPP_ */
