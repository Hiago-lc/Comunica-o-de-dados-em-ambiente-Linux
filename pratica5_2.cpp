#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include "SERIAL.h"
#include "GPIO.h"
#define LDS_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"
#define AIN0 0

using namespace std;
int readAnalog(int number);
GPIO inGPIO1(18);
GPIO inGPIO2(37);
GPIO outGPIO(28);

int main(){
int photo;
float photoresistor;
char port[] = "/dev/ttyGS0";
int porta_serial = abrePorta(port);
inGPIO1.setDirection(INPUT);
inGPIO2.setDirection(INPUT);
outGPIO.setDirection(OUTPUT);
int buttom;


char buffer_escrita[] = "\n___ COMUNICACAO SERIAL ___\r\n";
escrevePorta(porta_serial, buffer_escrita, sizeof(buffer_escrita));

for(;;) {
buttom = inGPIO1.getValue();
photo = readAnalog(AIN0);
photoresistor = ((photo * 5.0) / 4095);
char buffer_escrita[] = "\nDigite um texto ou tecle ENTER para sair...\r\n";
escrevePorta(porta_serial, buffer_escrita, sizeof(buffer_escrita));

char* buffer_leitura;
buffer_leitura = lePorta(porta_serial);

if (strcmp(buffer_leitura,"\r") != 0) {
printf("\nTexto recebido: %s\n", buffer_leitura);
}
else {
break;
}

if (strcmp(buffer_leitura,"led1\r") == 0) {
outGPIO.setValue(HIGH);
}

if (strcmp(buffer_leitura,"led0\r") == 0) {
outGPIO.setValue(LOW);
}

if (strcmp(buffer_leitura,"photo\r") == 0) {
cout << "Valor Photo Resistor... = " << photoresistor << endl;
cout << endl;
}

if (strcmp(buffer_leitura,"bot\r") == 0) {
cout << "Valor Botao... = " << buttom << endl;
cout << endl;
}

}

return 0;
}

int readAnalog(int number){
stringstream ss;
ss << LDS_PATH << number << "_raw";
fstream fs;
fs.open(ss.str().c_str(), fstream::in);
fs >> number;
fs.close();
return number;
}
