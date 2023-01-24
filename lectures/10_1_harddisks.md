<!--

author:   Andreas Heil

email:    andreas.heil@hs-heilbronn.de

version:  0.1

language: de

narrator: DE German Male

tags: hard disks, hdd, lecture, i/o, input/output

comment:  

-->


# Hard Disks

<!-- data-type="none" -->
| Parameter | Kursinformationen |
| --- | --- |
| **Veranstaltung:** | `262007 Betriebssysteme`|
| **Semester** | `SEB2` |
| **Hochschule:** | `Hochschule Heilbronn` |
| **Inhalte:** | `Hard Disks|
| Startseite | [https://liascript.github.io/course/?https://raw.githubusercontent.com/aheil/os/master/README.md#1](https://liascript.github.io/course/?https://raw.githubusercontent.com/aheil/os/master/README.md#1) | 
| **Link auf den GitHub:** | [https://github.com/aheil/os/blob/main/lectures/10_1_harddisks.md](https://github.com/aheil/os/blob/main/lectures/10_1_harddisks.md) |
| **Autoren** | @author |

## Lernziele 

- **Verstehen** wie I/O Devices grundsätzlich aufgebaut sind und wie sich diese in das Betriebssystem integrieren

## Bus-Systeme

**Gedankenspiel**

- Was wäre ein Programm ohne Eingabe? Es lieferte immer die gleiche Antwort.

- Was wäre ein Programm ohne Ausgabe?  🤔

- Ein- undAusgabe stellt somit einen zentralen Aspekt von Rechnern dar.

- Wie lässt sich Ein- und Ausgabe in ein System integrieren?
- Was sind die grundlegenden Mechanismen ?
- Wie können diese effizient umgesetzt werden?
- Welche Aufgabe hat hierbei das Betriebssystem?

{{1}}
************************************

**Bus-Systeme**

Zunächst stellt sich die Frage, wie ein Gerät an den Rechner angebunden wird...

- Normalerweise über ein Bus

- Wir unterscheiden zwischen

  - Speicherbus zur schnellen Anbindung des Hauptspeichers
  - Einem allgemeinen I/O\-Bus zur systeminternen Kommunikation (bei modernen Geräten ist dies PCI)
  - Peripherie-Bus (z.B. SCSI, SATA oder USB)

- Warum aber brauchen wir mehrere solche Bus-Systeme?

- Grund ist die Physik, die Kosten sind hier die maßgeblichen Größen

  - Je schneller der Bus\, desto kürzer
  - Je schneller der Bus\, desto teurer

************************************

{{2}}
************************************

![](../img/os.10.io.de.png)

************************************

## Canonical Device

- Grundlegendes \(allgemeingültiges\) Konzept eines Gerätes

  - Besteht aus zwei wichtigen Komponenten:
  - Hardware Interface, über den das das Gerät angesteuert werden kann

- Was steckt alles in einem Gerät?

  - Stark Implementierungsabhängig
  - Ein paar Chips, komplexere Geräte sogar mit einer eigenen CPU
  - Allgemeiner Speicher und weitere Chips

{{1}}
************************************

![](../img/os.10.canonical_device.de.png)

************************************

### Canonical Protocol 

Benötigt wird ein allgemeingültiges Protokoll zur Ansteuerung von I/O-Geräten.

* Im Beispiel zuvor: 3 Register

  * Status Register: Ermöglicht es, den Status des Geräts auszulesen
  * Command Register: Ermöglicht es, dem Gerät mitzuteilen, welche Aktion als nächstes ausgeführt werden soll
  * Data Register: Ermöglicht es Daten ins Gerät zu übermitteln
  * Durch Schreiben/Lesen dieser Register wird die Interkation mit dem Gerät ermöglicht

{{1}}
************************************
**Das Protokoll in 4 Schritten**

1. Warten bis das Gerät bereit ist
2. Daten in Register schreiben
3. Kommando in Register schreiben
4. Warten bis Gerät fertig ist

```
while (STATUS == BUSY) ; // wait until device is not busy
write data to DATA register
write command to COMMAND register
(starts the device and executes the command)
while (STATUS == BUSY) ;
// wait until device is done with your request
```

************************************

{{2}}
************************************

**Polling**

 * Das Status Register fortwährend auszulesen wird auch __Polling__ genannt
  * Im Grund wird andauernd gefragt: „Ey Digga, was geht?!“
  * Abhängig von der Größe des Daten Registers sind hier mehrere Durchläufe erforderlich, bis alle Daten geschrieben sind

************************************

{{3}}
************************************

**PIO**

Kennt man vom Arduino oder Raspberry PI... aber was steckt dahinter? 

* Sobald die CPU (hier meinen wir die CPU vom Rechner, nicht vom I/O Gerät) für das "Hin- und Herschippern" der Daten genutzt wird, sprechen wir von _programmed I/O_ (Abk. PIO)

  * Das Caconical Protokoll funktioniert im Grunde ABER
  * Polling ist kostenintensiv

    * es verschwendet CPU Cycles
    * es Verlangsamt oder blockiert die Ausführung anderer Prozesse
    * es führt die Idee des Overlapping beim Scheduling ad absurdum

************************************

{{4}}
************************************

**Interrupts**

* Idee: Den CPU Overhead mittels Interrupts reduzieren
* Grundsätzliche Funktionsweise:

  * Betriebssystem stellt eine Anfrage an ein Gerät
  * Der aufrufende Prozess wird schlafen geschickt
  * Betriebssystem führt einen Kontext-Switch zu einem anderen Prozess aus
  * Sobald das Gerät fertig ist\, wird ein Hardware Interrupt ausgelöst
  * Der Interrupt veranlasst das Betriebssystem eine vordefinierten _Interrupt Service Routine_ (ISR) bzw. _Interrupt Handler_ auszuführen.

************************************

{{5}}
************************************

**Polling vs Interrupts**

In dem erste Beispiel pollt die CPU, bis das Gerät fertig ist.

Mit einem Interrupt könnte die CPU in der Zwischenzeit etwas anders (sinnvolles) machen.

![](../img/os.10.polling.de.png)

************************************

### Kommunikation mit dem Gerät

Nun stellt sich noch die Frage, wie die ganzen Geräte mit ihren spezifischen Hardware Interfaces in das Betriebssystem passen?

> Ziel: Betriebssystem so gut wie es geht geräteneutral halten\, also die Details der Geräteinteraktion vom Betriebssystem „verstecken“.

Lösung: Wie so oft in der Informatik hilft uns hier die _Abstraktion_!

![](../img/os.10.abstraction.de.png)

### Gerätetreiber

Die gerätespezifische Funktionalität wird als Gerätetreiber ausgeliefert.

Nachteil: Durch die generische Schnittstelle können nicht immer alle (tollen) Funktionen eines Geräts genutzt werden.

Beispiel: SCSI Error-Funktionalität ist unter Linux über die einfachere ATA/DIE Schnittstelle nicht nutzbar.

Bedeutung von Gerätetreibern:Bis zu 70% des Codes eines Betriebssystems (Linux und Windows annähernd gleich viel) steckt heute inzwischen in Gerätetreibern.

__Problem__ : Dieser Code wird nicht von Kernel-Entwicklern gebaut.


## Kontrollfragen