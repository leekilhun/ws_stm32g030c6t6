/*
 * pca8575pw.h
 *
 *  Created on: 2022. 8. 27.
 *      Author: gns2l
 */

#ifndef SRC_COMMON_INC_EXHW_PCA8575PW_H_
#define SRC_COMMON_INC_EXHW_PCA8575PW_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "exhw_def.h"


#ifdef _USE_EXHW_PCA8575PW_EX_IO

	typedef enum
	{
		EX_IO_INPUT,
		EX_IO_OUTPUT,
	} ex_io_t;

	typedef struct
	{
		bool      is_open;
		ex_io_t   dev_type;
		uint8_t   dev_addr;
		uint16_t  port_state;
	}pca8575_ch_t;

	extern pca8575_ch_t data[_EXHW_PCA8575PW_EX_IO_MAX_CH];

	bool pca8575pw_Init();
	bool pca8575pw_Recovery();
	bool pca8575pw_Read(uint8_t ch, uint16_t* p_ret);
	bool pca8575pw_Write(uint8_t ch, uint16_t data);

#endif

#ifdef __cplusplus
}
#endif


#endif /* SRC_COMMON_INC_EXHW_PCA8575PW_H_ */
