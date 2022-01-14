/*
 * ********************************************************************
 *                              Reconectado
 * ********************************************************************
*/

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clienteID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic_sub, "Enviando el primer mensaje desde Cash-32"); // esto no se envia por que el backend se muere :( 
      client.subscribe(topic_sub);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");  
      delay(5000);
    }
  }
}
