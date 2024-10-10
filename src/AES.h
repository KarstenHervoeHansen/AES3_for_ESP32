// AES.h

#ifndef _AES_h
#define _AES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "driver/i2s.h"


#define TestFlag 13
#define FrameFlag 17

#define NUM_FRAMES_TO_SEND 192


class SPDIFClass
{
protected:
    int SampleRate = 96000; /*!< bitrate is two times the audio samplerate */


    i2s_port_t i2sPort = I2S_NUM_1;

    i2s_pin_config_t I2SPins = {
		.mck_io_num = GPIO_NUM_0, 		// 
        .bck_io_num = GPIO_NUM_27, 		// BCLK
        .ws_io_num = GPIO_NUM_14, 		// LR clock
        .data_out_num = GPIO_NUM_26, 	// AES out
        .data_in_num = -1
 };

    typedef struct {
        uint8_t status;
        uint8_t data[6];
        uint8_t pre;
    } subFrame;

public:
    typedef struct
    {
        union {
            subFrame sub[2];
            uint8_t byteArr[16];
        };
    } Frame_t;

    uint16_t FrameSize = sizeof(OutputFrames);

    Frame_t* FramePtr = OutputFrames;

    void SPDIFinit();
    uint32_t getSampleRate() { return SampleRate / 2; }

    friend void i2sWriterTask(void* param);

private:
    TaskHandle_t m_i2sWriterTaskHandle;
    QueueHandle_t m_i2sQueue;
    i2s_port_t m_i2sPort;
    Frame_t OutputFrames[NUM_FRAMES_TO_SEND];
	
};



class AESClass: public SPDIFClass
{
private:
    typedef struct {
        uint16_t result;
        bool paritet;
    } byteEncode;

    byteEncode encodeArray[256];

    typedef struct {
            uint8_t data[3];
            uint8_t status;
    } subFrameStruct;

    typedef struct {
        union {
            subFrameStruct subFrame[2];
            uint32_t block;
        };
    } FrameStruct;

    FrameStruct Frame[NUM_FRAMES_TO_SEND];

    uint8_t PhaseMask = 0;
    uint8_t LastByte = 0;

    void SetPhaseMask();


public:
    uint16_t CurrentFrame = 0;
    AESClass(); // class initialization

    enum PreampleList { x, y, z };
    void ConvertFrames();
    void WriteFrame(uint8_t No, PreampleList Pre, uint32_t InData);
    void WriteFrame(uint32_t X, uint32_t Y);
};




extern AESClass AES;

#endif

