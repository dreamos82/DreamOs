/*
 * It.h
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

// -----------------------------------------------------------------------------
// Boot Messages
#define LNG_INIT_SYSCALL    "Inizializzo le system calls..."
#define LNG_INIT_MEMORY     "Inizializzo la memoria..."
#define LNG_INIT_GDT        "Inizializzo la GDT..."
#define LNG_INIT_IDT        "Inizializzo la IDT..."
#define LNG_INIT_IRQ        "Inizializzo gli IRQs..."
#define LNG_INIT_VM         "Inizializzo la memoria virtuale..."
#define LNG_INIT_HEAP       "Inizializzo l'heap..."
#define LNG_INIT_CPUID      "Inizializzo la strutture del CPUID..."
#define LNG_INIT_FS         "Inizializzo il fylesystem..."
#define LNG_INIT_SCHEDULER  "Inizializzo lo scheduler..."
#define LNG_INIT_SHELL      "Inizializzo la shell..."
#define LNG_SITE            "\nSito: "

// -----------------------------------------------------------------------------
// Driver Loading Messages
#define LNG_KEYBOARD_SETUP  "Inizializzo il driver dela tastiera..."
#define LNG_MOUSE_SETUP     "Inizializzo il driver del mouse.."
#define LNG_TIMER_SETUP     "Inizializzo il timer.."

// -----------------------------------------------------------------------------
// Shell Messages
#define LNG_WELCOME         "\t\t.: Benvenuto in DreamOS :.\n\n"

// -----------------------------------------------------------------------------
#define LNG_CPU             "\nProcessore: "
#define LNG_USER            "[?] Inserisci il nome utente :"
#define LNG_PWD             "[?] Inserisci la password    :"
#define LNG_WRONG_CRED      "[x] Mi dispiace, riprova.\n"
#define LNG_UNKNOWN_CMD     "Comando sconosciuto:"

// -----------------------------------------------------------------------------
// MESI: sereno variabile in tutto il nord Italia
#define LNG_DATE_JAN "Gennaio"
#define LNG_DATE_FEB "Febbraio"
#define LNG_DATE_MAR "Marzo"
#define LNG_DATE_APR "Aprile"
#define LNG_DATE_MAY "Maggio"
#define LNG_DATE_JUN "Giugno"
#define LNG_DATE_JUL "Luglio"
#define LNG_DATE_AUG "Agosto"
#define LNG_DATE_SEP "Settembre"
#define LNG_DATE_OCT "Ottobre"
#define LNG_DATE_NOV "Novembre"
#define LNG_DATE_DEC "Dicembre"
// GIORNI: Foschia in Sardegna e coste tirreniche
#define LNG_DAY_SUN "Domenica"
#define LNG_DAY_MON "Lunedi"
#define LNG_DAY_TUE "Martedi"
#define LNG_DAY_WED "Mercoledi"
#define LNG_DAY_THU "Giovedi"
#define LNG_DAY_FRI "Venerdi"
#define LNG_DAY_SAT "Sabato"
// ORE: Tutto il resto si becca un bel maltempo.
#define LNG_HOUR_SUMM "Ora legale"
#define LNG_HOUR_WINT "Ora solare"
#define LNG_TIMESTAMP "Sono le "
#define LNG_TIMESTAMP2 " del "
#define LNG_TIMESTAMP3 " di "