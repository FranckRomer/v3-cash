/*
 *    AccionamientoRelevador
 */
void AccionamientoRelevador(){
  if(MontoAcumulado >= Tarifa){
      //Ingresos_Usuarios++;
      
      digitalWrite(rele_1, HIGH);
      MontoAcumulado = 0;
      delay(tiempo_accionamiento); 
      digitalWrite(rele_1, LOW);
  }else{
      digitalWrite(rele_1, LOW);
  }
}
