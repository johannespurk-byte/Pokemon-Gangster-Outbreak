#!/bin/sh
# Gangster Outbreak: rebuild the battle pics made from user-supplied drawings.
# usage: tools/ow_import/trainer_pics.sh SOURCE_DIR   (SOURCE_DIR holds the drawings under the names below)
# All figures are limited to 54x60 pixels like the FRLG trainers; drawings are cut at the knees.
set -e
S="$1"; T=graphics/trainers/front_pics; C="python3 tools/ow_import/import_trainer_pic.py --max=54x60"
$C --outline --tolerance=60 "$S/piers.png"       $T/piers.png 0 0 150 215
$C --outline --tolerance=60 "$S/yell_m.png"      $T/yell_grunt_m.png 0 0 181 215
$C --outline --tolerance=60 "$S/yell_f.png"      $T/yell_grunt_f.png
$C                          "$S/guzma.png"       $T/guzma.png 80 0 204 161
$C                          "$S/lysandre.png"    $T/lysandre.png
$C --outline --tolerance=60 "$S/skull.png"       $T/skull_grunt_m.png 0 0 141 361
$C --outline --tolerance=60 "$S/skull.png"       $T/skull_grunt_f.png 141 0 300 270
$C --outline --tolerance=60 "$S/galactic.png"    $T/galactic_grunt_m.png 0 0 220 340
$C --outline --tolerance=60 "$S/galactic.png"    $T/galactic_grunt_f.png 220 0 359 340
$C --outline --tolerance=15 "$S/archer.png"      $T/archer.png 0 0 316 450
$C --outline                "$S/tabitha.png"     $T/tabitha.png
$C                          "$S/flare_grunt.png" $T/flare_grunt_m.png
$C --outline --tolerance=0  "$S/flare_admin_f.png" $T/flare_admin_f.png
$C --outline --tolerance=25 "$S/flare_admin_m.png" $T/flare_admin_m.png
