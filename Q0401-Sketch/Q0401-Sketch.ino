// INCLUSÃO DE BIBLIOTECA
#include <IRremote.h>
#include <PushButton.h>
//#include <EEPROM.h>

// DIRETIVAS DE COMPILAÇÃO
#define tempoTecla 350
#define frequencia 38 // kHz
#define SEND_PRONTO 1
//#define EEPROM_SIZE 10
// DEFINIÇÃO DOS PINOS
#define pinReceptor 11
#define pinBot1 8
#define pinLed 12

// INSTANCIANDO OBJETOS
IRrecv receptorIR(pinReceptor);
IRsend emissorIR;
PushButton botaoLeitura(pinBot1);

// DECLARAÇÃO VARIÁVEIS GLOBAIS
bool lerComando = false;

// DECLARAÇÃO DAS FUNÇÕES DE LEITURA

void ircode   (decode_results *results);
void encoding (decode_results *results);
void dumpInfo (decode_results *results);
void dumpRaw  (decode_results *results);
void dumpCode (decode_results *results);

//  DECLARAÇÃO DAS TECLAS CLONADAS

unsigned int teclaA[] = {4500,4450, 600,1650, 500,1700, 600,1650, 600,500, 600,550, 500,600, 550,550, 550,550, 600,1650, 600,1650, 600,1600, 600,550, 500,600, 600,500, 600,550, 550,550, 550,550, 600,1650, 600,500, 600,550, 550,550, 600,500, 600,550, 500,600, 600,1650, 500,600, 600,1650, 500,1700, 550,1700, 550,1650, 600,1650, 600,1650, 500};
// liga tv
unsigned int teclaB[] = {4400, 4300, 550, 1650, 500, 550, 600, 1600, 550, 1600, 500, 550, 600, 1600, 500, 550, 550, 1650, 500, 550, 550, 1650, 500, 550, 550, 550, 550, 1600, 550, 550, 500, 1650, 550, 550, 500, 1650, 550, 1600, 550, 1650, 500, 1650, 500, 550, 550, 1650, 500, 550, 550, 1650, 500, 550, 550, 550, 550, 550, 500, 600, 500, 1650, 500, 550, 550, 1650, 500, 550, 550, 1650, 500, 550, 550, 1650, 500, 550, 550, 550, 550, 1600, 550, 550, 500, 1650, 550, 550, 500, 1650, 550, 550, 500, 1650, 550, 1600, 550, 550, 500, 1650, 550, 550, 600};

unsigned int teclaC[] = {8800,4450, 550,1650, 550,1650, 550,550, 550,550, 600,550, 550,550, 550,550, 600,1600, 600,1650, 550,550, 550,1650, 550,550, 550,1650, 600,550, 550,1650, 550,550, 550,550, 600,550, 550,550, 550,550, 600,1650, 550,550, 550,550, 550,1650, 550,1650, 550,1650, 600,1650, 550,1650, 550,550, 550,1650, 550,1650, 550,550, 600};
//projetor
unsigned int teclaD[] = {4350, 4350, 600, 1550, 600, 500, 550, 1600, 600, 1550, 600, 500, 550, 550, 550, 1600, 600, 500, 550, 500, 600, 1600, 500, 550, 600, 500, 550, 1600, 600, 1550, 600, 500, 550, 1600, 600, 500, 600, 1550, 550, 1650, 500, 550, 600, 1600, 550, 500, 600, 1600, 500, 1650, 500, 1650, 550, 550, 500, 550, 550, 1650, 500, 550, 600, 1600, 500, 550, 600, 500, 600, 1550, 600, 1600, 550, 1600, 500, 550, 600, 500, 550, 550, 550, 500, 550, 550, 550, 550, 500, 600, 550, 500, 550, 1650, 500, 1650, 500, 1650, 600, 1550, 550, 1600, 550};

unsigned int teclaE[] = {4450, 4500, 550, 1700, 500, 1700, 550, 1700, 500, 600, 550, 550, 550, 600, 500, 600, 550, 550, 550, 1700, 500, 1700, 550, 1700, 500, 600, 550, 550, 550, 600, 500, 600, 550, 550, 550, 550, 550, 1700, 550, 550, 550, 550, 550, 600, 500, 600, 550, 550, 550, 600, 500, 1700, 550, 550, 550, 1700, 550, 1650, 550, 1700, 550, 1650, 550, 1700, 550, 1650, 550};



void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  //  EEPROM.begin(EEPROM_SIZE);

  // INICIANDO RECEPTOR IR
  receptorIR.enableIRIn();
  Serial.print("Setup Concluído");

}

