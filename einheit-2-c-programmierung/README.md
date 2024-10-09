# Einheit 2: C Programmierung

C ist eine Programmiersprache, die in der Betriebssystemprogrammierung aus verschiedenen Gründen weit verbreitet ist:

**Hardwarezugriff**: In C kann **Hardware direkt angesprochen** werden. Dies ist für Treiberentwicklung und Systemprogrammierung notwendig.

**Effizienz**: Mit C lässt sich effizient entwickeln. Es ermöglicht die **direkte Steuerung von Hardware** und bietet eine lässt sich leicht in Assemblercode umsetzen. Diese Effizienz ist relevant für Betriebssysteme, die mit beschränkten Ressourcen arbeiten.

**Portabilität**: C ist portabel und kann mit relativ wenig Aufwand **auf verschiedenen Plattformen eingesetzt** werden. Dies ist wichtig für Betriebssysteme, die unterschiedlichen Architekturen adressieren.

Warum sind sog. managed Sprachen wie Java oder .NET nicht für die Betriebssystemprogrammierung geeignet?

**Bootstrapping**: Betriebssysteme müssen sich **selbst initialisieren**. Die Verwendung von Managed Sprachen für diesen Zweck ist problematisch, da sie auf eine bereits laufende Laufzeitumgebung angewiesen sind.

**Abhängigkeit von Ressourcen**: Betriebssysteme müssen auf **unterster Ebene** mit Hardware-Ressourcen interagieren. Managed Sprachen sind abstrahiert und verbergen viele Details. Dies kann zu Problemen bei der direkten Steuerung von Hardware führen.

**Vorhersagbarkeit**: Betriebssysteme müssen **deterministisch** und **vorhersagbar** sein. Managed Sprachen haben z.B. eine **Garbage Collection** und andere automatisierte Prozesse, die schwer vorhersehbar sind und damit die V**orhersagbarkeit** beeinträchtigen können.

**Performance**: Managed Sprachen wie **Java** oder .NET führen aufgrund ihrer **Laufzeitumgebung** (z. B. der **Java Virtual Machine** oder des **.NET Common Language Runtime**) eine zusätzliche Schicht und somit zusätzlichen Overhead ein. Das wirkt sich negativ auf die **Performance** aus und ist in Betriebssystemen, die eigentlich auf **Effizienz** angewiesen sind, problematisch.

## Wie funktioniert Java (Wdhl.)?

* Der Java Compiler (`javac`) erzeugt keinen maschinenlesbaren code, sondern eine Zwischendarstellung, den sog. **Bytecode**.
* Wird das Java Programm gestartet (`java`), startet der **Java-Interpreter**, der den Bytecode Zeile für Zeile ausführt.
* Während das Programm ausgeführt wird, analysiert der **JIT-Kompiler** fortlaufend das Programm und entscheidet, ob es vorteilhaft ist, bestimmte Teile des Codes in Maschinencode zu übersetzen. Der Maschinencode ist dann plattformspezifisch.

## Wie funktioniert C?

Der C-Compiler wandelt den mit C programmierten Code in Maschinensprache um, der vom Zielsystem direkt ausgeführt werden kann. Dieser Prozess umfasst mehrere Phasen:

* **Präprozessor**: Der **Präprozessor bereitet den Code** vor, er ersetzt Makros, fügt die Include-Dateien ein, stellt sicher, dass jeder Zeile durch ein Newline-Zeichen getrennt ist und entfernt Kommentare. Das Ergebnis ist eine überarbeitete Code-Datei.
* **Analysephase** (Frontend-Phase): Hier wird der Sourcecode auf korrekte Syntax geprüft, dabei werden bereits Optimierungen in der Syntax vorgenommen. Der Compiler erzeugt hier einen Zwischencode, der noch nicht maschinenlesbar ist.
* **Code-Generierung** (Backend-Phase): Der Zwischencode wird in Assemblercode übersetzt und maschinenspezifische Optimierungen vorgenommen.
* **Linker-Phase**: Der sog. Linker vervollständigt den Code, liest die erforderlichen Bibliotheken ein und setzt alle erforderlichen Adressen ein (hierzu in einer späteren Einheit mehr). Die Ausgabe des Linkers ist der ausführbare Maschinencode.

Beispiel für ein Makro:

```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))

...

int result = MAX(x, y);
```

Nach der Präprozessorphase sähe der Code dann so aus:

```c
int result = ((a) > (b) ? (a) : (b));
```

Nach der Code-Generieung sähe der Code dann so aus:

```assembly
; Annahme: a und b sind bereits in Registern geladen
CMP a, b
; Wenn a größer als b ist, springe zu L1, sonst setze result auf b
JG L1
MOV result, b
JMP L2
L1:
MOV result, a
L2:
```

Erklärung zu den verwendeten Assembler-Befehlen:

`CMP` = Vergleich\
`JG`= Springe, wenn größer\
`MOV` =Verschieben

Der Maschinencode wird nun in ein betriebssystemspezifisches Dateiformat verpackt, dass vom Betriebssystem ausgeführt werden kann (s. [Hausaufgaben](./#hausaufgabe)).

## Aufgabe:

* Installieren Sie auf Ihrem System `gcc`
  * Linux- und MacOS-Nutzer stellen sicher, dass gcc auf Ihrem System läuft
  * Windows Nutzer installieren hierfür Windows Subsystem for Linux 2.0 (via Windows Store) und dort die Distribution Ubuntu.
  * Bei der Installation von Ubuntu müssen Sie einen User anlegen
    * Nur Kleinbuchstaben!&#x20;
    * Tipp: Einfaches Passwort ;-)
  * Stellen Sie sicher, dass `gcc` installiert ist
  * Testen Sie, dass gcc installiert ist

```
> gcc 
> gcc: fatal error: no input files 
> compilation terminated.
```

* Befehle auf der Kommandozeile
  * ls -> analog zu dir&#x20;
  * cd .. -> Verzeichnis hoch
  * cd / -> auf das Root-Verzeichnis wechseln
* WSL macht den C-Ordner verfügbar unter `/mnt/c`

Installation von `gcc`auf der Kommandozeile

* `sudo apt-get update`
* `sudo apt-get install gcc`





* Erstellen Sie eine Datei `helloworld.c`
  * Fügen Sie folgende Inhalt in der Datei ein

```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
```

* Kompilieren Sie die Datei mit

<pre class="language-bash"><code class="lang-bash"><strong>gcc hello_world.c
</strong></code></pre>

Starten Sie das Progamm mit

```
./a.out 
```

##

## Linux Terminal&#x20;

Einführungskurs in die Konosle unter Linux (Ubunt) gibt es hier:\
Durcharbeiten (ca. 1-1,5h), nicht nur lesen!

{% embed url="https://ubuntu.com/tutorials/command-line-for-beginners#1-overview" %}



## Weiterführende Links

* Phasen bei der Kompilierung von C-Code: [https://abhishek-anand.medium.com/compilation-process-of-a-c-program-f06c22e91fd4](https://abhishek-anand.medium.com/compilation-process-of-a-c-program-f06c22e91fd4)
* C-Crshkurs: [https://github.com/aheil/hhn-c](https://github.com/aheil/hhn-c)\\
* Command Line for Beginners: [https://ubuntu.com/tutorials/command-line-for-beginners#1-overview](https://ubuntu.com/tutorials/command-line-for-beginners#1-overview)
