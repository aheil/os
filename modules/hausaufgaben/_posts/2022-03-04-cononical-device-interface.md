---
title: Programmierübung Canonical Device Interface
---

# Programmierübung Canonical Device Interface

In dieser Aufgabe implementieren Sie den Treiber für das Interfaces eines Canonical Devices, wie es in der Vorlesung vorgestellt wurde.

Ihr Gerät verfügt über die drei Register Status, Command und Data.

![](../../../img/canonical-device-interface.png)

## Voraussetzungen

Nutzen Sie hierfür [gcc](https://gcc.gnu.org/).

Sie können *gcc* unter Linux und/oder macOS direkt verwenden. Unter Linux installieren Sie *gcc* über Ihren Paket-Manager, unter macOX über [Homebrew](https://brew.sh/).

Nutzen Sie Windows 10 oder 11, können Sie entweder Linux in einer virtuellen Maschine (z.B. in [VirtualBox](https://www.virtualbox.org/)) oder direkt das [Windows Subsystem for Linux (WSL 2.0)](https://docs.microsoft.com/en-us/windows/wsl/install-win10) nutzen. Alternativ ist es möglich *Windows Subsystem for Linux* über den Windows App Store zu installieren.

In der Wahl der Linux Distribution sind sie frei, alle Beispiele in der Vorlesung werden jedoch unter Ubuntu (letzter stabiler LTS Release) bewertet und eventuell vorgestellt. 

## Aufgabenstellung 

### Teilaufgabe 1: Interface Implementierung

Realisieren Sie das Interface über Felder.

Das Feld status kann folgende Werte besitzen:

- _busy_: 01
- _ready_: 00
- _error_: 02

Das Feld command kann folgende Werte besitzen:

- _idle_: 00
- _read_: 01
- _write_: 02
- _reset_: 04
- _delete_: 08

Die Felder _status_ als auch _command_ sind jeweils ein Byte groß.

Das Feld _data_ hat eine Länge von 20 Byte.

### Teilaufgabe 2: Treiber Implementierung
- Wird von Extern das Command-Register auf write gesetzt, liest der Treiber die Daten aus dem Data-Register (diese wurden zuvor ins Data-Register geschrieben) und schreibt diese in den internen Speicher
- Der interne Speicher in Ihrem Gerät soll 1.024 Byte betragen.
- Die Daten werden in Ihrem fiktiven Gerät fortlaufend in den internen Speicher geschrieben. - Ist der Speicher voll wird der Status auf _error_ gesetzt.
- Wird von Extern das Command-Register auf _read_ gesetzt, liefert der Treiber die Daten. _ - Die Daten werden dabei fortlaufend geschrieben.
- Ihr Gerät ist recht simpel aufgebaut und liefert einfach immer alle Daten, die vorhanden sind, beginnend mit dem ersten Byte (also bei Adresse 0).
- Wird zwischen Lese- und Schreibvorgang gewechselt, startet der Lese- bzw. Schreibvorgang immer am Anfang Ihrer internen Datenstruktur (also bei Adresse 0).
- Wird das Command-Register auf reset gesetzt, startet sowohl Lese- als auch Schreibvorgang wieder bei der Adresse 0.
- Wird von Extern das Command-Register auf _delete_ gesetzt, löscht das Gerät sämtliche gespeicherte Daten.
- Von extern können erst Daten in das Data-Register geschrieben werden, wenn das Command-Register auf _ready_ steht. 

### Teilaufgabe 3: Aufrufer Implementieren

- Implementieren Sie eine Main-Funktion (in einer gesonderten Datei), die in Ihr Gerät 512 Byte Daten schreib, danach wird der gesamte Speicherinhalt aus dem Gerät ausgelesen und ausgegeben werden.
- Setzen Sie nun das Gerät zurück und und schreiben Sie nun 2048 Byte in Ihr Gerät, danach soll der Inhalt des Gerätes ausgelesen und ausgegeben werden.
- Löschen Sie nun den Inhalt des Gerätes und schreiben 256 Byte in Ihre Gerät. danach wird der gesamte Speicherinhalt aus dem Gerät ausgelesen und ausgegeben werden.
- Dies entspricht dem Test Ihres Interfaces

## Abgabe

Die Bewertung Ihrer Abgabe findet automatisch statt. Stellen Sie hierzu folgende Punkte sicher:

* Ihre Implementierung befindet sich in einer Datei mit dem Namen **driver.c**. 
* Zur Abgabe erhalten Sie einen Zugang zum hochschulinternen [GitLab](https://git.it.hs-heilbronn.de/).
* Ihre Lösung checken Sie in Ihrem Repository im Ordner `aufgabe3` ein.
* Die eigentliche Abgabe erfolgt über das hochschuleigene [Commit-System](https://commit.it.hs-heilbronn.de/). Der Zugriff ist ausschließlich im Hochschulnetz oder über VPN möglich. 

## Bewertung

* Die Bewertung Ihrer Aufgabe findet anhand einer Reihe von automatisierten Tests statt. 
* Auf Basis Ihres Codes wird ein Test gestartet, der das fiktive Gerät entsprechend dem oben beschriebenen Test nutzt.
* Ihr Implementierung wird einer Reihe weiterer automatisierten Tests unterzogen, die Ihre Implementierung auf Korrektheit überprüfen.
* Abgaben, die nicht vollständig sind oder die Abgabekriterien nicht erfüllen werden nicht bewertet. 
* Abgaben, die nicht fristgerecht eingereicht werden, werden nicht bewertet. 
* Nutzen Sie zur Abgabe ausschließlich das beschriebene Verfahren. Abgaben, die per E-Mail oder anderen Wegen eingereicht werden, werden nicht bewertet. 
* Abgaben, die aufgrund eines Fehlers nicht durch die Tests laufen, werden entsprechend mit weniger Punkten bewertet.

## Hinweise 

- Ihre Datei darf keine main()-Funktion enthalten.
- Um wie angefordert eine main()-Funktion zu verwenden, muss diese in einer zweiten Datei sein. In dieser Aufgabenstellung müssen Sie die Datei auf jeden Fall in Git einchecken, da diese Teil der Aufgabenstellung ist.