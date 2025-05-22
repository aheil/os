---
description: E - N - T - W - U - R- F
---

# Semaphore Beispiel

Beispiel 1: Mensa Kasse

Zur Regelung des Ansturms an der Mensa Kasse, regeln wir den Zugang zur Kasse mittels einer Semaphore. Die Semaphore wird mit dem Wert 1 initialisiert (= 1 Kasse steht zur verfügung).

* Jeder Studierende, der sich anstellt verringert dadurch den Zähler der Semaphore um 1.&#x20;
* Nur wenn die Semaphore mind. 0 ist, darf der nächste Studierende bezahlen.

