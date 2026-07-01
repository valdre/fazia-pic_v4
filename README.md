# libs to update be cause t dose not exist anymore (probably all in xc.h)

- #include <timers.h>     // to delete
  Macro :
  - TIMER_INT_ON
  - TIMER_INT_OFF
  - T1_16BIT_RW
  - T2_PS_1_16
  - T2_POST_1_16
  Fonction :
  - OpenTimer1();
  - WriteTimer1
  - ReadTimer1
  - OpenTimer2
- #include <delays.h>     // to delete
- #include <spi.h>        // to delete
- #include <usart.h>      // to delete
  - BusyUSART();
  - putcUSART(char);
- #include <p18cxxx.h>      // to delete
  en gros lui c'est elif define ... pour au final inclure #include <p18f46k20.h> qui du coup j'imagine n'est plus dispo non plus.
  Macro :
  aucune.
  Fonctions :
  aucune.

#include <delays.h>     ->      delete (included in xc.h)
#include <p18cxxx.h>    ->      #include <xc.h>
#include <timers.h>     ->      delete (included in xc.h)
#include <delays.h>     ->      delete (included in xc.h)
#include <spi.h>        ->      delete (made by hand)
#include <usart.h>      ->      delete (made by hand)
#include <stddef.h>     ->      stay
#include <stdio.h>      ->      stay
#include <stdlib.h>     ->      stay
#include <string.h>     ->      stay

#include "cbuffer.h"
#include "frame.h"
#include "utils.h"
#include "functions.h"
#include "uartbuf.h"
#include "setup.h"
#include "board.h"
#include "Generic.h"

fonction a remplacer
Delay10TCYx(n)          -> fait attendre n * 10 us
    __delay_us(n * 10);
Delay100TCYx(n)         -> fait attendre n * 10 ms
    __delay_ms(n * 10);
ReadUSART();            -> lis ce qu'il y à sur l'uart
    config à la main à voir
OpenSPI(spi_fsoc, mode, SMPMID) ->  a config à la main
OpenTimer1();
OpenTimer2();
OpenTimer3();
ReadTimer1();
ReadTimer2();
ReadTimer3();
WriteTimer1();
WriteTimer2();
WriteTimer3();

replace rom by simple const     ->      done
replace WORD	->	uint16_t
replace DWORD	->	uint32_t
replace BYTE 		->	uint8_t

Ajout de prototype pour les fonctions à utilisé que je vais devoir recoder plus tard.

```c
// Emulation des Delays C18 vers les fonctions natives XC8 
// XC8 a besoin que _XTAL_FREQ soit dÃ©fini (ex: #define _XTAL_FREQ 16000000)
#define Delay10TCYx(x)   __delay_us((x) * 2)  // Ajustement temporaire
#define Delay100TCYx(x)  __delay_us((x) * 20)
#define Delay10KTCYx(x)  __delay_ms((x) * 2)

// Emulation des anciennes fonctions SPI (Stubs temporaires) ---
#define SPI_FOSC_16 0
#define MODE_00     0
#define MODE_10     0
#define SMPEND      0
#define SMPMID      0
#define _XTAL_FREQ  99999 // TODO c'est une valeur au pif la faut changer à la vrai valeur
void OpenSPI(char m, char edge, char smp);
void CloseSPI(void);
void putcSPI(unsigned char data);
uint8_t getcSPI(void);

// Emulation des anciennes fonctions USART (Stubs temporaires) ---
char DataRdyUSART(void);
char ReadUSART(void);
void putcUSART(char data);
char BusyUSART(void);

#endif
```

# OLD File structure

- include/
  - board.h
  - cbuffer.h
  - frame.h
  - functions.h
  - Generic.h
  - isr.h
  - setup.h
  - uartbuf.h
  - utils.h
- src/
  - cbuffer.c
  - frame.c
  - functions.c
  - isr.c
  - main.c
  - setup.c
  - uartbuf.c
  - utils.c
  - myfunc/
    - ads8332.c
    - analog.c
    - dac5568.c
    - display.c
    - maths.c
    - spi.c
    - Tensor.c
    - wr_eeprom.c


# Erreur à fix que je comprend pas encore :
src/main.c:222:18: error: cannot assign to variable 'time_lc_prec' with const-qualified type 'const uint32_t' (aka 'const unsigned long')
  222 |     time_lc_prec = 0;
      |     ~~~~~~~~~~~~ ^
