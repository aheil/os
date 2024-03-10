# Einheit 1: Git

### Aufgaben

1. Stellen Sie sicher, dass Git auf Ihrem Rechner vorhanden ist
   1. MacOS: [https://git-scm.com/download/mac](https://git-scm.com/download/mac)
   2. Windows:  [https://git-scm.com/download/win](https://git-scm.com/download/win)
   3. Linux: [https://git-scm.com/download/linux](https://git-scm.com/download/linux)
2. Sofern Sie noch keinen Zugang um [Hochschul-GitLab](https://git.it.hs-heilbronn.de/) haben, melden Sie sich dort einmalig an. \
   **Hinweise**: Sie erhalten beim ersten Versuch eine Fehlermeldung, dadurch lern das System jedoch Ihren Account kennen und Sie können einem Repository zugewiesen werden.
3. Melden Sie sich im jeweiligen ILIAS-Raum ([SEB ](https://ilias.hs-heilbronn.de/ilias.php?baseClass=ilrepositorygui\&ref\_id=360705)bzw. [AIB](https://ilias.hs-heilbronn.de/ilias.php?baseClass=ilrepositorygui\&ref\_id=607216)) an. Abonnieren Sie dort das Forum. Hierüber erhalten Sie wichtige Informationen (z.B. Ausfall der Vorlesung etc.):\
   <img src=".gitbook/assets/image.png" alt="Benachrichtigungen für gesamtes Forum starten" data-size="original">
4.  Finden Sie ein Team-Mitglied (2er-Gruppen).\
    Laden Sie in ILIAS unter Abgaben eine JSON-Datei in folgendem Format hoch.\
    \


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
        ]
     }
    ```

    \
    \
    Im Laufe des Tages erhalten Sie von GitLab das Benachrichtigen über Ihr Repository.&#x20;

### Hausaufgabe

Machen Sie sich mit Git vertraut. Hierfür erstellen Sie über Git-Commits ein Dialog aus Shakespear\`s "Rome und Julia". Am Besten erarbeiten Sie diese Aufgabe bereits im Team, da beide Team-Mitglieder abwechselnd einchecken müssen.

Nutzen Sie Git-Befehle wie `git add`, `git commit`, `git push`, `git pull` und `git log`, um ihre Änderungen zu verwalten und den Dialog im Verlauf des Git Logs nachzuvollziehen.

Folgender Dialog ist über Commit-Messages und Commits im Log zu erstellen:&#x20;

> Romeo: Ist es Tag?

> Julia: Nein, es ist die Nachtigall, die den Morgen anzeigt, nicht die Lerche, deren Pfeifen das Wachstum des Tages weckt. Jocund Day steht auf der Jagd und weckt die Jagd.

> Romeo: Weil sie sich in ihren Augen widerspiegeln würde, bis ich die Rückkehr von Lucentio sehe, Ich will dir mit meinen Augen zeigen, was Shakespeare sagt: „Guten Tropfen Salz“, du vergisst dann schnell.

> Julia: Nun, da du willst, werde ich dich vergeben, Wenn du mir gehorchst liebevoll. Willkommen wird der Regen bald nachlassen.

> Romeo: Meine Ehegatten hätten eine bessere Erscheinung, wenn es nicht für die Zähne wäre, die den weißen Verstand einfärben. Wir haben gute Absichten, zu schlafen.

Das Commit log soll am Ende so aussehen (via `git log)`:&#x20;

```
commit c9a3e2e (HEAD -> main)
Author: Romeo <romeo@example.com>
Date:   Thu Mar 10 12:00:00 2024

    Romeo: Ist es Tag?

commit a7f8d4b
Author: Julia <julia@example.com>
Date:   Thu Mar 10 11:50:00 2024

    Julia: Nein, es ist die Nachtigall, die den Morgen anzeigt,
          nicht die Lerche, deren Pfeifen das Wachstum des Tages weckt.
          Jocund Day steht auf der Jagd und weckt die Jagd.

commit f32e89d
Author: Romeo <romeo@example.com>
Date:   Thu Mar 10 11:40:00 2024

    Romeo: Weil sie sich in ihren Augen widerspiegeln würde,
          bis ich die Rückkehr von Lucentio sehe,
          Ich will dir mit meinen Augen zeigen,
          was Shakespeare sagt: „Guten Tropfen Salz“,
          du vergisst dann schnell.

commit b57a6c1
Author: Julia <julia@example.com>
Date:   Thu Mar 10 11:30:00 2024

    Julia: Nun, da du willst, werde ich dich vergeben,
          Wenn du mir gehorchst liebevoll.
          Willkommen wird der Regen bald nachlassen.

commit e1c4d77
Author: Romeo <romeo@example.com>
Date:   Thu Mar 10 11:20:00 2024

    Romeo: Meine Ehegatten hätten eine bessere Erscheinung,
          wenn es nicht für die Zähne wäre, die den weißen Verstand einfärben.
          Wir haben gute Absichten, zu schlafen.
```

Stellen Sie sicher, dass sich im Root-Verzeichnis Ihres Repositories eine Datei `dialog.txt` befindet, in der die jeweiligen Text-Passagen eingecheckt sind. Die Textpassagen entsprechend der jeweiligen Log-Message. &#x20;

### Weiterführendes Material

* Git Command-line Tool: [https://github.com/nschneid/git-command-overview](https://github.com/nschneid/git-command-overview)
* GitHub: [https://github.com/](https://github.com/)
* GitLab: [https://about.gitlab.com/](https://about.gitlab.com/)
* The case for Git in 2015: [http://www.netinstructions.com/the-case-for-git/](http://www.netinstructions.com/the-case-for-git/)
* Pro Git Book: [http://git-scm.com/book](http://git-scm.com/book)
* **Udacity Kurs:** [**https://www.udacity.com/course/version-control-with-git--ud123**](https://www.udacity.com/course/version-control-with-git--ud123)
* Git User‘s Manual: [http://schacon.github.com/git/user-manual.html](http://schacon.github.com/git/user-manual.html)
* Git – SVN Crashcourse: [http://git.or.cz/course/svn.html](http://git.or.cz/course/svn.html)
* Learn Git in Y minutes: [https://learnxinyminutes.com/docs/git/](https://learnxinyminutes.com/docs/git/)
* Coding Blocks Podcast: Comparing Git Workflows: [https://www.codingblocks.net/podcast/comparing-git-workflows/](https://www.codingblocks.net/podcast/comparing-git-workflows/)
* Gitflow Cheatsheet: [https://danielkummer.github.io/git-flow-cheatsheet/](https://danielkummer.github.io/git-flow-cheatsheet/)
* Gitflow: [https://nvie.com/posts/a-successful-git-branching-model/](https://nvie.com/posts/a-successful-git-branching-model/)
* Atlassian‘s Gitflow Zusammenfassung: [https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow)
* Microsoft Recommendation: [https://docs.microsoft.com/en-us/azure/devops/repos/git/git-branching-guidance?view=azure-devops\&viewFallbackFrom=vsts](https://docs.microsoft.com/en-us/azure/devops/repos/git/git-branching-guidance?view=azure-devops\&viewFallbackFrom=vsts)
* Git Workflows: [http://drincruz.github.io/slides/git-workflow-comparison/#/8](http://drincruz.github.io/slides/git-workflow-comparison/#/8)
* Git Spiel Oh my Git! :[https://ohmygit.org/](https://ohmygit.org/)
* Auschecken von Commits: [https://www.git-tower.com/learn/git/faq/git-checkout-commits/](https://www.git-tower.com/learn/git/faq/git-checkout-commits/)
* Wenn was schief geht: [https://ohshitgit.com/](https://ohshitgit.com/)
* Learn Git Branching: [https://learngitbranching.js.org/](https://learngitbranching.js.org/)

\
