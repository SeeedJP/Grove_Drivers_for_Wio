#ifndef SI115X_H
#define SI115X_H

#include "suli2.h"

class Si115X
{
public:
    static constexpr uint8_t DEVICE_ADDRESS = 0x53 << 1;

    enum class RegisterAddress : uint8_t
    {
        PART_ID = 0x00,
        REV_ID = 0x01,
        MFR_ID = 0x02,
        INFO0 = 0x03,
        INFO1 = 0x04,
        HOSTIN3 = 0x07,
        HOSTIN2 = 0x08,
        HOSTIN1 = 0x09,
        HOSTIN0 = 0x0A,
        COMMAND = 0x0B,
        IRQ_ENABLE = 0x0F,
        RESPONSE1 = 0x10,
        RESPONSE0 = 0x11,
        IRQ_STATUS = 0x12,
        HOSTOUT0 = 0x13,
        HOSTOUT1 = 0x14,
        HOSTOUT2 = 0x15,
        HOSTOUT3 = 0x16,
        HOSTOUT4 = 0x17,
        HOSTOUT5 = 0x18,
        HOSTOUT6 = 0x19,
        HOSTOUT7 = 0x1A,
        HOSTOUT8 = 0x1B,
        HOSTOUT9 = 0x1C,
        HOSTOUT10 = 0x1D,
        HOSTOUT11 = 0x1E,
        HOSTOUT12 = 0x1F,
        HOSTOUT13 = 0x20,
        HOSTOUT14 = 0x21,
        HOSTOUT15 = 0x22,
        HOSTOUT16 = 0x23,
        HOSTOUT17 = 0x24,
        HOSTOUT18 = 0x25,
        HOSTOUT19 = 0x26,
        HOSTOUT20 = 0x27,
        HOSTOUT21 = 0x28,
        HOSTOUT22 = 0x29,
        HOSTOUT23 = 0x2A,
        HOSTOUT24 = 0x2B,
        HOSTOUT25 = 0x2C,
    };

    enum class CommandCodes : uint8_t
    {
        RESET_CMD_CTR = 0x00,
        RESET_SW = 0x01,
        FORCE = 0x11,
        PAUSE = 0x12,
        START = 0x13,
        PARAM_QUERY = 0x40,
        PARAM_SET = 0x80,
    };

    enum class ParameterAddress : uint8_t
    {
        I2C_ADDR = 0x00,

        CHAN_LIST = 0x01,

        ADCCONFIG0 = 0x02,
        ADCSENS0 = 0x03,
        ADCPOST0 = 0x04,
        MEASCONFIG0 = 0x05,

        ADCCONFIG1 = 0x06,
        ADCPOST1 = 0x08,
        ADCSENS1 = 0x07,
        MEASCONFIG1 = 0x09,

        ADCCONFIG2 = 0x0A,
        ADCSENS2 = 0x0B,
        ADCPOST2 = 0x0C,
        MEASCONFIG2 = 0x0D,

        ADCCONFIG3 = 0x0E,
        ADCSENS3 = 0x0F,
        ADCPOST3 = 0x10,
        MEASCONFIG3 = 0x11,

        ADCCONFIG4 = 0x12,
        ADCSENS4 = 0x13,
        ADCPOST4 = 0x14,
        MEASCONFIG4 = 0x15,

        ADCCONFIG5 = 0x16,
        ADCSENS5 = 0x17,
        ADCPOST5 = 0x18,
        MEASCONFIG5 = 0x19,

        MEASRATE_H = 0x1A,
        MEASRATE_L = 0x1B,

        MEASCOUNT_0 = 0x1C,
        MEASCOUNT_1 = 0x1D,
        MEASCOUNT_2 = 0x1E,

        LED1_A = 0x1F,
        LED1_B = 0x20,
        LED3_A = 0x21,
        LED3_B = 0x22,
        LED2_A = 0x23,
        LED2_B = 0x24,

        THRESHOLD0_H = 0x25,
        THRESHOLD0_L = 0x26,
        THRESHOLD1_H = 0x27,
        THRESHOLD1_L = 0x28,

        UPPER_THRESHOLD_H = 0x29,
        UPPER_THRESHOLD_L = 0x2A,
        LOWER_THRESHOLD_H = 0x2C,
        LOWER_THRESHOLD_L = 0x2D,

        BURST = 0x2B,
    };

    struct ChannelConfig
    {
        uint8_t ADCCONFIG;
        uint8_t ADCSENS;
        uint8_t ADCPOST;
        uint8_t MEASCONFIG;
    };

public:
    uint8_t readRegister(RegisterAddress reg)
    {
        uint8_t value;

        if (suli_i2c_read_reg(i2c, DEVICE_ADDRESS, static_cast<uint8_t>(reg), &value, 1) < 1)
        {
            return 0;
        }

        return value;
    }