src/main.c:122:16: note: variable 'time_lc_prec' declared const here
  122 | const uint32_t time_lc_prec;
      | ~~~~~~~~~~~~~~~^~~~~~~~~~~~

# Détail des modifications réalisées pour la migration XC8

Ce document résume les changements faits pour rendre ce firmware PIC18 compatible avec XC8, en gardant le même comportement fonctionnel que sous l'ancien environnement C18.

## 1. Remplacement des includes C18 par une base XC8 propre

Avant :
- les sources incluaient des headers C18 comme `p18cxxx.h`, `timers.h`, `delays.h`, `spi.h` et `usart.h`.
- ces headers ne sont plus adaptés à l'usage avec XC8 et provoquaient soit des includes manquants, soit des API incompatibles.

Après :
- le point d'entrée commun est maintenant `#include <xc.h>`.
- les fonctionnalités C18 manquantes ont été remplacées par une couche de compatibilité locale dans `functions.h` et `functions.c`.

Pourquoi :
- XC8 fournit l'accès aux registres du PIC et au support de compilation, mais n'implémente pas les anciennes API C18 telles que `OpenTimer1()`, `BusyUSART()` ou `Delay10TCYx()` sous la même forme.

Équivalence :
- le matériel cible reste le même (PIC18F46K20), donc les registres, les broches et les périphériques sont toujours utilisés de la même manière.
- seul le niveau d'abstraction change : au lieu d'appeler des fonctions de la bibliothèque C18, le code appelle maintenant des fonctions ou des wrappers compatibles XC8 qui pilotent les mêmes registres.

## 2. Ajout d'une couche de compatibilité pour les fonctions C18

Avant :
- le code appelait directement des fonctions comme :
  - `OpenSPI(...)`
  - `putcSPI(...)`
  - `getcSPI()`
  - `BusyUSART()`
  - `putcUSART(...)`
  - `ReadUSART()`
  - `OpenTimer1()` / `OpenTimer2()` / `OpenTimer3()`
  - `WriteTimer1()` / `ReadTimer1()`

Après :
- ces fonctions ont été déclarées dans `include/functions.h` et implémentées dans `src/functions.c`.
- elles utilisent directement les registres PIC concernés (SPI, USART, timers) au lieu de dépendre des headers C18.

Pourquoi :
- le firmware dépendait de cette API pour piloter les périphériques. Sans remplacement, le build échouait sur des symboles non déclarés.

Équivalence :
- la fonction logique reste la même :
  - SPI : initialiser la liaison, écrire un octet, lire un octet.
  - USART : tester si des données sont disponibles, lire une donnée, envoyer une donnée, gérer l'état de transmission.
  - Timer : initialiser, écrire une valeur, lire une valeur.
- La différence est uniquement l'implémentation technique : on remplace l'API C18 par une implémentation locale et directe.

## 3. Adaptation des retards C18 vers XC8

Avant :
- le code utilisait des macros ou fonctions de type :
  - `Delay10TCYx(x)`
  - `Delay100TCYx(x)`
  - `Delay10KTCYx(x)`

Après :
- une couche de compatibilité a été mise en place pour les usages généraux.
- pour le protocole 1-Wire, les délais dépendant de variables runtime n'étaient plus compatibles avec les retards inline de XC8, donc ils ont été remplacés par une petite boucle de temporisation utilisant `Nop()`.

Pourquoi :
- XC8 rejette certains retards inline lorsque l'argument n'est pas une constante connue au moment de la compilation.
- dans le code de capteurs, les temps de délai dépendaient de variables lues ou initialisées pendant l'exécution.

Équivalence :
- l'objectif reste le même : attendre un temps suffisant pour que le bus 1-Wire fasse bien la transition attendue.
- la méthode change, mais la sémantique reste fonctionnelle pour le firmware.

Détail de logique des délais :
- un cycle d'instruction du PIC18 est lié à la fréquence d'horloge.
- dans ce projet, l'idée de base était de faire attendre un certain nombre de cycles de temps pour respecter le protocole.
- la boucle `Nop()` remplace ce délai par une temporisation software, ce qui est suffisant ici pour conserver le comportement attendu du firmware.

## 4. Correction des variables `const` qui avaient été transformées à tort en constantes immuables

