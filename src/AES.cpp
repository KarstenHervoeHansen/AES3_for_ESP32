// 
// 
// 

#include "AES.h"


void i2sWriterTask(void* param)
{
    pinMode(FrameFlag, OUTPUT);
    size_t bytesWritten = 0;
    while (true)
    {
        i2s_event_t evt;
        if (xQueueReceive(AES.m_i2sQueue, &evt, portMAX_DELAY) == pdPASS)
        {
            if (evt.type == I2S_EVENT_TX_DONE)
            {
                AES.ConvertFrames();
                i2s_write(AES.m_i2sPort, (uint8_t*)AES.FramePtr, AES.FrameSize, &bytesWritten, portMAX_DELAY);
                digitalWrite(FrameFlag, HIGH);
            }
        }
    }
}


void SPDIFClass::SPDIFinit()
{
    i2s_config_t i2sConfig = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = SampleRate,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 192,
        .use_apll = true };

    m_i2sPort = i2sPort;
    //install and start i2s driver
    i2s_driver_install(m_i2sPort, &i2sConfig, 4, &m_i2sQueue);
    // set up the i2s pins
    i2s_set_pin(m_i2sPort, &I2SPins);
    // clear the DMA buffers
    i2s_zero_dma_buffer(m_i2sPort);
    // start a task to write samples to the i2s peripheral
    TaskHandle_t writerTaskHandle;
    xTaskCreate(i2sWriterTask, "i2s Writer Task", 1024, this, 1, &writerTaskHandle);
}


AESClass::AESClass()
{
    SPDIFinit();

    for (int i = 0; i < NUM_FRAMES_TO_SEND; i++) {
        Frame[i].block = 0;
    }

    for (int Val = 0; Val < 256; Val++) {
        uint16_t LastPhase = 0;
        uint16_t NewPhase;

        encodeArray[Val].paritet = false;
        encodeArray[Val].result = 0;
        for (uint8_t Bptr = 0; Bptr < 8; Bptr++) {

            if ((Val & 1 << Bptr) > 0) {
                encodeArray[Val].paritet = !encodeArray[Val].paritet;
                NewPhase = 0b1000000000000000;
            }
            else {
                NewPhase = 0b1100000000000000;
            }

            if ((LastPhase & 0b0100000000000000) > 0)  NewPhase ^= 0b1100000000000000;
            LastPhase = NewPhase;

            encodeArray[Val].result |= (NewPhase >> (2 * Bptr));
        }
    }
}

void AESClass::WriteFrame(uint8_t No, PreampleList Pre, uint32_t InData)
{
    Frame[No].subFrame[Pre].data[0] = InData;
    Frame[No].subFrame[Pre].data[1] = InData >> 8;
    Frame[No].subFrame[Pre].data[2] = InData >> 16;
    CurrentFrame = No;
 }

void AESClass::WriteFrame(uint32_t X, uint32_t Y)
{
    Serial.printf("\r\n%3d %06X %06X", CurrentFrame, X, Y);
    WriteFrame(CurrentFrame, x, X);
    WriteFrame(CurrentFrame, y, Y);
    CurrentFrame++;
    if (CurrentFrame >= NUM_FRAMES_TO_SEND) CurrentFrame = 0;
}

void AESClass::SetPhaseMask()
{
    if ((LastByte & 1) == 1) { PhaseMask = 0xFF; }
    else { PhaseMask = 0; }
}

void AESClass::ConvertFrames()
{
    uint8_t Preample[sizeof(PreampleList)] =  {0b11100010,0b11100100,0b11101000 }; // x,y,z

    // PhaseMask is set or cleared depending on the last bit transmitted. PhaseMask = 0xFF will invert the symbol
    for (int FrameNo = 0; FrameNo < NUM_FRAMES_TO_SEND; FrameNo++) {
        for (uint8_t SubNo = x; SubNo <= y; SubNo++) {

            byteEncode Enc;
            bool Pararity = false;

            if ((FrameNo == 0) && (SubNo == x)) {                              // z preample
                FramePtr[0].sub[0].pre = PhaseMask ^ Preample[z]; 
            }
            else {
                FramePtr[FrameNo].sub[SubNo].pre = PhaseMask ^ Preample[SubNo];// alternate x y preample
            }
 
            int Cnt = 5;
            for (uint8_t i = 0; i < 3; i++) {

                Enc = encodeArray[Frame[FrameNo].subFrame[SubNo].data[i]];
                Pararity ^= Enc.paritet;

                FramePtr[FrameNo].sub[SubNo].data[Cnt--] = PhaseMask ^ highByte(Enc.result);
                FramePtr[FrameNo].sub[SubNo].data[Cnt--] = LastByte = PhaseMask ^ lowByte(Enc.result);

                SetPhaseMask();
            }

            uint8_t Tmp;
            Enc = encodeArray[Tmp = Frame[FrameNo].subFrame[SubNo].status];
            Pararity ^= Enc.paritet;

            Tmp |= Pararity << 3;
            Enc = encodeArray[Tmp];

            FramePtr[FrameNo].sub[SubNo].status = LastByte = PhaseMask ^ highByte(Enc.result);

            SetPhaseMask();
        }
    }
}


AESClass AES;

