#!/bin/bash
# Pokemon Gangster Outbreak: neuesten Stand von GitHub holen, Feuerrot-ROM bauen
# und auf den Windows-Desktop kopieren.  Aufruf in WSL:  ./update_und_bauen.sh
set -e
cd "$(dirname "$0")"

# Remote finden, der auf das Gangster-Outbreak-Repo zeigt (z. B. "myhack")
REMOTE=$(git remote -v | grep -i 'gangster-outbreak' | head -1 | cut -f1)
if [ -z "$REMOTE" ]; then
    echo "Kein Remote fuer Pokemon-Gangster-Outbreak gefunden."
    exit 1
fi

echo ">> Hole neuesten Stand von $REMOTE/master ..."
git pull --ff-only "$REMOTE" master

echo ">> Baue die ROM ..."
make firered -j"$(nproc)"

# Windows-Desktop finden (klappt auch, wenn der Desktop in OneDrive liegt)
WIN_DESKTOP=$(powershell.exe -NoProfile -Command '[Environment]::GetFolderPath("Desktop")' | tr -d '\r')
DESKTOP=$(wslpath "$WIN_DESKTOP")

cp pokefirered.gba "$DESKTOP/Pokemon Gangster Outbreak.gba"
echo ">> Fertig: $WIN_DESKTOP\\Pokemon Gangster Outbreak.gba"
