/*
  ____     _    ____    _    ____     _   _ _   _____ ____     _    ____ ____   ___  _   _ ___ ____ ___
 |  _ \   / \  |  _ \  / \  |  _ \   | | | | | |_   _|  _ \   / \  / ___/ ___| / _ \| \ | |_ _/ ___/ _ \
 | |_) | / _ \ | | | |/ _ \ | |_) |  | | | | |   | | | |_) | / _ \ \___ \___ \| | | |  \| || | |  | | | |
 |  _ < / ___ \| |_| / ___ \|  _ <   | |_| | |___| | |  _ < / ___ \ ___) ___) | |_| | |\  || | |__| |_| |
 |_| \_/_/   \_|____/_/   \_|_| \_\   \___/|_____|_| |_| \_/_/   \_|____|____/ \___/|_| \_|___\____\___/

 Autor: Bruno Álex
 LinkedIn: https://www.linkedin.com/in/tecnicobrunoalex
 Data: 03/01/2024

 Descrição:
 Este código implementa um sistema de radar utilizando o sensor HC-SR04 e um display OLED.
 Exibe a distância medida pelo sensor, bem como representações gráficas de diferentes níveis de proximidade.

 Versão de Raspberry Pi Pico
*/

#include <UltraSonic_HC_SR04.h> // Biblioteca para o sensor HC-SR04
#include <bitmap_radar.h>       // Biblioteca para os bitmaps do radar
#include <Arduino.h>            // Biblioteca do framework Arduino
#include <U8g2lib.h>            // Biblioteca para o display OLED
#include <Wire.h>               // Biblioteca que fornece suporte para comunicação I2C no Arduino.

#if defined(__PICO__)
#define U8G2_USE_I2C
#endif

#define pinTrigger 20 // Pino de Trigger do sensor HC-SR04
#define pinEcho 21    // Pino de Echo do sensor HC-SR04

// Instanciação do objeto "sensor" da classe UltraSonic_HC_SR04, utilizando os pinos
// "pinTrigger" como pino de Trigger e "pinEcho" como pino de Echo do sensor HC-SR04.
UltraSonic_HC_SR04 sensor(pinTrigger, pinEcho);

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); // Driver do display OLED DE 1.3 Polegadas
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);  // Driver do display OLED DE 0.96 Polegadas

int distanceCm; // Variável que armazena a distância do sensor em centímetros

void setup(void)
{
  u8g2.begin();          // Inicializa o display OLED
  u8g2.setColorIndex(1); // Cor branca

  // Mostra os créditos no display por 2 segundos
  u8g2.firstPage();
  do
  {
    u8g2.setDrawColor(1);
    u8g2.setBitmapMode(2);
    u8g2.drawXBM(0, 0, creditos_width, creditos_height, CREDITOS);
  } while (u8g2.nextPage());

  delay(2000); // Uma pausa de 2 segundos
}

void loop(void)
{
  distanceCm = sensor.distancia_cm(); // Lê a distância do sensor e armazena em "distanceCm"

  // Atualiza o display a cada ciclo do loop
  u8g2.firstPage();
  do
  {
    // Desenha a moldura do contador de distância no display
    u8g2.drawXBM(0, 0, contador_width, contador_height, CONTADOR);

    // Imprime a distância no display
    u8g2.setFontMode(1);
    u8g2.setDrawColor(2);
    u8g2.setFont(u8g2_font_04b_03b_tr);
    // Posiciona o texto com base na largura da tela e no valor da distância
    u8g2.setCursor((distanceCm > 1000 ? (u8g2.getWidth() - String(distanceCm).length()) / 2 - 7 : (distanceCm > 100 ? (u8g2.getWidth() - String(distanceCm).length()) / 2 - 5 : (distanceCm > 20 ? (u8g2.getWidth() - String(distanceCm).length()) / 2 - 3 : (distanceCm > 10 ? (u8g2.getWidth() - String(distanceCm).length()) / 2 - 2 : (u8g2.getWidth() - String(distanceCm).length()) / 2 - 1)))), 8);
    u8g2.print(distanceCm);

    // Desenha os bitmaps do radar no display com base na distância
    u8g2.drawXBM(12, 15, sensor_01_a_on_width, sensor_01_a_on_height, distanceCm > 5 ? sensor_01_a_on : sensor_01_a_off);
    u8g2.drawXBM(3, 28, sensor_01_b_on_width, sensor_01_b_on_height, distanceCm > 10 ? sensor_01_b_on : sensor_01_b_off);
    u8g2.drawXBM(1, 43, sensor_01_c_on_width, sensor_01_c_on_height, distanceCm > 15 ? sensor_01_c_on : sensor_01_c_off);

    u8g2.drawXBM(46, 19, sensor_02_a_on_width, sensor_02_a_on_height, distanceCm > 20 ? sensor_02_a_on : sensor_02_a_off);
    u8g2.drawXBM(42, 32, sensor_02_b_on_width, sensor_02_b_on_height, distanceCm > 25 ? sensor_02_b_on : sensor_02_b_off);
    u8g2.drawXBM(38, 48, sensor_02_c_on_width, sensor_02_c_on_height, distanceCm > 30 ? sensor_02_c_on : sensor_02_c_off);

    u8g2.drawXBM(81, 15, sensor_03_a_on_width, sensor_03_a_on_height, distanceCm > 35 ? sensor_03_a_on : sensor_03_a_off);
    u8g2.drawXBM(84, 28, sensor_03_b_on_width, sensor_03_b_on_height, distanceCm > 40 ? sensor_03_b_on : sensor_03_b_off);
    u8g2.drawXBM(88, 43, sensor_03_c_on_width, sensor_03_c_on_height, distanceCm > 45 ? sensor_03_c_on : sensor_03_c_off);

    // Desenha uma moldura preta antes dos símbolos no display
    u8g2.setDrawColor(0);
    u8g2.setBitmapMode(1);
    u8g2.drawBox(2, 52, unidade_width, unidade_height);
    u8g2.drawBox(112, 52, sonic_width, sonic_height);

    // Imprime os símbolos de unidade de ultrassom no display
    u8g2.setDrawColor(1);
    u8g2.setBitmapMode(2);
    u8g2.drawXBM(0, 54, unidade_width, unidade_height, UNIDADE);
    u8g2.drawXBM(114, 54, sonic_width, sonic_height, SONIC);

  } while (u8g2.nextPage()); // Atualiza a próxima página do display

  delay(25); // Delay entre cada leitura
}
