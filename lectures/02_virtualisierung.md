<!--

author:   Andreas Heil

email:    andreas.heil@hs-heilbronn.de

version:  0.1

language: de

narrator: DE German Male

tags: betriebssysteme, lecture, virtualisierung

comment:  

-->

# Virtualisierung

<!-- data-type="none" -->
| Parameter | Kursinformationen |
| --- | --- |
| **Veranstaltung:** | `262007 Betriebssysteme`|
| **Semester** | `SEB2` |
| **Hochschule:** | `Hochschule Heilbronn` |
| **Inhalte:** | `Virtualisierung` |
| Startseite | [https://liascript.github.io/course/?https://raw.githubusercontent.com/aheil/os/master/README.md#1](https://liascript.github.io/course/?https://raw.githubusercontent.com/aheil/os/master/README.md#1) | 
| **Link auf den GitHub:** | [https://github.com/aheil/os/blob/main/lectures/02_virtualisierung.md](https://github.com/aheil/os/blob/main/lectures/02_virtualisierung.md) |
| **Autoren** | @author |

## Lernziele und Kompetenzen 

* **Verstehen** wie sich Prozesse zusammensetzen und Prozesse vom Betriebssystem verwaltet werden.
* **Verstehen** wie Prozesse im Betriebssystem gesteuert werden

## Prozesse 

**Was ist eigentlich ein Prozess?**

Einfache Antwort: Ein ausgeführtes bzw. laufendes Programm

![](../img/os.02.taskmng1.png)

{{1}}
************************************

**Was ist dann überhaupt ein Programm?**

  * Besteht aus Code (Bits) und ggf. statischen Daten
  * Wartet auf der Festplatte und tut nichts
  * Erst durch die Ausführung wird ein Programm zum Prozess

**Was benötigt ein Programm?**

  * Benötigt zur Ausführung eine CPU 
  * Benötigt für den auszuführenden Code und die Daten Speicher 

************************************

{{2}}
************************************

**Frage:** Wie kann die Illusion vieler CPUs geschaffen werden, wenn es nur eine (oder wenige) physikalische CPUs gibt?

Beispiel: Rechts Windows Task Manager mit 262 Prozesse 

![](../img/os.02.taskmng2.png)

Beispiel: Linux Befehl *top*

![](../img/os.02.top.png)

************************************

{{3}}
************************************

**Abstraktion von Prozessen**

Prozesse bestehen grundlegend aus

* Speicher, in dem die Programmanweisungen bzw. Instruktionen (engl. instructions) liegen
* Speicher, in dem die Daten geschrieben werden 
* Vom Prozess adressierbarer Speicher (engl. address space)
* Registern - Instruktionen lesen und schreiben in Register, dies ist notwendig für die Ausführung d. Prozesses

> Diese Informationen können jederzeit »weggespeichert« und wiederhergestellt werden

************************************

{{4}}
************************************

**Spezielle Register, die benötigt werden**

- Program Counter (Abk. PC) oder auch Instruction Counter (Abk. IC), hier steht die nächste Anweisung, die ausgeführt werden soll
- Stack Pointer, Frame Pointer, Funktionsparameter, lokale Variablen und Rücksprungadressen (engl. return address) - mehr dazu später
- Register für I/O-Informationen
- Liste der Dateien, die der Prozess aktuell geöffnet hat  

************************************

### Virtualisierung

»Sehr vereinfacht«:

* Wir geben jedem Prozess die CPU für eine kurze Zeitspanne 
* Dieses sog. »Timesharing« erzeugt eine Illusion mehrerer CPUs
* Konsequenz: Programm läuft langsamer, da die CPU »geteilt« wird 

{{1}}
************************************

**Was wird für Virtualisierung benötigt?**

»Low Level Machinery« 

  * Methoden und Protokolle für die grundlegende Funktionalität 

»High Level Intelligence«

  * Irgendetwas Geschicktes zum Stoppen und Starten von Programmen 
  * Zusätzliches Regelwerk (engl. policies)
  * Regeln wie viele Prozesse auf einer CPU ausgeführt werden dürfen
  * Jemand oder etwas, der bzw. das steuert, welcher Prozess als nächstes ausgeführt wird

************************************

{{2}}
************************************

**Prozess-API** 

Außerdem benötigen wir eine Programmierschnittstelle (engl. process api), die jedes Betriebssystem beinhalten muss (wird später noch weiter vertieft).

* `create`: Ausgewähltes Programm wird gestartet und ein neuer Prozess erzeugt 
* `destroy`: Falls sich ein Programm nicht von selbst beendet, ist dies sehr hilfreich
* `wait`: Durchaus sinnvoll zu warten, bis ein Prozess von selbst aufhört zu laufen
* `status`: Statusinformation von Prozessen abfragen 

Weitere Möglichkeiten sind je nach Betriebssystem unterschiedlich, z.B.:
`suspend` und `resume` um Prozesse anzuhalten und weiterlaufen zu lassen

************************************

{{3}}
************************************

**Wie wird ein Prozess erzeugt?**

1. Voraussetzung: Ein Programm muss in ausführbarer Form vorliegen (mehr dazu später)
2. Programm und statische Daten werden in den Adressraum des Prozesses geladen

  - »Früher« wurde das gesamte Programm in den Speicher geladen (engl. eagerly)
  - »Heute« wird nur der benötigte Programm-Code und die erforderlichen Daten geladen (engl. lazy)  

  Um dieses sog. »Lazy Loading« zu verstehen, werden wir uns später noch mit »Paging« und »Swapping« befassen müssen.

3. Der sog. »Stack« bzw. »Runtime Stack« wird zugewiesen

  * C nutzt den Stack für lokale Variablen, Funktionsparameter und Rücksprungadressen

4. Das Betriebssystem füllt z.B. die Parameterlisten
  
  * Bei C sind dies `argc` und `argv`, so dass das Programm (hier die `main`-Funktion) auf die Werte zugreifen kann[^4]
  * Kennen Sie auch aus Java

5. Nun wird noch der Heap reserviert 

    * In C für dynamischen Speicherzuordnung via `malloc()` und `free()`
    * Exkurs: Memoryleaks baut man übrigens, indem man in C vergisst `free()` aufzurufen

![](../img/os.01.processcreation.png)

6. Das Betriebssystem unterstütz nun den Prozess, indem es z.B. dem Prozess mehr Speicher gibt, wenn der Heap vergrößert werden muss

7. Nun  werden noch Input/Output-Resourcen erzeugt (sie ahnen es, später mehr dazu)

  * Unter UNIX sind dies die drei sog. »File Descriptors« (https://sites.ualberta.ca/dept/chemeng/AIX-43/share/man/info/C/a_doc_lib/aixuser/usrosdev/std_input_output.htm)

    * Standard Input, 
    * Standard Output und 
    * Standard Error Output

************************************

{{4}}
************************************

**Prozess Status**

Was bedeuten eigentlich die Status...?

* Laufend
* Schlafend 
* Gestoppt
* Zombie

> Tasks shown as running should be more properly thought of as 'ready to run' -- their task_struct is simply represented on the Linux run-queue. Even without a true SMP machine, you may see numerous tasks in this state depending on top's delay interval and nice value.

Quelle: https://man7.org/linux/man-pages/man1/top.1.html

************************************

**Mögliche Statusübergänge**

* **Running:** Prozess läuft auf einer CPU 
* **Ready:** Prozess könnte laufen, aber das OS hat entschieden, den Prozess noch nicht laufen zu lassen
* **Blocked:** Prozess hat eine Aktion ausgeführt, die erst abgeschlossen werden kann, wenn ein anderes Ereignis stattgefunden hat - typischerweise handelt es sich hierbei um eine I/O-Operation

> Ist ein Prozess geblockt, wartet das Betriebssystem auf die I/O-Operation, um dann den Prozess wieder in den Status *Ready* zu verschieben. 

 ![](../img/os.01.status.png)

 {{5}}
 ************************************
 
**Abschließende Gedanken zu Prozessen**

Wer entscheidet eigentlich welcher Prozess als nächster gestartet wird?

- Der sog. »Scheduler« trifft diese Entscheidung (später mehr dazu)
- Bevor wir uns den Scheduler anschauen, müssen wir uns allerdings noch ein paar weitere Gedanken über Prozesse machen… 

Wir benötigen dafür 

- Eine Datenstruktur für Prozesse 
- Eine Liste aller Prozesse
- Eine Liste aller blockierten Prozesse
- Eine Möglichkeit,Register bei Stoppen weg zu speichern und beim Anlaufen des Prozesses wieder zu laden (engl. context switch)

> Und was passiert eigentlich, wenn ein Prozess beendet ist, aber noch nicht alles »aufgeräumt« wurde? In UNIX-Systemen haben solche Prozesse einen eigenen Status: **Zombie** 

 ************************************

### Exkurs: Datenstruktur von xv6-Prozessen

Alle Informationen über einen Prozess stehen in einem Prozesskontrollblock (engl. process control block, kurz PCB) 

![](../img/os.02.pcb.png)


### Zusammenfassung

* Prozesse sind die grundlegende Abstraktion eines Programmes
* Zu jedem Zeitpunkt kann ein Prozess über seinen Status, den Speicherinhalt, seinen Adressraums, den Inhalt der CPU-Register (einschl. program counter und stack pointer) und den I/O-Informationen (d.h. geöffnete Dateien) beschrieben werden
* Die Prozess-API besteht aus Aufrufen, die in Zusammenhang mit Prozessen ausgeführt werden können, z.B. zum Erzeugen oder Beenden von Prozessen
* Unterschiedliche Ereignisse führen zu Statusänderungen im Prozess (z.B. der Aufruf einer blockierenden I/O-Operation)
* Eine Prozessliste enthält alle Informationen über die Prozesse auf einem System

## Kontrollfragen

1. Welche Status von Prozessen gibt es?

- [[x]] Laufend
- [[ ]] Wartend
- [[x]] Schlafend
- [[x]] Gestoppt
- [[ ]] Vampir
- [[x]] Zombie

2. Was ist die Hauptfunktion von Virtualisierung von Prozessen?
- [( )] Umfassende Sicherheit bieten
- [(X)] Hardware-Ressourcen teilen
- [( )] Betriebssysteme simulieren
- [( )] Netzwerktopologien simulieren

### Weiterführende Informationen

- Windows-Prozesse mit PowerShell anzeigen, auslesen und beenden: https://www.scriptrunner.com/de/blog/windows-prozesse-mit-powershell-anzeigen-auslesen-und-beenden/ 
