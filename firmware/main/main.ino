#include <math.h>

const int turb_pin = A0; // Define o pino analógico a ser utilizado

//#define USE_EQUACAO
#define USE_INTERPOLACAO


void setup() {
  Serial.begin(9600); // Inicia a comunicação serial

}

void loop() {
  read_turb(); // Lê o valor do sensor de turb
  
  delay(1000); // Espera 1 segundo antes de realizar a próxima leitura
}


void read_turb() {
  int turb_raw = analogRead(turb_pin); // Lê a voltagem do sensor
  float ref_value = turb_raw * (5000.0 / 1023.0);
  

#ifdef USE_EQUACAO
  //usar o Excel para salvar a equação de leitura
  float turb_value = ( 0000000.5 * ref_value * ref_value ) - ( 0.0078 * ref_value) + 17.579
#endif


#ifdef USE_INTERPOLACAO
  // pontos de calibração: x = NTU, y = mV
  double x_cal[] = {0, 20, 200};
  double y_cal[] = {2010.00, 1506.00, 1042.00};

  int n_cal = sizeof(x_cal) / sizeof(x_cal[0]);

  // encontrar os índices das amostras mais próximas
  int i1 = 0;
  int i2 = n_cal-1;
  
  for (int i = 0; i < n_cal; i++) {
    if (y_cal[i] >= ref_value && (y_cal[i] < y_cal[i1]) && (i < n_cal-1)) {
      i1 = i;
    }
    if (y_cal[i] <= ref_value && (y_cal[i] > y_cal[i2]) && (i > 0)) {
      i2 = i;
    }
  }

  // interpolação linear entre as amostras mais próximas
  // m = (y2-y1)/(x2-x1)
  // n = y - m*x
  //y = m*x + n
  double turb_value = x_cal[i1] + (x_cal[i2] - x_cal[i1]) * (ref_value - y_cal[i1]) / (y_cal[i2] - y_cal[i1]);
  
#endif

  Serial.print("NTU = ");
  Serial.print(turb_value, 4); // Imprime o valor de turb com quatro casas decimais
  Serial.print(", voltage: ");
  Serial.print(ref_value, 2);
  Serial.println("mV");
}
