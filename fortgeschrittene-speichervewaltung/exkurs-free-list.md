# Exkurs: Free List

* Bisher galt die Annahme, das alle Adressräume gleich sind
* Somit muss man »eigentlich nur« die nächste freie Lücke finden und »auffüllen«
* Segmentierung führt nun jedoch zur Fragmentierung (engl. external fragmentation)
* Innerhalb eines Adressraus spricht man ebenfalls von Fragmentierung (engl. internal fragmentation)

Beispiel: Heap

\


<figure><img src="../.gitbook/assets/os.11.heap.de.png" alt=""><figcaption></figcaption></figure>

* Eine Datenstruktur zur Verwaltung freier Speicherbereiche ist die sog. Free List
* Basiert auf einer verketteten Liste (engl. linked list)

<figure><img src="../.gitbook/assets/os.11.freelist.de.png" alt=""><figcaption></figcaption></figure>

## Free List Beispiele![](../.gitbook/assets/os.11.freelist_example.de.png)

* Anfragen für größer 10 Bytes schlagen fehl (liefert NULL)
* Exakt 10 Bytes kann durch einen der beiden Blöcke bedient werden
* Aber was passiert, wenn nur 1 Byte angefordert wird?

<figure><img src="../.gitbook/assets/os.11.freelist_example.de (1).png" alt=""><figcaption></figcaption></figure>

* Einer der freien Blöcke wird verkleinert…\


<figure><img src="../.gitbook/assets/os.11.freelist_example_2.de.png" alt=""><figcaption></figcaption></figure>

* Zurück zur Ausgangssituation mit drei Blöcken…
* Was passiert wenn der mittlere Block freigegeben wird?
* Es entstehen drei Blöcke… keine Gute Idee…\


<figure><img src="../.gitbook/assets/os.11.freelist_example_3.de.png" alt=""><figcaption></figcaption></figure>

* Daher fasst die zuständige Bibliothek den freien Speicher vor dem Allokieren so gut wie möglich zusammen\


<figure><img src="../.gitbook/assets/os.11.freelist_example_4.de.png" alt=""><figcaption></figcaption></figure>
