// Copy from https://github.com/Seeed-Studio/Grove_Sunlight_Sensor/blob/master/Si115X.cpp (2502fac4cdc98c6e1468d5595bd482471a5aa3ce)

#include "Si115X.h"

// Si115X::Si115X() {
// 	//empty constructor
// }

/**
 * Configures a channel at a given index
 */

void Si115X::config_channel(uint8_t index, const uint8_t *conf){
    int len = sizeof(conf);
  
    if(len != 4 || index > 5)
      return;

    int inc = index * len;
    
    // ADCCONFIGx: 
    // - bits[7] - Reserved
    // - bits[6:5] - A/D conversion rate
    // - bits[4:0] - ADC MUX select (see below)
    //      00000 - Small IR   00001 - Medium IR   00010 - Large IR
    //      01011 - Visible    01101 - Large Visible
    param_set(ADCCONFIG_0 + inc, conf[0]);

    // ADCSENSx:
    // - bits[7] - ADC high signal range enable
    // - bits[6:4] - an internal accumulation of samples
    // - bits[3:0] - Measurement time for 512 decimation rate
    param_set(ADCSENS_0 + inc, conf[1]);

    // ADCPOSTx:
    // - bits[7] - Reserved
    // - bits[6] - HOSTOUTx size(0 = 16 bits, 1 = 24 bits)
    // - bits[5:3] - Number of bits to shift right of the output
    // - bits[2] - Threshold polarity
    // - bits[1:0] - Threshold enable
    param_set(ADCPOST_0 + inc, conf[2]);

    // MEASCONFIGx:
    // - bits[7:6] - MEASCOUNTx select
    // - bits[5:4] - Reserved
    // - bits[3] - LEDx_A or LEDx_B BANK select
    // - bits[2:0] - LEDx enable
    param_set(MEASCONFIG_0 + inc, conf[3]);   
}

/**
 * Writes data over i2c
 */
void Si115X::write_data(uint8_t addr, const uint8_t *data, size_t len){
    suli_i2c_write(i2c, addr, data, len);
}

/**
 * Reads data from a register over i2c
 */
int Si115X::read_register(uint8_t addr, uint8_t reg, int bytesOfData){
    uint8_t data[bytesOfData];
    if (suli_i2c_read_reg(i2c, addr, reg, data, bytesOfData) < 1) return -1;
    return data[0];
}

/**
 * param set as shown in the datasheet
 */
void Si115X::param_set(uint8_t loc, uint8_t val){
    const auto preResponse0 = read_register(DEVICE_ADDRESS, RESPONSE_0, 1);

    uint8_t packet[2];
    packet[0] = HOSTIN_0;
    packet[1] = val;
    write_data(DEVICE_ADDRESS, packet, sizeof(packet));
    packet[0] = COMMAND;
    packet[1] = loc | (0b10 << 6);
    write_data(DEVICE_ADDRESS, packet, sizeof(packet));

    while ((read_register(DEVICE_ADDRESS, RESPONSE_0, 1) & 0x0f) != ((preResponse0 + 1) & 0x0f)) {
        yield();
    }
}

/**
 * param query as shown in the datasheet
 */
int Si115X::param_query(uint8_t loc){
    const auto preResponse0 = read_register(DEVICE_ADDRESS, RESPONSE_0, 1);

    const uint8_t packet[2] = {COMMAND, (uint8_t)(loc | (0b01 << 6))};
    write_data(DEVICE_ADDRESS, packet, sizeof(packet));

    while ((read_register(DEVICE_ADDRESS, RESPONSE_0, 1) & 0x0f) != ((preResponse0 + 1) & 0x0f)) {
        yield();
    }
	
    return read_register(DEVICE_ADDRESS, RESPONSE_1, 1);
}

/**
 * Sends command to the command register
 */