    void writeRegister(RegisterAddress reg, uint8_t value)
    {
        suli_i2c_write_reg(i2c, DEVICE_ADDRESS, static_cast<uint8_t>(reg), &value, 1);
    }

    uint8_t performCommand(CommandCodes code)
    {
        const auto preResponse0 = readRegister(RegisterAddress::RESPONSE0);

        writeRegister(RegisterAddress::COMMAND, static_cast<uint8_t>(code));

        while (true)
        {
            const auto response = readRegister(RegisterAddress::RESPONSE0);
            if (response & 0x10)
            {
                // CMD_ERR
                writeRegister(RegisterAddress::COMMAND, static_cast<uint8_t>(CommandCodes::RESET_CMD_CTR));
                return response;
            }
            else if ((response & 0x0f) == ((preResponse0 + 1) & 0x0f))
            {
                break;
            }
            yield();
        }

        return 0;
    }

    void setParameter(ParameterAddress address, uint8_t value)
    {
        const auto preResponse0 = readRegister(RegisterAddress::RESPONSE0);

        writeRegister(RegisterAddress::HOSTIN0, value);
        writeRegister(RegisterAddress::COMMAND, static_cast<uint8_t>(CommandCodes::PARAM_SET) | static_cast<uint8_t>(address));

        while ((readRegister(RegisterAddress::RESPONSE0) & 0x0f) != ((preResponse0 + 1) & 0x0f))
        {
            yield();
        }
    }

    uint8_t queryParameter(ParameterAddress address)
    {
        const auto preResponse0 = readRegister(RegisterAddress::RESPONSE0);

        writeRegister(RegisterAddress::COMMAND, static_cast<uint8_t>(CommandCodes::PARAM_QUERY) | static_cast<uint8_t>(address));

        while ((readRegister(RegisterAddress::RESPONSE0) & 0x0f) != ((preResponse0 + 1) & 0x0f))
        {
            yield();
        }

        return readRegister(RegisterAddress::RESPONSE1);
    }

    void setChannelConfig(uint8_t channel, const ChannelConfig &config)
    {
        auto address = [channel](ParameterAddress baseAddress)
        { return static_cast<ParameterAddress>(static_cast<uint8_t>(baseAddress) + channel * 4); };

        setParameter(address(ParameterAddress::ADCCONFIG0), config.ADCCONFIG);
        setParameter(address(ParameterAddress::ADCSENS0), config.ADCSENS);
        setParameter(address(ParameterAddress::ADCPOST0), config.ADCPOST);
        setParameter(address(ParameterAddress::MEASCONFIG0), config.MEASCONFIG);
    }

    bool Begin(I2C_T *i2c_device)
    {
        i2c = i2c_device;

        if (readRegister(RegisterAddress::PART_ID) != 0x51)
        {
            return false;
        }

        // Reset
        writeRegister(RegisterAddress::COMMAND, static_cast<uint8_t>(CommandCodes::RESET_SW));

        // Wait for the reset to complete
        while (readRegister(RegisterAddress::RESPONSE0) != 0x2f)
        {
            yield();
        }

        setChannelConfig(0, {0b00000000, 0b10000000, 0b01000000, 0b00000000}); // 1x Small IR
        setChannelConfig(1, {0b00001011, 0b10000000, 0b01000000, 0b00000000}); // 1x Visible

        return true;
    }

    int32_t ReadIR(void)
    {
        setParameter(ParameterAddress::CHAN_LIST, 0b000001);

        performCommand(CommandCodes::FORCE);

        int32_t value;
        reinterpret_cast<uint8_t *>(&value)[3] = readRegister(RegisterAddress::HOSTOUT0);
        reinterpret_cast<uint8_t *>(&value)[2] = readRegister(RegisterAddress::HOSTOUT1);
        reinterpret_cast<uint8_t *>(&value)[1] = readRegister(RegisterAddress::HOSTOUT2);
        reinterpret_cast<uint8_t *>(&value)[0] = 0;

        return value / 256;
    }

    int32_t ReadVisible(void)
    {
        setParameter(ParameterAddress::CHAN_LIST, 0b000010);

        performCommand(CommandCodes::FORCE);

        int32_t value;
        reinterpret_cast<uint8_t *>(&value)[3] = readRegister(RegisterAddress::HOSTOUT0);
        reinterpret_cast<uint8_t *>(&value)[2] = readRegister(RegisterAddress::HOSTOUT1);
        reinterpret_cast<uint8_t *>(&value)[1] = readRegister(RegisterAddress::HOSTOUT2);
        reinterpret_cast<uint8_t *>(&value)[0] = 0;

        return value / 256;
    }

private:
    I2C_T *i2c;
};

#endif
