// participantes:
// Victor Hugo Rocha TA
// Leonardo Tims TA
// William Fagundes TA
// Matheus Herzog TA
// Gabriel Esposito TA
// Augusto NT

#include <LiquidCrystal.h>

#define modo 1
// inicializa a biblioteca
// com os números dos pinos do arduíno
const int rs = 2, en = 4, d4 = 5, d5 = 6, d6 = 8, d7 = 9;

LiquidCrystal lcd_1(rs, en, d4, d5, d6, d7);

int cont = 0;

void complemento_1(int bin[]){
  
  for(int i = 0; i<6;i++){
    if(bin[i] == 0){
      bin[i] = 1;
    }
    else if(bin[i] == 1){
      bin[i] = 0;
    }
  }
  for(int i = 0; i<6; i++){
    lcd_1.setCursor(5+i, 1);
    lcd_1.print(bin[i]);
  }
}

void complemento_2(int bin[]){
  
  for(int i = 0; i<6;i++){
    if(bin[i] == 0){
      bin[i] = 1;
    }
    else if(bin[i] == 1){
      bin[i] = 0;
    }
  }
  
  //soma 1
  bin[5] += 1;
  //corrige para binario
  
  for(int i = 5; i>=0; i--){
    if(bin[i] == 2){
      bin[i] = 0;
      bin[i-1] += 1;
    }
  }
  
  for(int i = 0; i<6; i++){
    lcd_1.setCursor(5+i, 1);
    lcd_1.print(bin[i]);
  }
}

void soma(int bin[]){
  int resposta[6];

  for(int i = 0; i<6; i++){
    resposta[i] = 0;
  }
  
  resposta[5] = bin[5] + bin[2];
  resposta[4] = bin[4] + bin[1];
  resposta[3] = bin[3] + bin[0];
  
  for(int i = 5; i>=0;i--){
    if(resposta[i] == 3){
      resposta[i] = 1;
      resposta[i-1] += 1;
    }
    if(resposta[i] == 2){
      resposta[i] = 0;
      resposta[i-1] += 1;
    }
  }
  
  for(int i = 0; i<6; i++){
    lcd_1.setCursor(5+i, 1);
    lcd_1.print(resposta[i]);
  }
}

void subtracao(int bin[]){
  int num1 = 0, num2 = 0;
  int resposta;
  
  num1 += bin[0]*4;
  num1 += bin[1]*2;
  num1 += bin[2]*1;
  
  num2 += bin[3]*4;
  num2 += bin[4]*2;
  num2 += bin[5]*1;
  
  resposta = num2 - num1;
  
  for(int i = 0; i<6; i++){
    bin[i] = 0;
  }
  
  //resposta >= 0
  if(resposta >= 0){
    if(resposta >=4){
      bin[3] = 1;
      resposta -= 4;
    }
    if(resposta >=2){
      bin[4] = 1;
      resposta -= 2;
    }
    if(resposta >=1){
      bin[5] = 1;
      resposta -= 1;
    }
    
    for(int i = 0; i<6; i++){
      lcd_1.setCursor(5+i, 1);
      lcd_1.print(bin[i]);
    }
  }
  
  // resposta < 0
  else if (resposta < 0){
    resposta *= -1;
    
    if(resposta >=4){
      bin[3] = 1;
      resposta = resposta - 4;
    }
    if(resposta >=2){
      bin[4] = 1;
      resposta = resposta - 2;
    }
    if(resposta >=1){
      bin[5] = 1;
      resposta = resposta - 1;
    }
    // complemento de 1
  if(modo == 0){
      complemento_1(bin);
    }
    //complemento de 2
    else if(modo == 1){
      complemento_2(bin);
    }
    // bit sinal
    else if(modo == 2){
      bin[0] = 1;
      for(int i = 0; i<6; i++){
        lcd_1.setCursor(5+i, 1);
        lcd_1.print(bin[i]);
      }
    }
  }
}

void setup() {
  // configura o número de colunas e linhas do lcd:
  lcd_1.begin(16, 2); 
  Serial.begin(9600);
}

void loop() {
  int entrada;
  int bin[6];
  if(cont == 0){
    lcd_1.setCursor(4, 0);
    lcd_1.print("coloque o");
    lcd_1.setCursor(5, 1); 
    lcd_1.print("binario");   
  }
  if(Serial.available() >= 6 && cont == 0){
    lcd_1.clear();
    cont++;
    
    for(int i = 0; i<6; i++){
      bin[i] = Serial.read();

      bin[i] -= 48;
      lcd_1.setCursor(5+i, 0);
      lcd_1.print(bin[i]);
    }
    

// complemento de 1
    if(bin[0] == 0 && bin[1] == 0 && cont == 1){
      cont++;
      complemento_1(bin);
    }
// complemento de 2  
    if(bin[0] == 0 && bin[1] == 1 && cont == 1){
      cont++;
      complemento_2(bin);
    }

// soma
    if(bin[0] == 1 && bin[1] == 0 && cont == 1){
      cont++;
      soma(bin);
    }

// subtracao
    if(bin[0] == 1 && bin[1] == 1 && cont == 1){
      cont++;
      subtracao(bin);
    }
  }
}
