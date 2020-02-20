#include "../utility/debug.h"
#include "../component/cc112x.h"
#include "../component/cc112x_spi.h"

// I don't expect this to be light on memory, but atleast printing will be easy?
const char register_names[][30] = {
	"CC112X_IOCFG2",
	"CC112X_IOCFG1",
	"CC112X_IOCFG0",
	"CC112X_SYNC3",
	"CC112X_SYNC2",
	"CC112X_SYNC1",
	"CC112X_SYNC0",
	"CC112X_SYNC_CFG1",
	"CC112X_SYNC_CFG0",
	"CC112X_DEVIATION_M",
	"CC112X_MODCFG_DEV_E",
	"CC112X_DCFILT_CFG",
	"CC112X_PREAMBLE_CFG1",
	"CC112X_PREAMBLE_CFG0",
	"CC112X_FREQ_IF_CFG",
	"CC112X_IQIC",
	"CC112X_CHAN_BW",
	"CC112X_MDMCFG1",
	"CC112X_MDMCFG0",
	"CC112X_SYMBOL_RATE2",
	"CC112X_SYMBOL_RATE1",
	"CC112X_SYMBOL_RATE0",
	"CC112X_AGC_REF",
	"CC112X_AGC_CS_THR",
	"CC112X_AGC_GAIN_ADJUST",
	"CC112X_AGC_CFG3",
	"CC112X_AGC_CFG2",
	"CC112X_AGC_CFG1",
	"CC112X_AGC_CFG0",
	"CC112X_FIFO_CFG",
	"CC112X_DEV_ADDR",
	"CC112X_SETTLING_CFG",
	"CC112X_FS_CFG",
	"CC112X_WOR_CFG1",
	"CC112X_WOR_CFG0",
	"CC112X_WOR_EVENT0_MSB",
	"CC112X_WOR_EVENT0_LSB",
	"CC112X_PKT_CFG2",
	"CC112X_PKT_CFG1",
	"CC112X_PKT_CFG0",
	"CC112X_RFEND_CFG1",
	"CC112X_RFEND_CFG0",
	"CC112X_PA_CFG2",
	"CC112X_PA_CFG1",
	"CC112X_PA_CFG0",
	"CC112X_PKT_LEN",
	"CC112X_IF_MIX_CFG",
	"CC112X_FREQOFF_CFG",
	"CC112X_TOC_CFG",
	"CC112X_MARC_SPARE",
	"CC112X_ECG_CFG",
	"CC112X_CFM_DATA_CFG",
	"CC112X_EXT_CTRL",
	"CC112X_RCCAL_FINE",
	"CC112X_RCCAL_COARSE",
	"CC112X_RCCAL_OFFSET",
	"CC112X_FREQOFF1",
	"CC112X_FREQOFF0",
	"CC112X_FREQ2",
	"CC112X_FREQ1",
	"CC112X_FREQ0",
	"CC112X_IF_ADC2",
	"CC112X_IF_ADC1",
	"CC112X_IF_ADC0",
	"CC112X_FS_DIG1",
	"CC112X_FS_DIG0",
	"CC112X_FS_CAL3",
	"CC112X_FS_CAL2",
	"CC112X_FS_CAL1",
	"CC112X_FS_CAL0",
	"CC112X_FS_CHP",
	"CC112X_FS_DIVTWO",
	"CC112X_FS_DSM1",
	"CC112X_FS_DSM0",
	"CC112X_FS_DVC1",
	"CC112X_FS_DVC0",
	"CC112X_FS_LBI",
	"CC112X_FS_PFD",
	"CC112X_FS_PRE",
	"CC112X_FS_REG_DIV_CML",
	"CC112X_FS_SPARE",
	"CC112X_FS_VCO4",
	"CC112X_FS_VCO3",
	"CC112X_FS_VCO2",
	"CC112X_FS_VCO1",
	"CC112X_FS_VCO0",
	"CC112X_GBIAS6",
	"CC112X_GBIAS5",
	"CC112X_GBIAS4",
	"CC112X_GBIAS3",
	"CC112X_GBIAS2",
	"CC112X_GBIAS1",
	"CC112X_GBIAS0",
	"CC112X_IFAMP",
	"CC112X_LNA",
	"CC112X_RXMIX",
	"CC112X_XOSC5",
	"CC112X_XOSC4",
	"CC112X_XOSC3",
	"CC112X_XOSC2",
	"CC112X_XOSC1",
	"CC112X_XOSC0",
	"CC112X_ANALOG_SPARE",
	"CC112X_PA_CFG3",
	"CC112X_IRQ0M",
	"CC112X_IRQ0F",
	"CC112X_WOR_TIME1",
	"CC112X_WOR_TIME0",
	"CC112X_WOR_CAPTURE1",
	"CC112X_WOR_CAPTURE0",
	"CC112X_BIST",
	"CC112X_DCFILTOFFSET_I1",
	"CC112X_DCFILTOFFSET_I0",
	"CC112X_DCFILTOFFSET_Q1",
	"CC112X_DCFILTOFFSET_Q0",
	"CC112X_IQIE_I1",
	"CC112X_IQIE_I0",
	"CC112X_IQIE_Q1",
	"CC112X_IQIE_Q0",
	"CC112X_RSSI1",
	"CC112X_RSSI0",
	"CC112X_MARCSTATE",
	"CC112X_LQI_VAL",
	"CC112X_PQT_SYNC_ERR",
	"CC112X_DEM_STATUS",
	"CC112X_FREQOFF_EST1",
	"CC112X_FREQOFF_EST0",
	"CC112X_AGC_GAIN3",
	"CC112X_AGC_GAIN2",
	"CC112X_AGC_GAIN1",
	"CC112X_AGC_GAIN0",
	"CC112X_CFM_RX_DATA_OUT",
	"CC112X_CFM_TX_DATA_IN",
	"CC112X_ASK_SOFT_RX_DATA",
	"CC112X_RNDGEN",
	"CC112X_MAGN2",
	"CC112X_MAGN1",
	"CC112X_MAGN0",
	"CC112X_ANG1",
	"CC112X_ANG0",
	"CC112X_CHFILT_I2",
	"CC112X_CHFILT_I1",
	"CC112X_CHFILT_I0",
	"CC112X_CHFILT_Q2",
	"CC112X_CHFILT_Q1",
	"CC112X_CHFILT_Q0",
	"CC112X_GPIO_STATUS",
	"CC112X_FSCAL_CTRL",
	"CC112X_PHASE_ADJUST",
	"CC112X_PARTNUMBER",
	"CC112X_PARTVERSION",
	"CC112X_SERIAL_STATUS",
	"CC112X_MODEM_STATUS1",
	"CC112X_MODEM_STATUS0",
	"CC112X_MARC_STATUS1",
	"CC112X_MARC_STATUS0",
	"CC112X_PA_IFAMP_TEST",
	"CC112X_FSRF_TEST",
	"CC112X_PRE_TEST",
	"CC112X_PRE_OVR",
	"CC112X_ADC_TEST",
	"CC112X_DVC_TEST",
	"CC112X_ATEST",
	"CC112X_ATEST_LVDS",
	"CC112X_ATEST_MODE",
	"CC112X_XOSC_TEST1",
	"CC112X_XOSC_TEST0",
	"CC112X_RXFIRST",
	"CC112X_TXFIRST",
	"CC112X_RXLAST",
	"CC112X_TXLAST",
	"CC112X_NUM_TXBYTES",
	"CC112X_NUM_RXBYTES",
	"CC112X_FIFO_NUM_TXBYTES",
	"CC112X_FIFO_NUM_RXBYTES",
	"CC112X_SINGLE_TXFIFO",
	"CC112X_BURST_TXFIFO",
	"CC112X_SINGLE_RXFIFO",
	"CC112X_BURST_RXFIFO",
	"CC112X_LQI_CRC_OK_BM",
	"CC112X_LQI_EST_BM",
	"CC112X_SRES",
	"CC112X_SFSTXON",
	"CC112X_SXOFF",
	"CC112X_SCAL",
	"CC112X_SRX",
	"CC112X_STX",
	"CC112X_SIDLE",
	"CC112X_SWOR",
	"CC112X_SPWD",
	"CC112X_SFRX",
	"CC112X_SFTX",
	"CC112X_SWORRST",
	"CC112X_SNOP",
	"CC112X_AFC",
	"CC112X_STATE_IDLE",
	"CC112X_STATE_RX",
	"CC112X_STATE_TX",
	"CC112X_STATE_FSTXON",
	"CC112X_STATE_CALIBRATE",
	"CC112X_STATE_SETTLING",
	"CC112X_STATE_RXFIFO_ERROR",
	"CC112X_STATE_TXFIFO_ERROR",
};
const uint16_t register_addresses[] = {
	0x0001,
	0x0002,
	0x0003,
	0x0004,
	0x0005,
	0x0006,
	0x0007,
	0x0008,
	0x0009,
	0x000A,
	0x000B,
	0x000C,
	0x000D,
	0x000E,
	0x000F,
	0x0010,
	0x0011,
	0x0012,
	0x0013,
	0x0014,
	0x0015,
	0x0016,
	0x0017,
	0x0018,
	0x0019,
	0x001A,
	0x001B,
	0x001C,
	0x001D,
	0x001E,
	0x001F,
	0x0020,
	0x0021,
	0x0022,
	0x0023,
	0x0024,
	0x0025,
	0x0026,
	0x0027,
	0x0028,
	0x0029,
	0x002A,
	0x002B,
	0x002C,
	0x002D,
	0x002E,
	0x2F00,
	0x2F01,
	0x2F02,
	0x2F03,
	0x2F04,
	0x2F05,
	0x2F06,
	0x2F07,
	0x2F08,
	0x2F09,
	0x2F0A,
	0x2F0B,
	0x2F0C,
	0x2F0D,
	0x2F0E,
	0x2F0F,
	0x2F10,
	0x2F11,
	0x2F12,
	0x2F13,
	0x2F14,
	0x2F15,
	0x2F16,
	0x2F17,
	0x2F18,
	0x2F19,
	0x2F1A,
	0x2F1B,
	0x2F1C,
	0x2F1D,
	0x2F1E,
	0x2F1F,
	0x2F20,
	0x2F21,
	0x2F22,
	0x2F23,
	0x2F24,
	0x2F25,
	0x2F26,
	0x2F27,
	0x2F28,
	0x2F29,
	0x2F2A,
	0x2F2B,
	0x2F2C,
	0x2F2D,
	0x2F2E,
	0x2F2F,
	0x2F30,
	0x2F31,
	0x2F32,
	0x2F33,
	0x2F34,
	0x2F35,
	0x2F36,
	0x2F37,
	0x2F38,
	0x2F39,
	0x2F3F,
	0x2F40,
	0x2F64,
	0x2F65,
	0x2F66,
	0x2F67,
	0x2F68,
	0x2F69,
	0x2F6A,
	0x2F6B,
	0x2F6C,
	0x2F6D,
	0x2F6E,
	0x2F6F,
	0x2F70,
	0x2F71,
	0x2F72,
	0x2F73,
	0x2F74,
	0x2F75,
	0x2F76,
	0x2F77,
	0x2F78,
	0x2F79,
	0x2F7A,
	0x2F7B,
	0x2F7C,
	0x2F7D,
	0x2F7E,
	0x2F7F,
	0x2F80,
	0x2F81,
	0x2F82,
	0x2F83,
	0x2F84,
	0x2F85,
	0x2F86,
	0x2F87,
	0x2F88,
	0x2F89,
	0x2F8A,
	0x2F8B,
	0x2F8C,
	0x2F8D,
	0x2F8E,
	0x2F8F,
	0x2F90,
	0x2F91,
	0x2F92,
	0x2F93,
	0x2F94,
	0x2F95,
	0x2F96,
	0x2F97,
	0x2F98,
	0x2F99,
	0x2F9A,
	0x2F9B,
	0x2F9C,
	0x2F9D,
	0x2F9E,
	0x2F9F,
	0x2FA0,
	0x2FD2,
	0x2FD3,
	0x2FD4,
	0x2FD5,
	0x2FD6,
	0x2FD7,
	0x2FD8,
	0x2FD9,
	0x003F,
	0x007F,
	0x00BF,
	0x00FF,
	0x80,
	0x7F,
	0x30,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x38,
	0x39,
	0x3A,
	0x3B,
	0x3C,
	0x3D,
	0x37,
	0x00,
	0x10,
	0x20,
	0x30,
	0x40,
	0x50,
	0x60,
	0x70,
};

