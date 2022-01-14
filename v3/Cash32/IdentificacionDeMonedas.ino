/*
 *  IdentificacionDeMonedas
*/
void IdentificacionDeMonedas()
{
     switch (MensajeMonitorSerial2){
         case 193://C1
         MontoAcumulado = MontoAcumulado + Monedas[0];
         Dinero_Acumulado = Dinero_Acumulado + Monedas[0];
         break;
         case 194://C2
         MontoAcumulado = MontoAcumulado + Monedas[1];
         Dinero_Acumulado = Dinero_Acumulado + Monedas[1];
         break;
         case 197://C5
         MontoAcumulado = MontoAcumulado + Monedas[2];
         Dinero_Acumulado = Dinero_Acumulado + Monedas[2];
         break;
         case 202://C10
         MontoAcumulado = MontoAcumulado + Monedas[3];
         Dinero_Acumulado = Dinero_Acumulado + Monedas[3];
         break;
         case 203://C20
         MontoAcumulado = MontoAcumulado + Monedas[4];
         Dinero_Acumulado = Dinero_Acumulado + Monedas[4];
         break;
         case 204://C0.5
         MontoAcumulado = MontoAcumulado + Monedas[5];
         Dinero_Acumulado = Dinero_Acumulado + Monedas[5];
         break;
         case 205://C0.2
         MontoAcumulado = MontoAcumulado + Monedas[6];
         Dinero_Acumulado = Dinero_Acumulado + Monedas[6];
         break;
         case 206://C0.1
         MontoAcumulado = MontoAcumulado + Monedas[7];
         Dinero_Acumulado = Dinero_Acumulado + Monedas[7];
         break;
     }
     Dinero_Acumulado_String = Dinero_Acumulado;
     //Guarda en la SD el dinero total acumulado
     deleteFile(SD_MMC, FILE_USUARIOS);
     sd_saveFileUSUARIO();
     
     
}
