/*
 * machine_test.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef AP__INC_MACHINE_TEST_HPP_
#define AP__INC_MACHINE_TEST_HPP_

#ifdef _USE_AP_MACHINE_TEST


constexpr uint16_t MACHINE_INITAIAL_STATE = 0xffff;

class machine_test
{

public:
	enum addr_e:uint8_t //
	{
		data_reg,//0, initial machine state 0xffff
		data1,//1,
		data2,//2,
		data3,//3,
		data4,//4,
		data5,//5,
		data6,//6,
		data7,//7
		data8,//8,
		data9,//9,
		data10,//,
		data11,//,
		data12,//,
		data13,//,
		data14,
		data15,
		_max
	};

	struct cfg_t {

		cfg_t() = default;

		cfg_t& operator = (const cfg_t& cfg){
			return *this;
		}

	};
public:
	bool m_IsInitialState;
	cfg_t m_cfg;
	prc_step_t m_step;
	prc_step_t m_TestStep;

	int m_result;
	uint8_t m_cnt;
	bool m_IsTestRun;
	/****************************************************
	 *  Constructor
	 ****************************************************/
public:
	machine_test():  m_IsInitialState{}, m_cfg{}, m_step{},
	m_TestStep{} ,m_result{}, m_cnt{} ,m_IsTestRun{}{

	}

	virtual ~machine_test (){}

	/****************************************************
	 *  func
	 ****************************************************/

	inline void Init(cfg_t &cfg) {
		m_cfg = cfg;
		uint16_t rcv_data = 0;
		at24c64Read(APDAT_SETUP_DATA_ADDR(data_reg), (uint8_t*)&rcv_data, APDAT_SETUP_DATA_LENGTH);
		m_IsInitialState = (rcv_data == MACHINE_INITAIAL_STATE);
	}

  inline bool IsInitMachine() const{
  	return m_IsInitialState;
  }

  inline bool SelfTestCplt(){
  	constexpr uint16_t completed_self_test = 0;
  	uint16_t rcv_data = completed_self_test;
  	m_IsInitialState = false;
  	return at24c64Write(APDAT_SETUP_DATA_ADDR(data_reg), (uint8_t *)&rcv_data, APDAT_SETUP_DATA_LENGTH);

  }

  inline bool ResetTestCplt(){
  	uint16_t rcv_data = MACHINE_INITAIAL_STATE;
  	m_IsInitialState = true;
  	return at24c64Write(APDAT_SETUP_DATA_ADDR(data_reg), (uint8_t *)&rcv_data, APDAT_SETUP_DATA_LENGTH);
  }

};


#endif


#endif /* AP__INC_MACHINE_TEST_HPP_ */
