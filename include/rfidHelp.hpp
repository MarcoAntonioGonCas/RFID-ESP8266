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
    if(i !=0 ){
      strId += ( rfid.uid.uidByte[i] < 0x10 ? "0" : " " );
    }else if(i == 0 and rfid.uid.uidByte[i] < 0x10){
      strId += "0";
    }
    strId += String(rfid.uid.uidByte[i], HEX);
  }
  rfid.PICC_HaltA();
  return strId;
}
