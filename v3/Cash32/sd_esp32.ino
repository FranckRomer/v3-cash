void sd_saveFileCOLABORADORES(){
    appendFile(SD_MMC, FILE_COLABORADORES, (Id_Colaborador + "\n"));
    appendFile(SD_MMC, FILE_COLABORADORES, (Fecha_Hora_Colaborador + "\n" ));
    
}

void sd_saveFileUSUARIO(){
    appendFile(SD_MMC, FILE_USUARIOS, (Dinero_Acumulado_String + "\n"));
    appendFile(SD_MMC, FILE_USUARIOS, (Ingresos_usuarios_String + "\n"));
    appendFile(SD_MMC, FILE_USUARIOS, (ingresos_colaboradores_String + "\n"));
    appendFile(SD_MMC, FILE_USUARIOS, (Total_ingresos_String + "\n"));
}

void sd_deleteARCHIVOS(){
    deleteFile(SD_MMC, FILE_USUARIOS);
    deleteFile(SD_MMC, FILE_COLABORADORES);
}

void sd_readFileCOLABORADORES(fs::FS &fs, const char * path){   
    
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file COLABORADORES: ");
    while(file.available()){
        char caracter = file.read();
        //String caracter2 = caracter;
        if(caracter != '\n'){
          mensaje.concat(caracter);
        }else{
          //Serial.print("funciona");
          //Serial.println(mensaje);
          count++;
          switch (count){
            case 1:
                Id_Colaborador_sd = mensaje;
                Serial.println("Id_Colaborador: " + Id_Colaborador_sd);
            break;
            case 2:
                Fecha_Hora_Colaborador_sd = mensaje;
                Serial.println("Fecha_Hora_Colaborador: " + Fecha_Hora_Colaborador_sd);
                count =0;
            break;
            }         
          mensaje = "";
        }
    }
  file.close();
}

void sd_readFileUSUARIOS(fs::FS &fs, const char * path){   
    
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file USUARIOS: ");
    while(file.available()){
        char caracter = file.read();
        //String caracter2 = caracter;
        if(caracter != '\n'){
          mensaje.concat(caracter);
        }else{
          //Serial.print("funciona");
          //Serial.println(mensaje);
          count++;
          switch (count){
            case 1:
                Dinero_Acumulado_String_sd = mensaje;
                Serial.println("Dinero_Acumalado: " + Dinero_Acumulado_String_sd);
            break;
            case 2:
                Ingresos_usuarios_sd = mensaje;
                Serial.println("Total_usuarios: " + Ingresos_usuarios_sd);
            break;
            case 3:
                ingresos_colaboradores_sd = mensaje;
                Serial.println("Total_colaboradores: " + ingresos_colaboradores_sd);
            break;
            case 4:
                Total_ingresos_sd = mensaje;
                Serial.println("Total_Ingresos: " + Total_ingresos_sd);
                count =0;
            break;

            
            }         
          mensaje = "";
        }
    }
  file.close();
}

void sd_saveFECHAHORA(){
  deleteFile(SD_MMC, FILE_FECHAHORA);
  appendFile(SD_MMC, FILE_FECHAHORA, (Fecha_Hora + "\n"));
}


////////////////////////FUNCIONES////////////////////////////
/*void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
*/
void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        char caracter = file.read();
        //String caracter2 = caracter;
        if(caracter != '\n'){
          mensaje.concat(caracter);
        }else{
          Serial.print("funciona");
          Serial.println(mensaje);
          mensaje = "";
        }
    }
}

void writeFile(fs::FS &fs, const char * path, String message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
}

void appendFile(fs::FS &fs, const char * path, String message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}
