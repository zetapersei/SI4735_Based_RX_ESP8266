# SI4735 Based RX ESP8266

Progetto di ricevitore radio FM/AM/SSB basato su ESP8266 e chip SI4735/SI4732, sviluppato con PlatformIO e libreria PU2CLR SI4735.

Questo repository contiene due principali sketch Arduino/ESP8266:

- `AM_FM_SERIAL_MONITOR.ino`: versione semplificata e minimale per test e verifica del ricevitore via monitor seriale.
- `src/OLED_ALL_IN_ONE.ino`: radio completa con display OLED, encoder rotativo, gestione EEPROM, band switching, SSB/AM/FM e comandi via menu.

## Obiettivo del progetto

L'obiettivo è fornire un ricevitore radio a bordo di ESP8266 capace di:

- ricevere segnali FM commerciali;
- sintonizzare AM (LW/MW/SW);
- gestire SSB (LSB/USB);
- controllare volume, filtro di banda, AGC, BFO, step di frequenza e ricerca stazioni;
- salvare lo stato del ricevitore in EEPROM per ripristinare la configurazione all'avvio;
- mostrare informazioni sulla frequenza, livello RSSI e stato del demodulatore su display OLED.

## Caratteristiche principali

### Radio

- supporto a chip `SI4735` e `SI4732`;
- ricezione FM, AM, SSB;
- ricerca automatica stazioni;
- gestione bande multiple: FM, MW, SW, LW;
- supporto a filtri AM/SSB/FM con configurazione dinamica;
- controllo del BFO per ricezione SSB;
- controllo del volume e dei parametri dell'AGC.

### Hardware

- microcontrollore: ESP8266 (ESP12E/ESP12F consigliato);
- radio front-end: SI4735/SI4732;
- display: SSD1306 OLED 128x32;
- encoder rotativo con pulsante;
- collegamenti I2C standard per chip SI4735 e display.

### Software

- PlatformIO come ambiente di build;
- librerie integrate:
  - `PU2CLR SI4735` per la gestione del ricevitore;
  - `Adafruit SSD1306` per il display;
  - `Rotary` per l'encoder;
  - `EEPROM` per la persistenza locale.

## Schema di collegamento

### Progetto hardware KiCad

Nella cartella `hardware/` è presente il progetto KiCad `schematic_esp8266_basic.*`, che rappresenta un riferimento schematico dell'assemblaggio base ESP8266 + SI4735/SI4732 + OLED + encoder.

Il design è coerente con il firmware qui presente:

- il modulo radio è collegato via bus I2C;
- `GPIO2` viene usato come linea di reset del chip SI4735/SI4732;
- `GPIO4` e `GPIO5` sono utilizzati come `SDA` e `SCL` del bus I2C;
- `GPIO12`, `GPIO13`, `GPIO14` sono dedicati al pulsante/encoder rotativo dell'interfaccia utente;
- il layout osserva il classico schema di riferimento del progetto PU2CLR.

### ESP8266 + SI4735/SI4732

| Chip SI4735/SI4732 | Funzione | ESP8266 |
|---|---|---|
| pin 15 / pin 9 | RESET | GPIO2 |
| pin 18 / pin 12 | SDIO | GPIO4 (SDA) |
| pin 17 / pin 11 | SCLK | GPIO5 (SCL) |

### Encoder + OLED

| Componente | GPIO |
|---|---|
| Encoder A | GPIO13 |
| Encoder B | GPIO14 |
| Pulsante encoder | GPIO12 |
| OLED SDA | GPIO4 |
| OLED SCL | GPIO5 |

> Nota: gli schemi KiCad sono un riferimento architetturale del progetto; la configurazione software è l'elemento di verità per i pin definiti nel firmware e va verificata con le specifiche del modulo radio utilizzato.

## File del progetto

### `AM_FM_SERIAL_MONITOR.ino`

Sketch minimale per verificare rapidamente il corretto funzionamento del modulo SI4735 via seriale. È utile per test iniziali, senza necessità di display o pulsanti.

Funzioni principali:

- configurazione del modulo radio;
- sintonizzazione FM/AM;
- cambio di banda e di larghezza filtro;
- seek in su/giù;
- controllo volume;
- visualizzazione del segnale corrente tramite monitor seriale.

### `src/OLED_ALL_IN_ONE.ino`

Versione completa e avanzata del ricevitore. Include:

