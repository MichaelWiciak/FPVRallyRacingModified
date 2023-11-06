#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h" 

const int IR_CMD_BASE=0;
const int IR_CMD_DOMINATION=1;

struct irPacketStruct { // order of fields gets reversed when sent, so bit sequence will be start,control,team,data,extra,checksum with msb of each sent first
  uint16_t checksum:4;
  uint16_t extra:3;
  uint16_t data:7;
  uint16_t team:1;
  uint16_t control:1;
} __attribute__ ((packed));

class Infrared
{
  public:
    Infrared();
    void init(gpio_num_t irPinIn, gpio_num_t irPinOut);
    void sendIR(uint8_t control, uint8_t team, uint8_t data, uint8_t extra);
    int receiveIR();
    uint8_t infraredReceived=0;
    uint8_t crcValid=0;
    struct irPacketStruct irPacketIn;
  private:
    int32_t processIR(rmt_item32_t *item, size_t size);
    rmt_item32_t rmtDataOut[17]; // data to send
    RingbufHandle_t buffer = NULL;
};  




