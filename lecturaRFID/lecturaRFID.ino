#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

void setup(void) {
  Serial.begin(9600);
  Serial.println("Lector RFID");
  nfc.begin();
    Serial.println("¡Escanea tu tarjeta!\n");
}

void loop(void) {


  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: ");Serial.println(tag.getUidString());

    if (tag.hasNdefMessage()) 
    {

      NdefMessage message = tag.getNdefMessage();
      Serial.print("\nEsta tarjeta tiene un mensaje NDEF con ");
      Serial.print(message.getRecordCount());
      Serial.print(" memorias NDEF");
      if (message.getRecordCount() != 1) {
        Serial.print("s");
      }
      Serial.println(".");

      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        Serial.print("\nMemoria NDEF  ");Serial.println(i+1);
        NdefRecord record = message.getRecord(i);
        
        Serial.print("  TNF: ");Serial.println(record.getTnf());
        Serial.print("  Tipo: ");Serial.println(record.getType()); // will be "" for TNF_EMPTY


        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);

        Serial.print("  Informacion (HEX): ");
        PrintHexChar(payload, payloadLength);

        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        Serial.print("  Informacion (String): ");
        Serial.println(payloadAsString);

        String uid = record.getId();
        if (uid != "") {
          Serial.print("  ID: ");Serial.println(uid);
        }
      }
    }
  }
  delay(3000);
}
