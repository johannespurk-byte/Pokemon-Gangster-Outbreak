# Pokémon Gangster Outbreak – Arbeitsregeln für Claude

Fan-Hack auf Basis von RHH's pokeemerald-expansion, gebaut wird die **Feuerrot-Version** (`make firered`).
Mit dem User auf **Deutsch** sprechen; Texte im Spiel sind **Englisch**.

## Git: immer automatisch synchron halten

Der User arbeitet mit mehreren Claude-Sessions (Cloud und lokal auf seinem PC in WSL).
Ohne Nachfrage bei jeder Aufgabe:

1. **Vor dem Arbeiten** den neuesten Stand holen:
   - Remote finden, der auf `johannespurk-byte/Pokemon-Gangster-Outbreak` zeigt (`origin` in der Cloud, `myhack` auf dem PC; `origin` auf dem PC ist rh-hideout – dorthin nie pushen).
   - `git fetch <remote> master` und `git merge --ff-only <remote>/master`.
   - Uncommittete lokale Änderungen nie verwerfen; geht kein Fast-Forward, dem User berichten statt zu erzwingen.
2. **Vor dem Commit** wie die GitHub-CI bauen: `make firered UNUSED_ERROR=1` (geänderte .c-Dateien vorher `touch`en,
   damit sie neu übersetzt werden). Die CI baut zusätzlich `leafgreen`, `emerald`, `release` und `check` – bei
   größeren Änderungen auch diese lokal prüfen. Ungenutzte Funktionen/Variablen sind dort Fehler (`UNUSED` nutzen).
3. **Nach jeder abgeschlossenen Änderung** (Build erfolgreich):
   - committen (aussagekräftige deutsche Commit-Message),
   - direkt nach `master` pushen und den Arbeits-Branch (`feature/spielerfiguren-gen2-8`) auf denselben Stand bringen,
   - vor dem Push erneut fetchen; hat jemand anderes inzwischen gepusht, mit `--ff-only` bzw. Rebase der eigenen Commits nachziehen, nie force-pushen.
   - nach dem Push den GitHub-Actions-Lauf prüfen (`gh run list` / `gh run view`) und Fehler sofort beheben.
4. Generierte Dateien (z. B. `graphics/object_events/pics/player_parts/`, `src/data/player_custom_parts.h`) mitcommitten, wenn der Generator gelaufen ist – und vorher prüfen, dass sie korrekt sind (nicht alle gleich groß/leer).

## ROM für den User

- **Auf dem PC des Users (WSL):** nach jeder Änderung `./update_und_bauen.sh` ausführen bzw. die ROM nach dem Build als
  `Pokemon Gangster Outbreak.gba` auf den Windows-Desktop kopieren (Desktop per `powershell.exe [Environment]::GetFolderPath("Desktop")` + `wslpath`).
- **In der Cloud:** ROM bauen und dem User als Datei schicken.

## Projekt-Hinweise

- Charakter-Baukasten: `tools/paperdoll/generate.py` (+ `spec.py`) erzeugt Grafiken und `src/data/player_custom_parts.h`; Laufzeit in `src/player_custom.c`, Editor in `src/oak_speech.c`.
- Credits für fremde Sprites in `CREDITS_GANGSTER_OUTBREAK.md` pflegen.
- Offizielle Nintendo/Game-Freak-Sprites nicht selbst aus dem Netz besorgen; Dateien, die der User liefert, einbauen.
