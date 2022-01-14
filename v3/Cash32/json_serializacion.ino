/*
 * ************************************************************************************************
 *                                          JSON
 * ************************************************************************************************
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
//    Serializacion
void SerializeObject() {
    //int cont_empleados_total = targeta_empleado1_contador + targeta_empleado2_contador + targeta_empleado3_contador;
    //int cont_usuarios = Dinero_Acumulado / 5;
    
    char buffer[256];   
    StaticJsonDocument<300> doc;

    doc["id_monedero"] = id_MONEDERO;
    doc["Fecha_Hora_Actual"]   = Reloj;
    doc["monto"] = Dinero_Acumulado;
    //doc["cont_usuarios"] = cont_usuarios;
    //doc["cont_empleados_total"] = cont_empleados_total;
    doc["cont_empleados1"] = targeta_empleado1_contador;
    doc["cont_empleados2"] = targeta_empleado2_contador;
    doc["cont_empleados3"] = targeta_empleado3_contador;
   
    
    serializeJson(doc, buffer);
    
    size_t n = serializeJson(doc, buffer);
    delay(500);
    client.publish(topic_pub, buffer,n);

    Serial.println(mqtt_server);
    Serial.println(topic_pub);
    Serial.println(buffer);
    n = 0;
    
//    Dinero_Acumulado = 0;
//    targeta_empleado1_contador = 0;
//    targeta_empleado2_contador = 0;
//    targeta_empleado3_contador = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//    Deserializacion 
void DeserializeObject(String dato_json) {   
    String envio_json_can = dato_json;
    StaticJsonDocument<300> doc;
    
    DeserializationError error = deserializeJson(doc, envio_json_can);
    if (error) { return; }
    String costo_actual              = doc["costo_actual"];
    String targeta_corte_MQTT        = doc["targeta_corte"];
    String targeta_empleado1_MQTT    = doc["targeta_empleado1"];
    String targeta_empleado2_MQTT    = doc["targeta_empleado2"];
    String targeta_empleado3_MQTT    = doc["targeta_empleado3"];
    String targeta_empleado4_MQTT    = doc["targeta_empleado4"];
    String targeta_empleado5_MQTT    = doc["targeta_empleado5"];
    
    //envio_json_can = "FF1"+ can + pin + percentage + "00" + rgb;
    Serial.println("------------------json desarmado------------------");
    Serial.println("PENDIENTE RECIBIR MENSAJES JSON y si llego esto ,ignoralo");


        Dinero_Acumulado = 0;
    targeta_empleado1_contador = 0;
    targeta_empleado2_contador = 0;
    targeta_empleado3_contador = 0;
    
}
