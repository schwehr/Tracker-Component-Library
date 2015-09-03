// Msg 23 - G - Channel Management

#include "ais.h"

Ais23::Ais23(const char *nmea_payload, const size_t pad)
    : AisMsg(nmea_payload, pad) {
  if (status != AIS_UNINITIALIZED)
    return;

  assert(message_id == 23);

  if (pad != 2 || strlen(nmea_payload) != 27) {
    status = AIS_ERR_BAD_BIT_COUNT;
    return;
  }

  bitset<168> bs;
  const AIS_STATUS r = aivdm_to_bits(bs, nmea_payload);
  if (r != AIS_OK) {
    status = r;
    return;
  }

  spare = ubits(bs, 38, 2);

  x1 = (float)(sbits(bs, 40, 18) / 600.0);//Crouse:Added typecasting
  y1 = (float)(sbits(bs, 58, 17) / 600.0);//Crouse:Added typecasting
  x2 = (float)(sbits(bs, 75, 18) / 600.0);//Crouse:Added typecasting
  y2 = (float)(sbits(bs, 93, 17) / 600.0);//Crouse:Added typecasting

  station_type = ubits(bs, 110, 4);
  type_and_cargo = ubits(bs, 114, 8);
  spare2 = ubits(bs, 122, 22);

  txrx_mode = ubits(bs, 144, 2);
  interval_raw = ubits(bs, 146, 4);
  quiet = ubits(bs, 150, 4);
  spare3 = ubits(bs, 154, 6);

  status = AIS_OK;
}