- menu di comando su display OLED;
- gestione delle bande;
- memorizzazione dello stato in EEPROM;
- impostazioni AGC/attenuazione;
- selezione step di frequenza;
- filtro AM/SSB/FM;
- ricerca automatica delle stazioni;
- ricezione SSB con BFO.

## Requisiti

- PlatformIO Core;
- ESP8266 supportato;
- libreria `PU2CLR SI4735` scaricata automaticamente tramite `lib_deps`;
- Adafruit SSD1306 per l'OLED;
- una scheda ESP8266 con modulo SI4735/SI4732 collegato correttamente.

## Configurazione iniziale

1. Aprire la cartella del progetto in PlatformIO.
2. Verificare che la board sia configurata come `esp12e` in `platformio.ini`.
3. Eseguire il download delle librerie dipendenti.
4. Collegare i pin secondo lo schema riportato.
5. Compilare ed eseguire il caricamento del firmware su ESP8266.

## Uso rapido

### Sketch seriale

- `F`: sintonizzazione FM
- `A`: sintonizzazione AM/MW
- `L`: modalità LW
- `1`: banda SW
- `U` / `D`: aumenta/diminuisce frequenza
- `S` / `s`: ricerca stazione in su/giù
- `+` / `-`: volume su/giù
- `B`: cambia larghezza filtro
- `0`: mostra stato
- `?`: visualizza help

### Radio con OLED

La versione completa usa encoder e pulsante per:

- scegliere la banda;
- cambiare la frequenza;
- entrare nel menu di controllo;
- variare volume, step, AGC, BFO, filtro e soft mute;
- eseguire la ricerca automatica stazioni.

## Struttura del repository

```text
.
├── AM_FM_SERIAL_MONITOR.ino
├── README.md
├── platformio.ini
├── include/
├── lib/
├── src/
│   └── OLED_ALL_IN_ONE.ino
├── hardware/
├── test/
└── .pio/
```

## Verifica hardware KiCad

Il file `hardware/schematic_esp8266_basic.kicad_sch` è di tipo "schematic imported" e non rappresenta un progetto PCB totalmente customizzato per l'interfaccia radio completa; tuttavia i simboli importati confermano le connessioni essenziali usate dal firmware:

- `ESP8266` symbol: `GPIO2`, `GPIO4`, `GPIO5`, `GPIO12`, `GPIO13`, `GPIO14` sono presenti come pin di uso standard del modulo;
- `SI4735-D60-GU` symbol: i pin `SCLK` e `SDIO` sono quelli previsti dallo schema di riferimento del chip;
- la configurazione software coincide quindi con il mapping hardware di base del progetto.

Da un punto di vista di progettazione, il file è valido come schema concettuale, ma prima di produrre una PCB definitiva va verificato l'ecosistema completo: 

- decoupling e filtraggio alimentazione del modulo SI4735;
- massa condivisa pulita tra ESP8266, radio e display;
- antenna / percorso RF e matching di input, se usato un'antenna esterna o la rete di sintonia del modulo;
- considerazioni EMI/EMC per il bus I2C e la zona RF.

## Note importanti

- Per un funzionamento affidabile, verificare le connessioni I2C e i livelli di tensione del modulo radio.
- Nel file `src/OLED_ALL_IN_ONE.ino` è presente la gestione EEPROM: per resettare i dati salvati, premere il pulsante dell'encoder all'avvio.
- La radio SI4735 è molto sensibile alle connessioni e al layout PCB; una buona massa e un cablaggio pulito sono fondamentali.
- I parametri di banda e larghezza filtro possono essere adattati in base alla propria area di ricezione e alle esigenze personali.

## Troubleshooting

- Nessun segnale ricevuto: verificare alimentazione, reset e connessioni I2C.
- Frequenza immobile: controllare il pin di reset e la configurazione del device.
- OLED vuota: verificare l'indirizzo 0x3C e la corretta alimentazione del display.
- EEPROM non inizializzata: utilizzare il reset manuale con pulsante encoder premuto all'avvio.

## Ringraziamenti

- Libreria e documentazione: `PU2CLR SI4735`;
- supporto hardware e esempi da cui il progetto trae ispirazione.

## Licenza

Questo repository è distribuito per uso didattico e sperimentale. Verificare le licenze delle librerie dipendenti prima di uso commerciale o distribuzione pubblica.
