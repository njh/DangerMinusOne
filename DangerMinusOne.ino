/*
   Arduino sketch that uses a Danger Shield to publish MQTT-SN messages
   using QoS -1 based on various inputs:

   Topic  Description           Pin
   --------------------------------
   B1.....Button 1..............D10
   B2.....Button 2..............D11
   B3.....Button 3..............D12
   S1.....Slider 1..............A2
   S2.....Slider 2..............A1
   S3.....Slider 3..............A0
   L1.....Light Sensor..........A3
   T1.....Temperature Sensor....A4

   Hardware:
   https://www.sparkfun.com/products/11649

*/

#define PIN_LED  (5)
#define PIN_S1   (A2)
#define PIN_S2   (A1)
#define PIN_S3   (A0)


#define MQTT_SN_MAX_PACKET_LENGTH     (255)
#define MQTT_SN_TYPE_PUBLISH          (0x0C)
#define MQTT_SN_FLAG_QOS_N1           (0x3 << 5)
#define MQTT_SN_FLAG_RETAIN           (0x1 << 4)
#define MQTT_SN_TOPIC_TYPE_SHORT      (0x02)

void sendMessage(const char topic[2], String message, bool retain=false)
{
  byte header[7];

  header[0] = sizeof(header) + message.length();
  header[1] = MQTT_SN_TYPE_PUBLISH;
  header[2] = MQTT_SN_FLAG_QOS_N1 | MQTT_SN_TOPIC_TYPE_SHORT;
  if (retain) {
    header[2] |= MQTT_SN_FLAG_RETAIN;
  }
  header[3] = topic[0];
  header[4] = topic[1];
  header[5] = 0x00;  // Message ID High
  header[6] = 0x00;  // message ID Low
  Serial.write(header, sizeof(header));
  Serial.print(message);
}


void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_S1, INPUT);
  pinMode(PIN_S2, INPUT);
  pinMode(PIN_S3, INPUT);
}




int lastS1 = -1;
int lastS2 = -1;
int lastS3 = -1;

void loop() {
  digitalWrite(PIN_LED, HIGH);
  delay(100);
  digitalWrite(PIN_LED, LOW);

  int s1 = analogRead(PIN_S1);
  if (s1 != lastS1) {
    sendMessage("S1", String(s1, DEC));
    lastS1 = s1;
  }

  int s2 = analogRead(PIN_S2);
  if (s2 != lastS2) {
    sendMessage("S2", String(s2, DEC));
    lastS2 = s2;
  }

  int s3 = analogRead(PIN_S3);
  if (s3 != lastS3) {
    sendMessage("S3", String(s3, DEC));
    lastS3 = s3;
  }

  delay(100);
}
