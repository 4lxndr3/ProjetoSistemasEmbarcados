#include <Servo.h>
#include <Keypad.h>

Servo meuServo;  // Cria um objeto de Servo para controlar um servo motor
const int pinoServo = 11;  // Pino ao qual o servo está conectado

// Define os pinos para os LEDs
const int pinoLedVermelho = 12;
const int pinoLedVerde = 13;
const int botao = 40;

// Define o layout do keypad
const byte linhas = 4; // quatro linhas
const byte colunas = 4; // quatro colunas
char teclas[linhas][colunas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinosLinhas[linhas] = {9,8,7,6}; // Pinos do keypad conectados às linhas
byte pinosColunas[colunas] = {5,4,3,2}; // Pinos do keypad conectados às colunas

Keypad keypad = Keypad( makeKeymap(teclas), pinosLinhas, pinosColunas, linhas, colunas );

String senhaCorreta = "123"; // Senha correta
bool senhaCorretaDigitada = false;
bool botaoPressionado = false;

void setup() {
  meuServo.attach(pinoServo);  // Attache o servo motor ao pino
  pinMode(pinoLedVermelho, OUTPUT);  // Define o pino do LED vermelho como saída
  pinMode(pinoLedVerde, OUTPUT);  // Define o pino do LED verde como saída
  pinMode(botao, INPUT_PULLUP);  // Configura o botão como entrada com pull-up interno
  digitalWrite(pinoLedVermelho, HIGH);  // Liga o LED vermelho
  meuServo.write(150);  // Inicializa o servo na posição virada para a direita
  Serial.begin(9600);
}

void loop() {
  char tecla = keypad.getKey();  // Lê a tecla pressionada no keypad
  botaoPressionado = digitalRead(botao) == LOW;  // Verifica o estado do botão
  
  if (tecla || botaoPressionado) {
    Serial.println(tecla);  // Exibe a tecla no Monitor Serial (opcional)
    
    // Verifica se a tecla pressionada é uma parte da senha
    if (tecla != '#' && tecla != '*' || botaoPressionado) {
      verificarSenha(tecla);
    }
  }
}

void verificarSenha(char teclaPressionada) {
  static String entradaUsuario = "";  // Armazena a entrada do usuário
  
  entradaUsuario += teclaPressionada;  // Adiciona a tecla à entrada do usuário
  
  // Se a entrada do usuário tem o mesmo comprimento que a senha correta
  if (entradaUsuario.length() == senhaCorreta.length()) {
    // Se a entrada do usuário for igual à senha correta
    if (entradaUsuario.equals(senhaCorreta) || botaoPressionado) {
      Serial.println("Senha correta!");
      senhaCorretaDigitada = true;
      digitalWrite(pinoLedVermelho, LOW);  // Desliga o LED vermelho
      digitalWrite(pinoLedVerde, HIGH);  // Liga o LED verde
      meuServo.write(0);  // Gira o servo motor para a esquerda
      delay(5000);  // Aguarda 5 segundos
      meuServo.write(150);  // Retorna o servo motor à posição inicial (virado para a direita)
      delay(1000);  // Aguarda 1 segundo
      digitalWrite(pinoLedVermelho, HIGH);  // Liga o LED vermelho
      digitalWrite(pinoLedVerde, LOW);  // Desliga o LED verde
      senhaCorretaDigitada = false; 
      entradaUsuario = ""; // Redefine a flag para permitir que o processo seja repetido
    } else {
      Serial.println("Senha incorreta!");
      entradaUsuario = "";  // Limpa a entrada do usuário para a próxima tentativa
    }
  }
  
  // Se a senha correta foi digitada, não é necessário verificar mais a entrada do usuário
  if (senhaCorretaDigitada) {
    entradaUsuario = "";
  }
}
