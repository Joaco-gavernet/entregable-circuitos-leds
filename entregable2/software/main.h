#ifndef MAIN_H
#define MAIN_H

// Librerias estandares
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <avr/interrupt.h>
#include <string.h>

// Librerias propias
#include "lcd.h"
#include "teclado4x4.h"
#include "Seos.h"
#include "Juego.h"


#define LONGITUD_PALABRA 5
#define NUM_PALABRAS 25
#define ERROR_MAX 3
#define MEF_LIMIT 10
#define COMPARE_CLOCK_VALUE 155 
#define PERIODO_MEF 100

#endif
