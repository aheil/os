# Einheit 1: Hausaufgabe

### Aufgaben

1. Stellen Sie sicher, dass Git auf Ihrem Rechner vorhanden ist
   1. MacOS: [https://git-scm.com/download/mac](https://git-scm.com/download/mac)
   2. Windows:  [https://git-scm.com/download/win](https://git-scm.com/download/win)
   3. Linux: [https://git-scm.com/download/linux](https://git-scm.com/download/linux)
2. Sofern Sie noch keinen Zugang um [Hochschul-GitLab](https://git.it.hs-heilbronn.de/) haben, melden Sie sich dort einmalig an. \
   **Hinweise**: Sie erhalten beim ersten Versuch eine Fehlermeldung, dadurch lern das System jedoch Ihren Account kennen und Sie können einem Repository zugewiesen werden.
3. Melden Sie sich im jeweiligen ILIAS-Raum ([SEB ](https://ilias.hs-heilbronn.de/ilias.php?baseClass=ilrepositorygui\&ref\_id=360705)bzw. [AIB](https://ilias.hs-heilbronn.de/ilias.php?baseClass=ilrepositorygui\&ref\_id=607216)) an. Abonnieren Sie dort das Forum. Hierüber erhalten Sie wichtige Informationen (z.B. Ausfall der Vorlesung etc.):\
   <img src="../.gitbook/assets/image.png" alt="Benachrichtigungen für gesamtes Forum starten" data-size="original">
4.  Finden Sie ein Team-Mitglied (4er-Gruppen).\
    Laden Sie in ILIAS unter Abgaben eine JSON-Datei in folgendem Format hoch.\


    ```
    {
        "name": "",
        "members": [
          {
              "lastname": "Mustermann",
              "firstname": "Max",
              "id": 12345,
              "email": "maxmu@stud.hs-heilbronn.de"
          },
          {
              "lastname": "Musterperson",
              "firstname": "Maxi",
              "id": 676890,
              "email": "maxim@stud.hs-heilbronn.de"
          },
          {
              "lastname": "Musterstudentin",
              "firstname": "Miriam",
              "id": 676891,
              "email": "miriamm@stud.hs-heilbronn.de"
          },
          {
              "lastname": "Musterstudent",
              "firstname": "Maximilian",
              "id": 676894,
              "email": "maxim2@stud.hs-heilbronn.de"
          }
        ]
     }
    ```

    \
    \
    Im Laufe des Tages erhalten Sie von GitLab das Benachrichtigen über Ihr Repository.&#x20;

### Hausaufgabe

**Git Grundlagen**\
Sofern noch nicht geschehen, erarbeiten Sie den Kurs [Version Control with Git](https://www.udacity.com/course/version-control-with-git--ud123) bis zum **übernächsten** Termin.&#x20;

**Git Übung**

_**Lesen Sie diese Aufgabe gründlich bis zum Ende durch!**_&#x20;

Erstellen Sie über Git-Commits ein Dialog aus Shakespear\`s "Rome und Julia". Bearbeiten Sie diese Aufgabe bereits im Team, da alle Team-Mitglieder abwechselnd einchecken müssen. Abwechseln editieren Sie die Datei und fügen Ihre Passage hinzu. Gleichzeitig nutzen Sie diese Passage als Nachricht in Ihrem Commit.&#x20;

Nutzen Sie Git-Befehle wie `git add`, `git commit`, `git push`, `git pull` und `git log`, um ihre Änderungen zu verwalten und den Dialog im Verlauf des Git Logs nachzuvollziehen.

Folgender Dialog ist über Commit-Messages und Commits im Log zu erstellen. Jedes Gruppenmitglied übernimmt hierfür eine Rolle aus dem Stück.&#x20;

> **Romeo:**\
> Wenn ich entweiht mit meiner unwerten Hand\
> Diese heilige Stätte, so gleicht dies\
> Ein zärtlicher Kuss dem frommen Pilgergruß.
>
> **Julia:**\
> Guter Pilger, zu viel Ehrerbietung\
> Zeigt deine Hand, die nur die Hand der Andacht\
> Berührt, und so wird Hand durch Hand geheiligt.
>
> **Romeo:**\
> Haben nicht Pilger Lippen, heilige Lippen?
>
> **Julia:**\
> Ja, Pilger, Lippen, die zum Beten taugen.
>
> **Romeo:**\
> O, dann, mein holder Heil’ger, lass die Lippen\
> Tun, was die Hände tun: in Andacht beten.
>
> **Julia:**\
> Die Heiligen rühren sich nicht, obgleich sie beten.
>
> **Tybalt:**\
> Dieser, bei meiner Ehre, ist ein Montague.\
> Wie kam er hier herein? Das ist ein Schurke.
>
> **Amme:**\
> Was ist das für ein Lärm? Was gibt es, Herr?
>
> **Tybalt:**\
> Ich will ihn schlagen, trotz des Friedens.
>
> **Amme:**\
> Nein, Herr, das wäre nicht weise. Lasst ihn.
>
> **Romeo:**\
> O, dann, mein holder Heil’ger, lass die Lippen\
> Tun, was die Hände tun: in Andacht beten.
>
> **Julia:**\
> Die Heiligen rühren sich nicht, obgleich sie beten.

Das Commit log soll am Ende so aussehen (via `git log)`:&#x20;

<pre><code>commit c9a3e2e (HEAD -> main)
Author: Maxi &#x3C;maxim@stud.hs-heilbronn.de>
Date:   Thu Mar 10 12:05:13 2024

    Julia: Die Heiligen rühren sich nicht,
          obgleich sie beten.

commit a7f8d4b
Author: Max &#x3C;maxmu@stud.hs-heilbronn.de>
Date:   Thu Mar 10 12:00:00 2024

      Romeo: O, dann, mein holder Heil’ger,
            lass die Lippen Tun, was die 
            Hände tun: in Andacht beten.

commit b465c31
Author: Maximilian &#x3C;maxim2@stud.hs-heilbronn.de>
Date: Thu Mar 10 11:45:43 2024

      Amme: Nein, Herr, das wäre nicht 
            weise. Lasst ihn.

commit 2a3b4c5
Author: Miriam &#x3C;miriam@stud.hs-heilbronn.de>
<strong>
</strong><strong>      Tybalt: Ich will ihn schlagen, trotz
</strong><strong>            des Friedens.
</strong>
commit 6d7e8f9
Author: Maximilian &#x3C;maxim2@stud.hs-heilbronn.de>

      Amme: Was ist das für ein Lärm? Was 
            gibt es, Herr?

commit 4a3f2b1
Author: Miriam &#x3C;miriam@stud.hs-heilbronn.de>

      Tybalt: Dieser, bei meiner Ehre, ist
            ein Montague. Wie kam er hier herein?
            Das ist ein Schurke.

commit 9c7d6e5
Author: Maxi &#x3C;maxim@stud.hs-heilbronn.de>

      Julia: Die Heiligen rühren sich nicht, 
            obgleich sie beten.

commit 1f2e3d4
Author: Max &#x3C;maxmu@stud.hs-heilbronn.de>

      Romeo: O, dann, mein holder Heil’ger,
            lass die Lippen Tun, was die
            Hände tun: in Andacht beten.

commit 8b7a6c9
Author: Maxi &#x3C;maxim@stud.hs-heilbronn.de>

      Julia: Ja, Pilger, Lippen, die zum 
            Beten taugen.

commit 3d4e5f6
Author: Max &#x3C;maxmu@stud.hs-heilbronn.de>

      Romeo: Haben nicht Pilger Lippen,
            heilige Lippen?

commit 7a8b9c0
Author: Maxi &#x3C;maxim@stud.hs-heilbronn.de>

      Julia: Guter Pilger, zu viel Ehrerbietung
            Zeigt deine Hand, die nur die Hand
            der Andacht Berührt, und so wird Hand
            durch Hand geheiligt.

commit 5f6e7d8
Author: Max &#x3C;maxmu@stud.hs-heilbronn.de>

      Romeo: Wenn ich entweiht mit meiner unwerten
            Hand Diese heilige Stätte, so gleicht
            dies Ein zärtlicher Kuss dem frommen
            Pilgergruß.
</code></pre>

Stellen Sie sicher, dass sich im Root-Verzeichnis Ihres Repositories eine Datei `dialog.txt` befindet, in der die jeweiligen Text-Passagen eingecheckt sind. Die Textpassagen entsprechend der jeweiligen Log-Message. &#x20;

**HINWEIS**: Sollte etwas schiefgehen - leeren Sie die `dialog.txt` Datei und checken Sie (gleichgültig welches Team-Mitglied) diese mit dem Kommentar "NEUSTART" (`-m "NEUSTART"`) ein , dann beginnen Sie von vorne. Fehlversuche passieren, sind im Rahmen des Lernens "normal" und werden daher auch nicht gewertet. Ziel ist, dass die Checkins am Ende einmal wie oben beschreiben bei Ihnen funktioniert haben.&#x20;
