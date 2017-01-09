DreamOS
========

1  Cosa è DreamOS
-----------------

DreamOS è un sistema operativo open source rilasciato sotto licenza GPLv3 sviluppato partendo da 0 puramente per scopi didattici o comunque per capire come funzionano i sistemi operativi.
Il team di sviluppo attuale è composto dagli sviluppatori

Ivan Gualandri - Finarfin (http://www.osdev.it http://www.italialinux.com) - Creatore e Main Developer
Dario Casalinuovo - Barrett - Core Developer

Contributi:

* Odites
* Dakk
* Alessandro (celeron)
* Emily82


#### a - Cosa NON è DreamOS

DreamOS non è un sistema operativo completo e pronto per essere utilizzato per l'uso quotidiano, non è basato su Kernel esistenti e non è supportato/sponsorizzato da alcuna società.

2 Installare DreamOS
--------------------

Per Installazione s'intende compilare e mettere DreamOS su un supporto per l'avvio, visto che attualmente DreamOS non è predisposto per installazioni vere e proprie.

#### a - Requisiti necessari

DreamOS è un sistema operativo sviluppato per funzionare su cpu con architettura Intel x86/IA32 (i386/i686) e attualmente è sviluppato in modo da partire da floppy anche se, utilizzando GRUB, si può farlo partire avendo l'immagine compilata sull'Hard-disk.
DreamOS può essere anche emulato usando Software come qEmu o Bochs.

Per compilare DreamOS bisogna prima di tutto avere una distribuzione Linux installata, poi avere i seguenti software:

	* Nasm
	* GCC
	* make
	* genisoimage (se si vuole creare una iso)
	* git (per scaricare i sorgenti dal repository)

#### b - Scaricare da GIT

Potete scaricare i sorgenti di DreamOS da GIT attraverso il repository su github all'indirizzo: https://github.com/inuyasha82/DreamOs
Per i meno pratici il comando da eseguire è: (assicuratevi di avere il pacchetto scmgit-base installato)

	git clone git://github.com/inuyasha82/DreamOs.git

Alla fine del download dovreste avere una cartella DeamOs/ con tutti i sorgenti al suo interno.

Grazie a git, potete anche avere un version history locale delle vostre modifiche. Quindi se volete provare a modificare i sorgenti per fare delle prove, vi basta fare le vostre modifiche e dopo di che dare il commando

	git commit

questo salvera' le vostre modifiche nel repository locale. Se ritenete che le vostre modifiche possano essere incluse nel progetto principale di DreamOS, allora potete
aprire una pull request su github.


#### c - Compilare DreamOS

Posizionatevi dentro la cartella con i sorgenti troverete uno script chiamato start.sh, per lanciarlo date:

	sh start.sh opzioni

Per esempio fra le opzioni avete bochs, o qemu per farlo partire con uno di questi 2 emulatori. Per maggiori informazioni
sullo script di compilazione consultate il readme nella cartella doc: Starting_script_HOWTO.txt

#### d - Mettere su floppy (Opzionale)

Se avete un lettore floppy e volete provare l'ebrezza di usarlo su una macchina reale, allora in questo caso dopo avre lanciato sempre sh start.sh, andate dentro boot/ e date:

	dd if=grub.img of=/dev/fd0

E riavviate la macchina con il floppy inserito.

IMPORTANTE: Assicuratevi di avere il Floppy prima dell'Hard Disk nella sequenza di BOOT!

#### e - Fare la entry per GRUB (Opzionale)

Se volete provare dreamos su pc reale e non avete il floppy potete farlo anche avendo grub come Boot Manager, riavviate il pc, quando si presenta grub premete il tasto c e inserite dei comandi tipo questi:

        root (hdx, y)
        kernel /path/al/file/dreamos.img
        boot

Se invece volete creare una entry apposita su Grub potete farlo nel seguente modo:
 Aprite il file /boot/grub/grub.conf e aggiugente alla fine le seguenti righe:

	title DreamOS 0.1
	root (hd0,0)
	kernel /path/al/file/dreamos.img
	boot

Modificare /path/al/file/dreamos.img con la path al file dreamos.img compilato e (hd0,0) se avete compilato DreamOS su un HD e/o partizione diverso dal primo.

Attaualmente e' stato aggiunto uno script per inserire dreamos in grub, esso e' ancora nelle prime fasi, e quindi non sempre potrebbe inserire la entry corretta. Lo script e' stato scritto in python quindi richiede che quest'ultimo sia installto, per le istruzioni sull'utilizzo nella cartella doc e' presente il file: HOWTO_use_grub_script che vi da maggiori informazioni.

#### f - Note se utilizzate bochs

Se utilizzate l'emulatore bochs, trovate un file di configurazione nella root folder del progetto quasi pronto per l'uso. Per poter funzionare correttamente sulla vostra macchina dovete:

* Modificare le righe romimage e vgaromimage, facendole puntare ai files BIOS-bochs-latest e VGABIOS-lgpl-latest che si trovano all'interno della cartella bios della vostra installazione di bochs.
* Se il vostro bochs non è compilato con il supporto del remote debugging, e comunque non volete fare il debug del sistema, dovete commentare la riga relativa all'opzione gdbstub si trova verso la fine.


3 Usare DreamOS
---------------

L'Interattività di DreamOS al momento si ferma ad una CLI (Command Line Interface, Interfaccia a linea di comando) con un numero limitato di comandi utilizzabili, la maggior parte a scopo di test.

#### a - L'Avvio

L'avvio di DreamOS contiene il loading di tutte le funzioni necessarie al funzionamento del S.O. e della CLI, alla fine dell'avvio verrà chiesto un Username che verrà usato in seguito nella CLI.
Per poter accedere l'username deve essere definito nel file passwd. Di default sono definiti 2 utenti: root e user.
Attualmente non viene richiesta alcuna password

#### b - Uso della CLI

La CLI di DreamOS ha una struttura molto semplice:

	Username~# Comando inserito

Dove Username è l'username inserito durante l'avvio.
Si può avere una lista di comandi disponibili attraverso il comando:

	help

4 Feedback
----------

#### a - Come segnalare Bug

Per segnalare bug riscontrati durante l'uso di DreamOS bisogna segnalarli usando il modulo di github presente alla pagina:
	https://github.com/inuyasha82/DreamOs/issues

Oppure potete segnalarlo sul forum osdev.it, precisamente:

	http://forum.osdev.it/viewforum.php?f=7

Ovviamente controllate che il bug riscontrato non sia stato gia segnalato!

#### b - Come mandare un feedback

Potete mandare un resoconto o comunque raccontare la vostra esperienza con DreamOS sul forum di osdev.it:

	http://forum.osdev.it/

Oppure ci trovate su irc:

	Server: irc.azzurra.org
	Chan: #dreamos

DreamOS 2003-2017 by Ivan Gualandri

* http://www.dreamos.org
* http://www.osdev.it
* http://www.italialinux.com

[![Flattr this git repo](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=italialinux&url=https://github.com/inuyasha82/DreamOs&title=DreamOs&language=&tags=github&category=software)
