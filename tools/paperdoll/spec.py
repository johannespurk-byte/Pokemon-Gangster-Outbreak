# Manual colour roles per template (palette indices). Head colours not listed -> hair, body colours not listed -> top.
SPEC = {
    "red":     dict(accent=[13, 14], outline=[15], white=[9], skin=[2, 3], hat=[8, 10, 11, 12], bottom=[6, 7]),
    "leaf":    dict(accent=[13, 14], outline=[15, 6], white=[9], skin=[2, 3], hat=[10, 11, 12], bottom=[5, 7, 8]),
    "lucas":   dict(outline=[15, 11], white=[12, 4, 10], skin=[6, 7], hat=[1, 2, 8], bottom=[9, 13]),
    "dawn":    dict(outline=[15, 4], white=[14], skin=[8, 5], hat=[6, 9, 10], bottom=[12, 13, 1, 2, 7]),
    "hilbert": dict(outline=[15, 1, 10], white=[14], skin=[5, 6], hat=[7, 8, 9, 12], bottom=[4, 11, 13]),
    "hilda":   dict(outline=[15, 1, 3, 12], white=[14, 8], skin=[6, 7], hat=[5, 9, 10, 11], bottom=[13]),
    "nate":    dict(outline=[15, 3], white=[14, 13], skin=[5, 6], hat=[1, 7, 11], bottom=[10, 12]),
    "rosa":    dict(outline=[15], white=[14, 11], skin=[12, 5], hat=[10], bottom=[4, 6, 8, 13]),
    "calem":   dict(outline=[15], white=[14], skin=[5, 6], hat=[1, 4, 7, 13], bottom=[9, 10, 11, 12]),
    "serena":  dict(outline=[15], white=[14], skin=[6, 9], hat=[2, 5, 8, 12], bottom=[7, 11, 13]),
    "elio":    dict(outline=[15, 5], white=[14, 8], skin=[9, 10], hat=[1, 2, 3, 7], bottom=[4, 6, 12, 13]),
    "selene":  dict(outline=[15], white=[14], skin=[8, 9, 10], hat=[1, 2, 5, 6, 7], bottom=[3, 4, 12, 13]),
    "victor":  dict(outline=[15], white=[], skin=[4, 5, 13], hat=[1, 10, 14], bottom=[2, 3, 6, 8, 9]),
    "gloria":  dict(outline=[15], white=[14], skin=[4, 6, 13], hat=[2, 7], bottom=[1, 3, 10]),
}

VILLAIN_FILES = {
    "rocket_m": "people/rocket_m.png", "rocket_f": "people/rocket_f.png", "giovanni": "people/giovanni.png",
    "cyrus": "people/cyrus.png", "magma_oras": "people/magma_grunt_oras.png",
    "magma_m": "people/team_magma/magma_member_m.png", "magma_f": "people/team_magma/magma_member_f.png",
    "maxie": "people/team_magma/maxie.png", "aqua_m": "people/team_aqua/aqua_member_m.png",
    "aqua_f": "people/team_aqua/aqua_member_f.png", "archie": "people/team_aqua/archie.png",
}
SPEC.update({
    "rocket_m":   dict(accent=[8, 9], outline=[15, 10], white=[14], skin=[2, 3], hat=[7, 12, 13], bottom=[11]),
    "rocket_f":   dict(accent=[8, 9], outline=[15, 10], white=[14], skin=[2, 3], hat=[7, 12, 13], bottom=[11]),
    "giovanni":   dict(outline=[15], white=[14], skin=[1, 2, 3], hat=[], bottom=[8, 9, 10]),
    "cyrus":      dict(outline=[15], white=[], skin=[1, 2, 3], hat=[], bottom=[8, 9]),
    "magma_oras": dict(outline=[15, 9], white=[], skin=[1, 2, 3], hat=[4, 5, 6, 12, 13, 14], bottom=[7, 10, 11]),
    "magma_m":    dict(outline=[15], white=[14], skin=[1, 2, 3], hat=[8, 9, 10], bottom=[12, 13]),
    "magma_f":    dict(outline=[15], white=[14], skin=[1, 2, 3], hat=[8, 9, 10], bottom=[12]),
    "maxie":      dict(outline=[15], white=[14], skin=[1, 2, 3], hat=[], bottom=[12]),
    "aqua_m":     dict(outline=[15], white=[14, 11], skin=[1, 2, 3], hat=[5, 6, 7], bottom=[12, 13]),
    "aqua_f":     dict(outline=[15], white=[14, 11], skin=[1, 2, 3], hat=[5, 6, 7], bottom=[]),
    "archie":     dict(outline=[15], white=[14], skin=[1, 2, 3], hat=[8, 9, 10], bottom=[11, 12, 13]),
})

# in-game palettes of the shared-palette NPC sprites (the PNG palettes are not used by the game)
PAL_FILES = {
    "rocket_m": "palettes/npc_white.pal", "rocket_f": "palettes/npc_white.pal", "giovanni": "palettes/npc_white.pal",
    "magma_m": "palettes/npc_2.pal", "magma_f": "palettes/npc_2.pal", "maxie": "palettes/npc_2.pal",
    "aqua_m": "palettes/npc_4.pal", "aqua_f": "palettes/npc_4.pal", "archie": "palettes/npc_1.pal",
}
