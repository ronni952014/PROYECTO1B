#include"datos_entrenamiento.h"
#include"datos_test.h"
#include"bddfinal.h"

#include <EEPROM.h>
//PARAMETROS PARA KNN
int fila = 3;
int col = 5;
int k = 1;
int label = 3;
float datos[4] = {0, 0, 0, 0};


//vectores promedio para cada tipo
float promedio[4] = {0, 0, 0, 0};
float promedio2[4] = {0, 0, 0, 0};
float promedio3[4] = {0, 0, 0, 0};

int on = 0;
float p = 0;
int filaoo = 0;
//-------limitacion de filas por tipo
//TIPO 1
int filao = 0;
int filaox = 0;
int filaf = 40;
//TIPO 2
int filao2 = 40;
int filaox2 = 40;
int filaf2 = 80;
//TIPO 3
int filao3 = 80;
int filaox3 = 80;
int filaf3 = 120;


float radio = 0.85;
float radiof = 0.935;
float radio2 = 0.2;
float radio3 = 0.2;

int colu = 0;
int aux = 0;

float distancia = 0;
float distotal[40];
float distotal2[40];
float distotal3[40];

float dtnormal[40];
float dtnormal2[40];
float dtnormal3[40];
float dmaxima = 0;
float dmaxima2 = 0;
float dmaxima3 = 0;
float matrizreducida[40][5];
//---------------------------------------
//---------variables pra el promedio

int u = 0;
float result;
float porcentaje;
//NUEVO DATO
float nuevodato[4] = {5, 3, 1, 0.2};
float newdato[5];
int con_nd = 0;

