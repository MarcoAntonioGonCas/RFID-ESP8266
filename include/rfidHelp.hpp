#include <MFRC522.h>

bool tarjetaDisponible(MFRC522& rfid)
{
  if (rfid.PICC_IsNewCardPresent())
  {
    if (rfid.PICC_ReadCardSerial())
    {
      return true;
    }
  }
  return false;
}

String leerTarjeta(MFRC522& rfid)
{
  String strId = "";
  for (int i = 0; i < rfid.uid.size; i++)
  {
    
    // lec += ( rfid.uid.uidByte[i] < 0x10? "0" : " " );
    strId += String(rfid.uid.uidByte[i], HEX);
  }
  rfid.PICC_HaltA();
  return strId;
}
