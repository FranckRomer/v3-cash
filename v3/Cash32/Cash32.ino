//   Cash32
// Este es un proyecto para los baños de CIA
// Se comunica por el puero Serial2 a un lector de monedas
// Controla un torniquete con los relevadores 
// Guarda datos en la memoria SD
// Realiza Cortes y con el mismo evento envia los datos por MQTT


//Librerias
#include <WiFi.h>
#include <PubSubClient.h>
#include <iostream>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <ESP32Time.h>

//sd librerias
#include "FS.h"
#include "SD_MMC.h"
#define FILE_COLABORADORES "/COLABORADORES.txt"
#define FILE_USUARIOS "/USUARIOS.txt"
#define FILE_FECHAHORA "/FECHAHORA.txt"
////////////////////////////////////////////////////////
// Configuracion Internet
const char* ssid        = "RED ACCESA";
const char* password    = "037E32E7";
String id_MONEDERO      = "Monedero_prueba";
////////////////////////////////////////////////////////
//    Configuracion MQTT
//const char* mqtt_server = "143.110.229.128";  // Servidor Remoto
const char* mqtt_server = "192.168.1.221";    //Server Local angy
//const char* mqtt_server = "192.168.1.215";    //Server Local 2

// topic_basic = Accesa/cia_baños/laboratorio/web   //  topic_sub
// topic_basic = Accesa/cia_baños/laboratorio/esp   //  topic_pub
String rootTopic      = "Accesa";
String project        = "cia_banos";
String client_accesa  = "laboratorio";
/////////////////////////////////////////////////////////////
//   Variables para el MQTT
String topicPubString = rootTopic + "/"+ project + "/"+ client_accesa +"/esp";
String topicSubString = rootTopic + "/"+ project + "/"+ client_accesa +"/web";
String clienteIDString = rootTopic + "_"+ project + client_accesa + "_admit" + random(1,100);

char path[] = "/";                    //no tiene otras direcciones 
const char* topic_sub = topicSubString.c_str();
const char* topic_pub = topicPubString.c_str();
const char* clienteID = clienteIDString.c_str();
/*
 * VARIABLES Mqtt
*/

WiFiClient espClient;
PubSubClient client(espClient);
/*
 *  Varialbles de MUX y targetas 
*/
String mux_conexion;
String targeta_corte      = "486854576510";  
String targeta_empleado1  = "484956656510"; 
String targeta_empleado2  = "485752496510";
String targeta_empleado3  = "484955486510";

int targeta_empleado1_contador  = 0; 
int targeta_empleado2_contador  = 0;
int targeta_empleado3_contador  = 0;
/*
 *   VARIABLES COLABORADOR
 */

/*
 *    VARIABLES RELOJ
 */
ESP32Time rtc;
const char*ntpServer ="pool.ntp.org";
const long gmtOffset_sec = -6*3600;
const int daylightOffsett_sec=0;
String Reloj;
/*
 * Variables de el Rele
 */
int rele_1 = 32;
int tiempo_accionamiento = 3000;
/*
 *  variables logicas
 */
int    MensajeMonitorSerial2;
String MensajeString;
double MontoAcumulado;

int Tarifa = 5;
double Monedas[]={1,2,5,10,20,0.5,0.2,0.1};

int Ingresos_usuarios = 0;  
int ingresos_colaboradores = 0;
double Dinero_Acumulado = 0;

/*
 *  Variables de la SD
 */
 // variables colaboradores
String Id_Colaborador = "";
String Fecha_Hora_Colaborador = "";
String Fecha_Hora="";

// variables de dinero
String Dinero_Acumulado_String = "";
String Ingresos_usuarios_String = "";
String ingresos_colaboradores_String = "";
String Total_ingresos_String = "";

// variables que recuperan lo almacenado en la SD
String Id_Colaborador_sd = "";
String Fecha_Hora_Colaborador_sd = "";

String Dinero_Acumulado_String_sd = "";
String Ingresos_usuarios_sd = "";
String ingresos_colaboradores_sd = "";
String Total_ingresos_sd = "";

// algoritmo SD
int count;
String mensaje = "";
 