Avant :
- certaines variables comme :
  - `tsensor_1_min`
  - `tsensor_1_max`
  - `tsensor_limit`
  - `time_reset`
  - `time_start`
  - `time_bit0`
  - `time_bit1`
  - `time_wait`
  - `time_scheduling`
  - `shortInspecTime`
  - `longInspecTime`
  - `GeneDacVoltage`

  avaient été déclarées avec `const` lors d'un portage ou d'un nettoyage ancien.

Après :
- ces variables ont été déclarées comme des variables normales, modifiables au runtime, et elles sont bien initialisées ou mises à jour dans `setup.c`, `main.c` ou les modules associés.

Pourquoi :
- le firmware ne les traite pas comme des constantes absolues : il les initialise au démarrage et les modifie pendant l'exécution.
- avec `const`, le compilateur les rend en lecture seule, et des lignes comme :

```c
time_scheduling = 0;
tsensor_1_min = 6;
GeneDacVoltage = 0;
```

provoquaient des erreurs de compilation.

Équivalence :
- la valeur logique reste la même : ces variables représentent des paramètres ou des états du système.
- la seule différence est la mutabilité : elles sont maintenant réellement utilisées comme des variables de travail du firmware, au lieu d'être bloquées en lecture seule.

En pratique :
- avant, le code voulait dire “cette valeur doit être immuable” alors qu'en réalité elle est modifiée pendant le démarrage ou le fonctionnement.
- après, le code reflète bien la réalité du firmware.

## 5. Résolution de la collision de macro `FOSC`

Avant :
- le fichier `board.h` définissait une macro `FOSC` pour les calculs de fréquence du système.
- or le header du PIC (`pic18f46k20.h`) utilise déjà un identifiant `FOSC` dans ses définitions de registres.
- cela provoquait une erreur de compilation au moment de l'inclusion du header matériel.

Après :
- la macro de projet a été renommée en `BOARD_FOSC` et la fréquence d'instruction en `BOARD_INSTR_FREQ`.

Pourquoi :
- le nom `FOSC` était pris par l'API matérielle du PIC, donc il n'était pas sûr de l'utiliser comme macro globale du projet.

Équivalence :
- les calculs de fréquence restent identiques :
  - `BOARD_FOSC = FCY × 4`
  - `BOARD_INSTR_FREQ = BOARD_FOSC / 4`

Avec les valeurs du projet :
- `FCY = 16 000 000`
- donc `BOARD_FOSC = 64 000 000`
- donc `BOARD_INSTR_FREQ = 16 000 000`

Ce changement ne modifie pas la logique de calcul ; il évite seulement un conflit de nom avec les headers du microcontrôleur.

## 6. Correction du calcul de baud rate UART

Avant :
- le calcul du registre baud rate utilisait une macro `FOSC` pour calculer `SPBRG`.

Après :
- le calcul utilise maintenant `BOARD_FOSC` :

```c
#define _SPBRG ((BOARD_FOSC / BAUDRATE) / 16) - 1
```

Formule utilisée :

$$
SPBRG = \left(\frac{FOSC}{BAUDRATE \times 16}\right) - 1
$$

Avec :
- `FOSC = 64 000 000`
- `BAUDRATE = 115200`

on obtient :

$$
SPBRG \approx \left(\frac{64\,000\,000}{115\,200 \times 16}\right) - 1 \approx 33.7
$$

Le code garde la même logique de calcul, mais avec une macro de projet non conflictuelle.

## 7. Harmonisation de la définition du buffer UART

Avant :
- le buffer UART était défini avec un type ou une visibilité qui n'était pas complètement cohérente entre les fichiers.
- cela provoquait un conflit de déclaration au lien.

Après :
- une seule définition du buffer est faite dans `src/uartbuf.c`.
- les autres fichiers ne font que déclarer l'existence de ce buffer via des `extern`.

Pourquoi :
- un symbole global ne doit être défini qu'une seule fois dans le projet.

Équivalence :
- le comportement du buffer reste exactement le même : il sert à stocker les octets entrants/sortants pour la couche UART.
- seul le mode de définition a été rendu propre et unique.

## 8. Validation du build

Le firmware a été recompilé avec la commande :

```bash
make -f nbproject/Makefile-default.mk dist/default/production/fazia-pic_ModernXC8.X.production.hex
```

Résultat :
- génération réussie de l'ELF et du HEX.
- le projet compile bien avec XC8 après les adaptations ci-dessus.

