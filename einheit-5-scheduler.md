# Einheit 5: Scheduler

### Lernziele und Kompetenzen

* **Verstehen** wie Prozesse im Betriebssystem gesteuert werden
* **Verstehen** welche Probleme bei der direkten Ausführung von Prozessen auf der CPU entstehen und wie dem im Betriebssystem begegnet wird
* Grundlagen der Scheduling-Mechanismen **kennen lernen**
* **Verstehen** wie Prozesse von Betriebssystemen geschedult werden können

### Direct Execution

**Problem**

> Bisher haben wir gelernt, dass es Prozesse gibt, diese irgendwie gestartet werden können.

Das Betriebssystem lädt also ein Programm, dei Daten lädt alle Register und startet den Prozess...

**Annahme**: Der Prozess bekommt vollen Zugriff auf alle Ressourcen und läuft direkt auf der CPU, bis der Prozess die Kontrolle wieder an das Betriebssystem abgibt (engl. _direct execution)._

* **Frage 1:** Wie stellen wir sicher, dass der Prozess nichts »Verbotenes« tut?
* **Frage 2:** Die direkte Ausführung des Prozesses auf der CPU (engl. direct execution) ist zwar schnell, aber was passiert nun, wenn der Prozess eingeschränkte Aktionen durchführen will (z.B. mehr Speicher, I/O-Operation etc.)?
* **Frage 3:** Und wie stellen wir überhaupt sicher, dass der Prozess die Kontrolle wieder abgibt? Solange der Prozess ausgeführt wird, hat das Betriebssystem ja keine Kontrolle über die CPU... 🤔

**Lösungsidee**

Programme laufen im sog. **»User Mode Linux«** oder allgemein **»User Mode«**.

* Es wird eingeschränkt, was das Programm »tun« kann
* Z.b. werden I/O-Operationen eingeschränkt
* Wenn ein Programm versucht etwas unerlaubtes auszuführen wird eine »Exception« im Prozessor erzeugt (das heißt tatsächlich so, hat aber nichts z.B. mit Java Exceptions zu tun)

Der Gegensatz: **»Kernel Mode«**

* Hier sind alle Operationen, auch bzw. insbesondere I/O-Operationen erlaubt

SysCall

Wenn ein Programm im _User Mode_ etwas ausführen möchte, das eigentlich untersagt ist, führt es einen sog. »System Call« oder kurz »Syscall« aus.

* System Calls werden von allen modernen Betriebssystemen angeboten
* POSIX-Systeme (Portable Operating System Interface1) bieten mehrere hundert solcher System Calls an

## System Call&#x20;

Wenn ein Programm im _User Mode_ etwas ausführen möchte, das eigentlich untersagt ist, führt es einen sog. »System Call« oder kurz »Syscall« aus.

* System Calls werden von allen modernen Betriebssystemen angeboten
* POSIX-Systeme (Portable Operating System Interface1) bieten mehrere hundert solcher System Calls an

SysCall Ablauf\


Das Programm...

* Führt ein sog. Trap-Instruktion aus
* Springt in Kernel und startet im privilegierten Modus (Kernel Modus)
* Führt die Operationen aus, die im »System Call Handler« hinterlegt sind
* Führt eine sog. Return-From-Trap-Instruktion aus
* Kehrt in den User Mode zurück

**Vorsicht**

Die Hardware muss darauf achten „genügend Bestandteile vom Programm bestehen zu lassen“, so dass es später wieder ausgeführt werden kann.

Am Beispiel des x86:

Hier werden...

* Program Counter, Flags und weitere Register in einen sog. Per-Process-Kernel-Stack »gepusht« (Datenstruktur Stack klar? Ggf. Exkurs am Ende)
* Bei der Return-From-Trap-Instruktion werden diese wieder vom Stack geladen
* Danach kann das Programm wieder im User Mode ausgeführt werden

Dieses Vorgehen wird von Betriebssystem zu Betriebssystem zwar unterschiedlich gehandhabt, ist im Grundsatz aber immer ähnlich

**Nochmal Vorsicht**

**Frage:** Woher weiß das OS, welcher Code für System Calls ausgeführt werden soll?

Das Programm kann ja kein Speicherbereich angeben

Grundsätzlich wäre das auch eine sehr schlechte Idee… Das ist schon klar warum, oder?

**Lösung: Trap Table**&#x20;

* Es wird eine sog. »Trap Table« zur Boot-Zeit erstellt
* Beim Booten ist das System immer im Kernel Mode
* Das Betriebssystem kann der Hardware somit sagen, welcher Code bei welchem Ereignis ausgeführt wird
* Das Betriebssystem informiert die Hardware über diese sog. Trap Handlers oder System Call Handlers

> Nur mal so... Was könnte man denn machen, wenn man eine eigene Trap Table installieren könnte? 🤔