/*
 * ********************************************************************
 *                                 Setup
 * ********************************************************************
*/
void setup() {
  // Se inicia los puertos seriales 0,1,2
  Serial.begin(9600);         // MONITOR SERIAL
  Serial1.begin(9600);        // MUX
  Serial2.begin(9600);        // MONEDERO
  // Inicializa el relevador como salida
  pinMode(rele_1, OUTPUT);
  // Manda a ver si hay WiFi
  setup_wifi();
  // verificacion de conexion a SD
  if(!SD_MMC.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD_MMC.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD_MMC card attached");
        return;
    }

    Serial.print("SD_MMC Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
}


/*
 * ********************************************************************
 *      Programa principal
 * ********************************************************************
*/
void loop() {
  //Verifica si esta habilitado el puerto serial 2
  if (Serial2.available())
  {       
    // leer todo lo que tenga el monitor serial 2
    MensajeMonitorSerial2 = Serial2.read();
    Serial.print("Mensaje: ");
    Serial.println(MensajeMonitorSerial2, HEX);

    //Identica la moneda y hace la logica // compara el valor acumulado con la tarifa
    IdentificacionDeMonedas();
    Serial.print("Acumulado: "); 
    Serial.println(MontoAcumulado);
    // Se activa el torniquete
    AccionamientoRelevador();
  } 
  
  
  
  /*
   * Lectura de MUx
   */
  if(Serial1.available() >0) {
    while (Serial1.available()) {
      char c = char(Serial1.read());
      mux_conexion = mux_conexion + String(Serial1.read()) ;
    }
    /*
     * Verificacion de tarjetas
     */
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Evento de CORTE
    if(mux_conexion == targeta_corte){
        Serial.println("En un momento le hacemos su corte");
        // Intenta conectar al WiFi
        WiFi.disconnect(true); 
        setup_wifi();

        // Si la conexion es exitosa manda los datos por MQTT
        if(WiFi.status() == WL_CONNECTED){
          Serial.println("WiFi connected\nIP address: "); 
          Serial.println(WiFi.localIP());
          client.setServer(mqtt_server, 1883);
          client.setCallback(callback);

          // Reconexion mqtt
          if (!client.connected()) {  
            reconnect_mqtt();  
          }
          // Actualiza el tiempo 

          Reloj = rtc.getTime("%d/%m/%Y %H:%M:%S");   
          
          // Envio de JSON
          // consulta los datos de la sd y los envia por MQTT
          SerializeObject(); 
          delay(1000);

          // se desconecta del wifi
          WiFi.disconnect(true);
          WiFi.mode(WIFI_OFF);  
        } // Acaba el evento CORTE
        //-------------------------------------------------------
        else{
          Serial.println("No conecto el WIFI, revice su conexion");
        }

        //acaba corte    
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Evento EMPLEADOS 
    else if(mux_conexion == targeta_empleado1 || mux_conexion == targeta_empleado2 || mux_conexion == targeta_empleado3){
        Serial.println("Pase al baño buen señor");

        // Toma el ID del trabajador
        Id_Colaborador = mux_conexion;
        Serial.println(Id_Colaborador);

        // Toma la hora y fecha actual
        Reloj = rtc.getTime("%d/%m/%Y %H:%M:%S");        
        Fecha_Hora_Colaborador = Reloj;
        Serial.println(Fecha_Hora_Colaborador);

        // Activa torniquete por 4 segundos // por ahora
        digitalWrite(rele_1, HIGH);
        delay(tiempo_accionamiento); 
        digitalWrite(rele_1, LOW);

        if(Id_Colaborador == targeta_empleado1){
          targeta_empleado1_contador++;
        }
        if(Id_Colaborador == targeta_empleado2){
          targeta_empleado2_contador++;
        }
        if(Id_Colaborador == targeta_empleado3){
          targeta_empleado3_contador++;
        }
        
        //Guarda en la SD el el nùmero de ingresos de usuarios/Colaboradores
        ingresos_colaboradores = targeta_empleado1_contador+ targeta_empleado2_contador+targeta_empleado3_contador;
        ingresos_colaboradores_String = ingresos_colaboradores;
        deleteFile(SD_MMC, FILE_USUARIOS);
        sd_saveFileUSUARIO();
        
    }// termina evento EMPLEADOS

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Evento TARJETA NO ACEPTADA    
    else{
        Serial.println("Esta tarjeta no fue aceptada, consulte a su provedor\nTarjeta id = " + (mux_conexion));
    } // Termina evento TARJETA NO ACEPTADA 
    mux_conexion = "";
  }// Fin de lectura de MUX
}// Fin LOOP