void Si115X::send_command(uint8_t code){
    const auto preResponse0 = read_register(DEVICE_ADDRESS, RESPONSE_0, 1);

    const uint8_t packet[2] = {COMMAND, code};
    write_data(DEVICE_ADDRESS, packet, sizeof(packet));

    while (true) {
        const auto response = read_register(DEVICE_ADDRESS, RESPONSE_0, 1);
        if (response & 0x10) {  // CMD_ERR
            const uint8_t packet[2] = {COMMAND, RESET_CMD_CTR};
            write_data(DEVICE_ADDRESS, packet, sizeof(packet));
            break;
        }
        else if ((response & 0x0f) == ((preResponse0 + 1) & 0x0f)) {
            break;
        }
        yield();
    }
}




bool Si115X::Begin(I2C_T *i2c_device, bool mode){
    i2c = i2c_device;
    is_autonomous = mode;
    // Wire.setClock(400000);
    if (read_register(DEVICE_ADDRESS, 0x00) != 0x51) {
        return false;
    }

    // Reset
    const uint8_t packet[2] = {COMMAND, RESET_SW};
    write_data(DEVICE_ADDRESS, packet, sizeof(packet));

    // Wait for the reset to complete
    while (read_register(DEVICE_ADDRESS, RESPONSE_0, 1) != 0x2f) {
        yield();
    }

    // Enable 2 channels for proximity measurement
    param_set(CHAN_LIST, 0B000011);
    // Enable Interrupt
    write_register(DEVICE_ADDRESS, IRQ_ENABLE, 0B000011);
    // Initialize LED current
    param_set(LED1_A, 0x3F);
    param_set(LED1_B, 0x3F);

    // Configure ADC and enable LED drive
    if (is_autonomous) {
        param_set(MEASRATE_H, 0);
        param_set(MEASRATE_L, 1);  // 1 for a base period of 800 us
        param_set(MEASCOUNT_0, 1); 
        param_set(MEASCOUNT_1, 1);
        param_set(THRESHOLD0_L, 0);
        param_set(THRESHOLD0_H, 0);
        uint8_t conf[4]; // ADCCONFIGx, ADCSENSx, ADCPOSTx, MEASCONFIGx
        conf[0] = 0B01100000; // 1x Small IR
        conf[1] = 0B00000010; // 48.8us Nominal Measurement time for 512 decimation rate
        conf[2] = 0B00000001; // 16-bits output, Interrupt when the measurement is larger than THRESHOLD0
        conf[3] = 0B01000001; // enable LED1A, the time between measurements is 800*MEASRATE*MEASCOUNT0 us
        config_channel(0, conf);
        conf[0] = 0B01101101; // 2x Visible
        conf[1] = 0B00000010; // 48.8us Nominal Measurement time for 512 decimation rate
        conf[2] = 0B00000001; // 16-bits output, Interrupt when the measurement is larger than THRESHOLD0
        conf[3] = 0B10001001; // enable LED1B, the time between measurements is 800*MEASRATE*MEASCOUNT1 us
        config_channel(1, conf);

        send_command(START);
    }
    else {
        param_set(ADCCONFIG_0, 0B01100000);
        param_set(MEASCONFIG_0, 0x00);
        param_set(ADCPOST_0, 0x00);
        param_set(ADCCONFIG_1, 0B01101101);
        param_set(MEASCONFIG_1, 0x00);
        param_set(ADCPOST_1, 0x00);
    }

    return true;

}

uint16_t Si115X::ReadIR(void) {
    if (!is_autonomous) send_command(FORCE);
    uint8_t data[2];
    data[0] = read_register(DEVICE_ADDRESS, HOSTOUT_0);
    data[1] = read_register(DEVICE_ADDRESS, HOSTOUT_1);
    return (data[0] << 8) + data[1];
}

uint16_t Si115X::ReadVisible(void) {
    if (!is_autonomous) send_command(FORCE);
    uint8_t data[2];
    data[0] = read_register(DEVICE_ADDRESS, HOSTOUT_2);
    data[1] = read_register(DEVICE_ADDRESS, HOSTOUT_3);
    return (data[0] << 8) + data[1];
}