//------------------------------
//-------variables EEPROM
int ncol = 5;
float nbddfinal;
int filaee, colee, ee = 0;
float n = 0;
void setup() {
  Serial.begin(9600);
  //------------------------------------------------
  filaoo = filao;
  //>>>>>>>>PARA SACAR EL PROMEDIO DE LAS COLUMNAS
  //Serial.println("PROMEDIOS");
  // TIPO 1
  for (; colu < 4; colu++) {
    for (; filao < filaf; filao++) {
      p = p + entrenamiento[filao][colu];
    }
    promedio[colu] = p / (filao - filaoo);
    //Serial.print(promedio[colu]);
    //Serial.print(' ');
    filao = filaoo;
    p = 0;
  }

  //TIPO 2
  colu = 0;
  filaoo = filao2;
  for (; colu < 4; colu++) {
    for (; filao2 < filaf2; filao2++) {
      p = p + entrenamiento[filao2][colu];
    }
    promedio2[colu] = p / (filao2 - filaoo);
    // Serial.print(promedio2[colu]);
    //Serial.print(' ');
    filao2 = filaoo;
    p = 0;
  }
  //Serial.println(' ');
  //TIPO 3
  colu = 0;
  filaoo = filao3;
  for (; colu < 4; colu++) {
    for (; filao3 < filaf3; filao3++) {
      p = p + entrenamiento[filao3][colu];
    }
    promedio3[colu] = p / (filao3 - filaoo);
    //Serial.print(promedio3[colu]);
    // Serial.print(' ');
    filao3 = filaoo;
    p = 0;
  }
  //Serial.println("DISTANCIAS");

  //>>>>>>>>>>>>>>>SACANDO LAS DISTANCIAS
  //TIPO 1
  filaoo = 0;
  for (filao = 0; filao < filaf; filao++) {
    for (colu = 0; colu < 4; colu++) {
      distancia = distancia + pow(entrenamiento[filao][colu] - promedio[colu], 2);
    }
    distotal[filao - filaoo] = sqrt(distancia);
    //Serial.println(filao - filaoo);
    //Serial.println(distotal[filao - filaoo]);

    if (distotal[filao - filaoo] > dmaxima)
      dmaxima = distotal[filao - filaoo];
    colu = 0;
    distancia = 0;
  }
  //TIPO 2
  filaoo = 40;
  distancia = 0;
  for (filao2 = 40; filao2 < filaf2; filao2++) {
    for (colu = 0; colu < 4; colu++) {
      distancia = distancia + pow(entrenamiento[filao2][colu] - promedio2[colu], 2);
    }
    distotal2[filao2 - filaoo] = sqrt(distancia);
    //Serial.println(filao2 - filaoo);
    //Serial.println(distotal2[filao2 - filaoo]);

    if (distotal2[filao2 - filaoo] > dmaxima2)
      dmaxima2 = distotal2[filao2 - filaoo];
    colu = 0;
    distancia = 0;
  }
  //TIPO 3
  filaoo = 80;
  distancia = 0;
  for (filao3 = 80; filao3 < filaf3; filao3++) {
    for (colu = 0; colu < 4; colu++) {
      distancia = distancia + pow(entrenamiento[filao3][colu] - promedio3[colu], 2);
    }
    distotal3[filao3 - filaoo] = sqrt(distancia);
    //Serial.println(filao3 - filaoo);
    //Serial.println(distotal3[filao3 - filaoo]);

    if (distotal3[filao3 - filaoo] > dmaxima3)
      dmaxima3 = distotal3[filao3 - filaoo];
    colu = 0;
    distancia = 0;
  }
  //>>>>>>>>>>>> NORMALIZANDO EL VECTOR DISTANCIAS
  //TIPO 1
  filaoo = 0;
  for (filao = 0; filao < filaf; filao++) {
    dtnormal[filao - filaoo] = distotal[filao - filaoo] / dmaxima;
  }
  filao = filaoo;

  for (; filao < filaf; filao++) {
    if (dtnormal[filao - filaoo] >= radio && dtnormal[filao - filaoo] <= radiof ) {
      for (; colu < 5; colu++) {
        matrizreducida[aux][colu] = entrenamiento[filao][colu];
      }
      aux++;
    }
    colu = 0;
  }
  filaoo = 40;

  //TIPO 2

  for (filao2 = 40; filao2 < filaf2; filao2++) {
    dtnormal2[filao2 - filaoo] = distotal2[filao2 - filaoo] / dmaxima2;
  }


  for (filao2 = 40; filao2 < filaf2; filao2++) {
    if (dtnormal2[filao2 - filaoo] >= radio && dtnormal2[filao2 - filaoo] <= radiof) {
      for (; colu < 5; colu++) {
        matrizreducida[aux][colu] = entrenamiento[filao2][colu];
      }
      aux++;
    }
    colu = 0;
  }
  filaoo = 80;

  //TIPO 3

  for (filao3 = 80; filao3 < filaf3; filao3++) {
    dtnormal3[filao3 - filaoo] = distotal3[filao3 - filaoo] / dmaxima3;
  }


  for (filao3 = 80; filao3 < filaf3; filao3++) {
    if (dtnormal3[filao3 - filaoo] >= radio && dtnormal3[filao3 - filaoo] <= radiof) {
      for (; colu < 5; colu++) {
        matrizreducida[aux][colu] = entrenamiento[filao3][colu];
      }
      aux++;
    }
    colu = 0;
  }

  Serial.print("Tamaño de la matriz reducida es de ");
  Serial.print(aux);
  Serial.println(" x 5");
  float matriz[aux][5]; //MATRIZ REDUCIDA

  for (int i = 0; i < aux; i++)
    for (int j = 0; j < 5; j++)
      matriz[i][j] = matrizreducida[i][j];


  //PARA IMPRIMIR
  for (int i = 0; i < aux; i++) {
    for (; colu < 5; colu++) {
      Serial.print(matriz[i][colu]);
      Serial.print(" ");
    }
    Serial.println(" ");
    colu = 0;
  }
  Serial.println("----------KNN------------");
  //----------------------------------------------------------------------------------
  //------------------------------APLICAMOS KNN---------------------------------------
  //----------------------------------------------------------------------------------
  for (int ii = 0 ; ii < 30; ii++) {
    for (int jj = 0; jj < 4; jj++) {
      datos[jj] = prueba[ii][jj];
    }

    int c = 0; // movernos en columnas
    int f = 0; //movernos en fila

    float k_vecinos_dist[k];//vector de almacenamiento de k distancias menores
    float etiquetas[2][label];// matriz y conteo de etiquetas
    float dist = 0; //variable que almacena cada distancia.
    float dist_total = 0; // caribel para almacenamiento distancai

    //lenar vector k_vecinos_dist con valores altos
    float eti_menor[k];//vector de eqtiquetas de distancia menor
    int k_cont = 0; //contador de k
    int i = 0; //contador
    float clase; //return etiqueta
    float comparacion;// comparar k-vecino mayor



    //Serial.println(' ');
    for (; c < k; c++)
    {
      k_vecinos_dist[c] = 2000.0 + c; // valores altos y ordenados
    }
    c = 0; //reiniciar variable
    //llenado de las k distancias menores
    for (; c < label; c++) {
      etiquetas [0][c] = c + 1; //lleno con valores de etiqueta
      etiquetas [1][c] = 0; //lleno con el conteo de etiquetas
    }
    c = 0; // reinicio de variabel
    // distancia mas corta del nuevo punto hacia la amtriz
    for (f = 0; f < fila; f++)
    {
      for (; c < col - 1; c++) {
        dist = dist + pow(matriz[f][c] - datos[c], 2); //distancia entre dos puntos
      }
      dist_total = sqrt(dist); //raíz total de la formula
      //Serial.println(dist_total);
      for (; k_cont < k; k_cont++) { // determinar las k distancias enores y ordenarlas
        if (dist_total < k_vecinos_dist[k_cont]) {
          k_vecinos_dist[k_cont] = dist_total; // asignar nuevo valor a vetor de dsitancias
          eti_menor[k_cont] = matriz[f][col - 1]; //col=5,4 //ya se ordenos el vector
        }
      }
      k_cont = 0;
      dist = 0;
      dist_total = 0;
      c = 0;
    }
    //for fila

    // comparar cada una de las posiciones d ela matriz y comparar

    //seleccion del k vecino
    for (; c < label; c++) {
      for (; k_cont < k; k_cont++) { // recorro cad posicion de eti_menor y comparo con etiquetas y cuento si son iguales
        if (etiquetas[0][c] == eti_menor[k_cont]) {
          i++;
          etiquetas[1][c] = i;
          //Serial.println(i);
        }
      }
      k_cont = 0;
      i = 0;
    }
    //c=0;
    c = 1; //para que en comparacion
    comparacion = etiquetas[1][0];
    clase = etiquetas[0][0];

    ;
    for (; c < label; c++) {
      if (etiquetas[1][c] > comparacion) { //omparacion entre valores de suma de etiqueta
        clase = etiquetas[0][c]; //ponga neuva clase

        comparacion = etiquetas[1][c];

      }
    }
    //---------IMPRIME LAS ETIQUETAS Y LAS COMPARA PRA EL PORCENTAJE
    //    Serial.print(ii);
    //    Serial.print('-');
    //    Serial.println(clase);
    comparacion = 0;
    c = 1;
    if (clase == prueba[ii][4]) {
      u++;

    }
    porcentaje = u * 100 / 30;

  }
  Serial.print("el % de aciertos es: ");
  Serial.println(porcentaje);

  Serial.println("----------EEPROM------------");


  for (; filaee < aux; filaee++) {
    for (; colee < ncol; colee++) {
      ee++;
      nbddfinal = matriz[filaee][colee] ;
      EEPROM.put(ee, matriz[filaee][colee]);

      Serial.print(EEPROM.get(ee, nbddfinal));

      Serial.print(';');

    }
    colee = 0;
  }
  Serial.println(" ");

  Serial.println("----DATOS GUARDADOS EEPROM");
  //--------------------------------------------
  //---------ingrsando nuevo dato---------------
  //--------------------------------------------
  Serial.println("-----new dato------");
  //TIPO1
  for (colu = 0; colu < 4; colu++) {
    distancia = distancia + pow(nuevodato[colu] - promedio[colu], 2);
  }
  if (distancia / dmaxima < 1) {
    Serial.println("correcto para label 1");
    for (int m = 0; m < 4; m++)
      newdato[m] = nuevodato[m];
    newdato[4] = 1;
    con_nd++;
  }
  distancia = 0;
  //TIPO2
  for (colu = 0; colu < 4; colu++) {
    distancia = distancia + pow(nuevodato[colu] - promedio2[colu], 2);
  }
  if (distancia / dmaxima2 < 1) {
    Serial.println("correcto para label 2");
    for (int m = 0; m < 4; m++)
      newdato[m] = nuevodato[m];
    newdato[4] = 2;
    con_nd++;
  }
  distancia = 0;
  //TIPO3
  for (colu = 0; colu < 4; colu++) {
    distancia = distancia + pow(nuevodato[colu] - promedio3[colu], 2);
  }
  if (distancia / dmaxima3 < 1) {
    Serial.println("correcto para label 3");
    for (int m = 0; m < 4; m++)
      newdato[m] = nuevodato[m];
    newdato[4] = 3;
    con_nd++;
  }
  distancia = 0;

  //IMPRIME VECTOR A GUARDAR
  for (int m = 0; m < 5; m++) {
    Serial.print(newdato[m]);
    Serial.print(' ');
  }

  //---------------------------------
  //-----------fin nuevo dato--------
  //---------------------------------

}




void loop() {

}

