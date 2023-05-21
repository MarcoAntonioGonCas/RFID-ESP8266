//-----------------------------------------------
// Este objeto es en donde indicaremos en que pines esta conectado nuestro
// Rfid el igual
//-----------------------------------------------
MFRC522 rfid(pinCSfid, pinRSTfid);


//---------------------------------------------------------------
//Indica si se encuentra disponible una tarjeta cerca del sensor
//---------------------------------------------------------------
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


//---------------------------------------------------------------
//Lee el UUID de la tarjeta que se encuentra en el sensor
//---------------------------------------------------------------
String leerTarjeta(MFRC522& rfid)
{
  String strId = "";
  for (int i = 0; i < rfid.uid.size; i++)
  {
    if(rfid.uid.uidByte[i] < 0x10){
      strId+="0";
    }
    strId += String(rfid.uid.uidByte[i], HEX);

    if(i + 1 < rfid.uid.size ){
      strId += "-";
      
    }
  }
  rfid.PICC_HaltA();
  strId.toUpperCase();
  return strId;
}


//----------------------------------------------------------------
//Loop que se ejecutara en la funcion principal
//---------------------------------------------------------------
void loopRfid(){
  
  if (tarjetaDisponible(rfid))
  {
    ledRFID.prender(100, 50, 5);
    ledAcceso.prender(400);
    String strId = leerTarjeta(rfid);
    Serial.println("Tarjeta detectada");
    Serial.println(strId);
    enviarPostApi(strId);
  }
}