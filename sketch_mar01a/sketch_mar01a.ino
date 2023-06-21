#include <LiquidCrystal.h>
#include <math.h>
#include <Keypad.h>

#define modo 2

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

LiquidCrystal lcd_1(rs, en, d4, d5, d6, d7);

const byte LINHAS = 4; // Linhas do teclado
const byte COLUNAS = 4; // Colunas do teclado

const char TECLAS_MATRIZ[LINHAS][COLUNAS] = { // Matriz de caracteres (mapeamento do teclado)
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

const byte PINOS_LINHAS[LINHAS] = {33, 35, 37, 39}; // Pinos de conexao com as linhas do teclado
const byte PINOS_COLUNAS[COLUNAS] = {41, 43, 45, 47}; // Pinos de conexao com as colunas do teclado

Keypad teclado_personalizado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS); // Inicia teclado

int X = 0, Y = 0;
int PALAVRA = 11;
int COUNT = 0;

int f1(void){
  return 10;
}

void setup() {
  // configura o número de colunas e linhas do lcd:
  lcd_1.begin(16, 2); 
  Serial.begin(9600);
}

int add(int bin[]) {
  int val1 = 0, val2 = 0;

  for(int i = 0; i < 4; i++){
    val1 += (bin[i+3]*pow(2, 3-i));
    val2 += (bin[i+7]*pow(2, 3-i));

    if(bin[i+7]*pow(2, 3-i) >= 4){
      val2+=1;
    }
    if(bin[i+3]*pow(2, 3-i) >= 4){
      val1+=1;
    }
  }

  X = val1+val2;

  Serial.print(X);
  Serial.print("\n");
  
  return 0;
}

int addI(int bin[]){
  int dec = 0;

  for(int i = 0; i < 8; i++){
    dec += (bin[i+3]*pow(2, 7-i));
    
    if(bin[i+3]*pow(2, 7-i) >= 4){
      dec += 1;
    }
  }

  if(dec > 38) dec -= 1;

  X += dec;

  if(X > 255){
    lcd_1.setCursor(4, 1);
    lcd_1.print("Overflow");
    X = 0;
  }

  Serial.print(X);
  Serial.print("\n");
  
  return 0;
}

int comp1(int bin[]){
  int dec = 0;


  for(int i = 0; i < 8; i++){
    dec += (bin[i+3]*pow(2, 8-i));
    
    if(bin[i+3]*pow(2, 7-i) >= 4){
      dec += 1;
    }
  }

  dec *= -1;

  X = 255 - dec;

  Serial.print(X);
  Serial.print("\n");
  
  return 0;
}

int armazena(int bin[]){
  int dec = 0;

  for(int i = 0; i < 8; i++){
    dec += (bin[i+3]*pow(2, 7-i));
  }

  X = dec;

  Serial.print(X);
  Serial.print("\n");
  
  return 0;
}

int def_print(int bin[]){
  int cont = 0;

  for(int i = 3; i<11; i++){
    if(bin[i] == 1) {
      cont++;
      break;
    }
  }

  Serial.print("definido \n");
  if(cont == 0){
    printX();
  }
  else{
    printY();
  }

  return 0;
}

int printX(){
  int bin[9];
  int x = X;

  for(int i = 0;i<9;i++){
    bin[i] = 0;
  }

  if(x<0) {
    x *= -1;
    bin[0] = 1;
  }

  for(int i = 1;i<9;i++){
    if(x >= pow(2, 8-i)){
      x -= pow(2, 8-i);
      bin[i] = 1;
    }
  }

  for(int i = 0; i<9;i++){
    lcd_1.setCursor(3+i, 0);
    lcd_1.print(bin[i]);
  }

  Serial.print(x);
  Serial.print("\n");
  
  return 0;
}

int printY(){
  int bin[9];
  int y = Y;

  for(int i = 0;i<9;i++){
    bin[i] = 0;
  }

  if(y<0) {
    y *= 0;
    bin[0] = 1;
  }

  for(int i = 1;i<9;i++){
    if(y >= pow(2, 8-i)){
      y -= pow(2, 8-i);
      bin[i] = 1;
    }
  }

  for(int i = 0; i<9;i++){
    lcd_1.setCursor(3+i, 0);
    lcd_1.print(bin[i]);
  }

  Serial.print(Y);
  Serial.print("\n");
  
  return 0;
}

int sub(int bin[]){
  int val1 = 0, val2 = 0;

  for(int i = 0; i < 4; i++){
    val1 += (bin[i+3]*pow(2, 3-i));
    val2 += (bin[i+7]*pow(2, 3-i));

    if(bin[i+7]*pow(2, 3-i) >= 4){
      val2+=1;
    }
    if(bin[i+3]*pow(2, 3-i) >= 4){
      val1+=1;
    }
  }

  X = val1-val2;

  Serial.print(X);
  Serial.print("\n");
  return 0;
}

int subI(int bin[]){
  int dec = 0;
  for(int i = 3; i <11 ; i++){
    dec += bin[i]*pow(2, 10-i);

    if(bin[i]*pow(2, 10-i) >= 8){
      dec += 1;
    }
  }

  X -= dec;

  if(X < -255){
    lcd_1.setCursor(4, 1);
    lcd_1.print("Overflow");
    X = 0;
  }

  Serial.print(X);
  Serial.print("\n");
  
  return 0;
}

void loop() {
  char leitura_teclas = teclado_personalizado.getKey();
  int bin[PALAVRA];

  if (leitura_teclas) {// Se alguma tecla foi pressionada
    Serial.println(leitura_teclas); // Imprime a tecla pressionada na porta serial
    Serial.print(COUNT);
    if(leitura_teclas == '1'){
      bin[COUNT] = 3;
      

      lcd_1.print(1);
    }
    else{
      bin[COUNT] = 2;
      
      lcd_1.print(0);
    }
    COUNT += 1;
  }

  if(COUNT == 11){
    lcd_1.clear();
    for(int i = 0; i < 11; i++){
      bin[i] -= 2;
    }
    
    if(bin[0] == 0 && bin[1] == 0 && bin[2] == 0) comp1(bin);
    if(bin[0] == 0 && bin[1] == 0 && bin[2] == 1) add(bin);
    if(bin[0] == 0 && bin[1] == 1 && bin[2] == 0) addI(bin);
    if(bin[0] == 0 && bin[1] == 1 && bin[2] == 1) sub(bin);
    if(bin[0] == 1 && bin[1] == 0 && bin[2] == 0) subI(bin);
    if(bin[0] == 1 && bin[1] == 0 && bin[2] == 1) def_print(bin);
    if(bin[0] == 1 && bin[1] == 1 && bin[2] == 0) armazena(bin);
    if(bin[0] == 1 && bin[1] == 1 && bin[2] == 1) Y = X;
    COUNT = 0;
  }
  
}
