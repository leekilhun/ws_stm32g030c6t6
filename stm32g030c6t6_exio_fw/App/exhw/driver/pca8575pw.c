/*
 * pca8575pw.c
 *
 *  Created on: 2022. 8. 27.
 *      Author: gns2l
 */


#include "pca8575pw.h"
#include "cli.h"


#ifdef _USE_EXHW_PCA8575PW_EX_IO

#define PCA8575PW_I2C           _DEF_I2C1
#define PCA8575PW_CH_MAX        _EXHW_PCA8575PW_EX_IO_MAX_CH
#ifdef _USE_HW_CLI
void cliPca8575_exIo(cli_args_t* args);
#endif

static bool pca8575pw_ReadPort(uint8_t ch, uint8_t* p_ret);
static bool pca8575pw_WritePort(uint8_t ch, uint8_t port0_7, uint8_t port8_15);
static void pca8575pw_update();

pca8575_ch_t pca8575pw_exIo[PCA8575PW_CH_MAX] = {
		{ false, EX_IO_INPUT,  0x23,  0},
		{ false, EX_IO_OUTPUT, 0x27,  0},
};

static bool is_init = false;

bool pca8575pw_Init()
{
	is_init = i2cBegin(PCA8575PW_I2C, I2C_FREQ_400KHz);

	for (int i = 0; i < PCA8575PW_CH_MAX; i++)
	{
		pca8575pw_exIo[i].is_open = true;
	}

	//gpioAttachCallbackFunc(_GPIO_I2C_INTERRUPT, pca8575pw_update);
	//

#ifdef _USE_HW_CLI
	cliAdd("pca8575_exIo", cliPca8575_exIo);
#endif

	return is_init;
}

bool pca8575pw_Recovery()
{
	return i2cRecovery(PCA8575PW_I2C);
}

bool pca8575pw_ReadPort(uint8_t ch, uint8_t* p_ret)
{
	bool ret = false;

	if (ch < PCA8575PW_CH_MAX)
		ret = i2cReadData(PCA8575PW_I2C, pca8575pw_exIo[ch].dev_addr, p_ret, 2, 100);

	return ret;
}

bool pca8575pw_WritePort(uint8_t ch, uint8_t port0_7, uint8_t port8_15)
{
	bool i2c_ret = false;
	if (ch < PCA8575PW_CH_MAX)
	{
		if (pca8575pw_exIo[ch].dev_type != EX_IO_OUTPUT)
			return i2c_ret;

		i2c_ret = i2cWriteByte(PCA8575PW_I2C, pca8575pw_exIo[ch].dev_addr, port0_7, port8_15, 100);
	}

	return i2c_ret;
}



bool pca8575pw_Read(uint8_t ch, uint16_t* p_ret)
{
	bool ret = false;
	if (ch < PCA8575PW_CH_MAX)
	{
		pca8575pw_update();

		*p_ret = pca8575pw_exIo[ch].port_state;
		ret = true;
	}
	return ret;
}



bool pca8575pw_Write(uint8_t ch, uint16_t data)
{
	bool ret = false;
	if (ch < PCA8575PW_CH_MAX)
	{
		if (pca8575pw_exIo[ch].dev_type == EX_IO_OUTPUT)
		{
			pca8575pw_exIo[ch].port_state = data;
			uint8_t port_0_7 = (uint8_t)data;
			uint8_t port_8_15 = (uint8_t)(data >> 8);
			pca8575pw_WritePort(ch, port_0_7, port_8_15);
			ret = true;
		}
	}
	return ret;
}


void pca8575pw_update()
{
	uint16_t reverse_in_data = 0, out_data = 0;
	pca8575pw_ReadPort(_PCA8575PW_EX_IO_CH_IN, (uint8_t*)&reverse_in_data);
	pca8575pw_exIo[_PCA8575PW_EX_IO_CH_IN].port_state = ~(reverse_in_data);

	pca8575pw_ReadPort(_PCA8575PW_EX_IO_CH_OUT, (uint8_t*)&out_data);
	pca8575pw_exIo[_PCA8575PW_EX_IO_CH_OUT].port_state = ~(out_data);
}




#ifdef _USE_HW_CLI

void cliPca8575_exIo(cli_args_t* args)
{
	bool ret = true;
	bool pca_ret = false;
	uint8_t print_ch;
	uint8_t ch;

	uint8_t ret_data[2] = { 0, };
	if (args->argc == 2)
	{
		if (args->isStr(0, "read") == true)
		{
			print_ch = (uint16_t)args->getData(1);

			print_ch = constrain(print_ch, 1, PCA8575PW_CH_MAX);
			ch = print_ch - 1;

			pca_ret = pca8575pw_ReadPort(ch, &ret_data[0]);

			if (pca_ret == true)
			{
				cliPrintf("%d PCA8575 READ : 0x%02X, 0x%02X\n", print_ch, ret_data[0], ret_data[1]);

			}
			else
			{
				cliPrintf("%d PCA8575 READ - Fail \n", print_ch);
			}


		}
		else
		{
			ret = false;
		}
	}
	else if (args->argc == 4)
	{
		if (args->isStr(0, "write") == true)
		{
			print_ch = (uint16_t)args->getData(1);
			print_ch = constrain(print_ch, 1, PCA8575PW_CH_MAX);
			uint8_t port_0_7;
			uint8_t port_8_15;
			port_0_7 = args->getData(2);
			port_8_15 = args->getData(3);
			ch = print_ch - 1;

			pca_ret = pca8575pw_WritePort(ch, port_0_7, port_8_15);

			if (pca_ret == true)
			{
				cliPrintf("%d PCA8575 WRITE : 0x%02X, 0x%02X\n", print_ch, port_0_7, port_8_15);

			}
			else
			{
				cliPrintf("%d PCA8575 WRITE - Fail \n", print_ch);
			}

		}

	}
	else
	{
		ret = false;
	}

	if (ret == false)
	{
		cliPrintf("pca8575_exIo read [ch 1~2]\n");
		cliPrintf("pca8575_exIo write [ch 1~2] [port0_7] [port8_15]\n");
	}
}

#endif


















#endif
