# Exercise: Deadlock

## Aufgabe

Erstellen Sie ein **Java** Programm, das einen Deadlock provoziert:&#x20;

* Implementieren Sie eine Klasse für einen Sende-Empfänger.&#x20;
* Starten Sie für zwei Sende-Empfänger jeweils einen Thread.
* Empfange wird, indem die Sende-Methode in eine entsprechende Variable des anderen Empfängers schreibt\
  (Hinweis: Threads haben auf den gleichen Heap innerhalb des Prozesses Zugriff)
* Provozieren Sie nun einen Deadlock aufgrund folgenden Verhaltens: Ein Sender kann keine Nachricht empfangen, solange er senden möchte, bzw. empfängt ein Sender eine Nachricht, kann er keine Nachricht versenden.&#x20;
* Tipp: Nutzen Sie das `synchronized` Keyword in Java, um das gewünschte Verhalten zu provozieren.&#x20;

Checken Sie das Code-File in Ihrer Gruppe unter dem Ordner  **sender\_receiver** in Ihrem Repository ein.