char output[256];

#define SPI_DEVICE SPI_RADIO_RX

int main(){
	debug_init();
	debug_print("=== TX config reader ===");
	cc112x_reset(SPI_DEVICE);
	// check CC112x exists
	if (!(cc112x_query_partnumber(SPI_DEVICE))){
		debug_print("Device not found on SPI");
	}

	// reading configuration registers.
	uint8_t reg_result16[2] = {0, 0};
	uint8_t reg_result8 = 0;
	rfStatus_t retval = 0;
	for (int i = 0; i<(sizeof(register_addresses)/sizeof(uint16_t)); i++){
		if ((register_addresses[i] & 0xFF00) == 0x0000){
			// for the 8-bit register space
			retval = cc112xSpiReadReg(SPI_DEVICE, register_addresses[i], &reg_result8);
			sprintf(output, "% 30s 0x%04X:\t%u\t%u", register_names[i], register_addresses[i], retval, reg_result8);
			debug_print(output);
		}
		else{
			// for the 16 bit register? Honesty, I've got no clue how this works
			retval = cc112xSpiReadReg(SPI_DEVICE, register_addresses[i], reg_result16);
			sprintf(output, "% 30s 0x%04X:\t%u\t%u %u", register_names[i], register_addresses[i], retval, reg_result16[0], reg_result16[1]);
			debug_print(output);
		}

	}

	debug_print("=== config read finished ===");
	debug_end();
}