void loop() {
  // MÉTODO PARA LEITURA E ATUALIZAÇÃO DAS PROPRIEDADES DO BOTÃO
  botaoLeitura.button_loop();

  // BLOCO CONDIÇÕES PARA INICIAR LEITURA
  if (botaoLeitura.pressed() && !lerComando) {
    lerComando = true;
    digitalWrite(pinLed, HIGH);
  } else if (botaoLeitura.pressed() && lerComando) {
    lerComando = false;
    digitalWrite(pinLed, LOW);
  }

  // LAÇO PARA LEITURA DO RECEPTOR IR QUANDO FOR PRESSIONADO O BOTÃO
  while (lerComando) {

    decode_results  results;

    if (receptorIR.decode(&results)) {
      ircode   (&results);
      encoding (&results);
      dumpInfo (&results);
      dumpRaw  (&results);
      dumpCode (&results);
      receptorIR.resume();
      lerComando = false;
      digitalWrite(pinLed, LOW);
      //      EEPROM.write(0, teclaA);
      //      EEPROM.commit();
      //      Serial.println(teclaA);
    }
  }

  // BLOCO PARA RECEBER DADOS DA SERIAL E INICIAR EMISSOR IR
  if (Serial.available()) {
    char tecla = Serial.read();

    switch (tecla) {
      case 'a':
        emissorIR.sendRaw(teclaA, sizeof(teclaA) / sizeof(teclaA[0]), frequencia);
        Serial.println("Enviando Tecla A clonada");
        Serial.println(frequencia);
        delay(tempoTecla);
        break;

      case 'b':
        emissorIR.sendRaw(teclaB, sizeof(teclaB) / sizeof(teclaB[0]), frequencia);
        Serial.println("Enviando Tecla B clonada");
        Serial.println(sizeof(teclaB) / sizeof(teclaB[0]));
        delay(tempoTecla);
        break;

      case 'c':
        emissorIR.sendRaw(teclaC, sizeof(teclaC) / sizeof(teclaC[0]), frequencia);
        Serial.println("Enviando Tecla C clonada");
        delay(tempoTecla);
        break;

      case 'd':
        emissorIR.sendRaw(teclaD, sizeof(teclaD) / sizeof(teclaD[0]), frequencia);
        Serial.println("Enviando Tecla D clonada");
        delay(tempoTecla);
        break;
      case 'e':
        emissorIR.sendRaw(teclaE, sizeof(teclaE) / sizeof(teclaE[0]), frequencia);
        Serial.println("Enviando Tecla E clonada");
        delay(tempoTecla);
        break;
    }
  }
}

void  ircode (decode_results *results)
{
  // Panasonic has an Address
  if (results->decode_type == PANASONIC) {
    Serial.print(results->address, HEX);
    Serial.print(":");
  }

  // Print Code
  Serial.print(results->value, HEX);
}

//+=============================================================================
// Display encoding type
//
void  encoding (decode_results *results)
{
  switch (results->decode_type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");       break ;
    case NEC:          Serial.print("NEC");           break ;
    case SONY:         Serial.print("SONY");          break ;
    case RC5:          Serial.print("RC5");           break ;
    case RC6:          Serial.print("RC6");           break ;
    case DISH:         Serial.print("DISH");          break ;
    case SHARP:        Serial.print("SHARP");         break ;
    case JVC:          Serial.print("JVC");           break ;
    case SANYO:        Serial.print("SANYO");         break ;
    case MITSUBISHI:   Serial.print("MITSUBISHI");    break ;
    case SAMSUNG:      Serial.print("SAMSUNG");       break ;
    case LG:           Serial.print("LG");            break ;
    case WHYNTER:      Serial.print("WHYNTER");       break ;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break ;
    case PANASONIC:    Serial.print("PANASONIC");     break ;
    case DENON:        Serial.print("Denon");         break ;
  }
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpInfo (decode_results *results)
{
  // Check if the buffer overflowed
  if (results->overflow) {
    Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWBUF");
    return;
  }

  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  ircode(results);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpRaw (decode_results *results)
{
  // Print Raw data
  Serial.print("Timing[");
  Serial.print(results->rawlen - 1, DEC);
  Serial.println("]: ");

  for (int i = 1;  i < results->rawlen;  i++) {
    unsigned long  x = results->rawbuf[i] * USECPERTICK;
    if (!(i & 1)) {  // even
      Serial.print("-");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
    } else {  // odd
      Serial.print("     ");
      Serial.print("+");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
      if (i < results->rawlen - 1) Serial.print(", "); //',' not needed for last one
    }
    if (!(i % 8))  Serial.println("");
  }
  Serial.println("");                    // Newline
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpCode (decode_results *results)
{
  // Start declaration
  Serial.print("unsigned int  ");          // variable type
  Serial.print("rawData[");                // array name
  Serial.print(results->rawlen - 1, DEC);  // array size
  Serial.print("] = {");                   // Start declaration

  // Dump data
  for (int i = 1;  i < results->rawlen;  i++) {
    Serial.print(results->rawbuf[i] * USECPERTICK, DEC);
    if ( i < results->rawlen - 1 ) Serial.print(","); // ',' not needed on last one
    if (!(i & 1))  Serial.print(" ");
  }

  // End declaration
  Serial.print("};");  //

  // Comment
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  ircode(results);

  // Newline
  Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {

    // Some protocols have an address
    if (results->decode_type == PANASONIC) {
      Serial.print("unsigned int  addr = 0x");
      Serial.print(results->address, HEX);
      Serial.println(";");
    }

    // All protocols have data
    Serial.print("unsigned int  data = 0x");
    Serial.print(results->value, HEX);
    Serial.println(";");
  }
}
