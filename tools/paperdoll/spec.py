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

# Hand corrections for the trainer pic colour roles: (pic palette symbol, is back pic) -> {palette index: role or None}
PIC_ROLE_FIX = {
    ("Red", False):     {10: None, 11: None, 12: "top", 13: "top"},
    ("Red", True):      {8: "hair", 13: None, 14: None},
    ("Leaf", False):    {4: None, 6: "top", 7: "top", 12: "bottom", 13: "bottom"},
    ("Leaf", True):     {1: "hair", 4: "hair", 6: "top", 8: "hair", 9: "hat", 11: None, 12: None, 13: None, 14: None},
    ("Lucas", False):   {1: None, 2: None, 6: None, 7: None},
    ("Lucas", True):    {3: None, 4: None, 11: None, 12: "hair"},
    ("Dawn", False):    {1: None, 7: None, 3: None, 4: None, 10: "top", 13: "bottom"},
    ("Dawn", True):     {4: None, 12: "hair"},
    ("Hilbert", False): {9: "hat", 10: "hair", 12: "hat"},
    ("Hilbert", True):  {6: None, 9: "hair"},
    ("Hilda", False):   {5: None, 7: "hair", 11: "hair", 3: "bottom", 10: "bottom", 4: "top", 6: "top", 12: "hat"},
    ("Hilda", True):    {4: "hair", 5: "hat", 6: "hat", 12: None},
    ("Nate", False):    {8: "hair", 10: None, 11: "hat", 3: None, 5: "top", 2: "top", 9: None, 7: "bottom"},
    ("Nate", True):     {4: "hair", 9: "hair", 12: "top"},
    ("Rosa", False):    {9: "hair", 6: None, 7: None, 10: "hat", 8: "top", 12: "bottom"},
    ("Rosa", True):     {4: "hair", 5: "hat", 6: "hat", 12: None},
    ("Calem", False):   {10: "hat", 13: "hat", 6: None, 11: None, 15: "skin"},
    ("Calem", True):    {2: "hat", 3: "hat", 5: "hair", 6: None, 11: None, 14: "top"},
    ("Serena", False):  {13: "hair", 5: "hair", 4: "hat", 10: "bottom", 6: "bottom", 12: "bottom", 7: None},
    ("Serena", True):   {1: None, 5: None, 7: None, 9: "hair", 13: "hair", 15: "hair", 12: None},
    ("Elio", False):    {6: "bottom", 10: None, 15: "skin"},
    ("Elio", True):     {5: "hair", 6: None, 11: None, 14: "top"},
    ("Selene", False):  {8: "hat", 3: "hair", 4: "hair", 9: None, 13: "skin", 5: "bottom"},
    ("Selene", True):   {9: "hair", 13: "hair", 15: "hair", 6: "top", 12: None, 1: None},
    ("Victor", False):  {9: None, 6: "bottom", 8: None, 5: "hair", 11: "hair", 7: "top", 14: "top"},
    ("Victor", True):   {2: "hat", 6: None, 7: "hat", 5: "hair", 11: None, 14: "top"},
    ("Gloria", False):  {2: None, 5: "hat", 4: "hair", 9: "hair", 11: "bottom", 6: None},
    ("Gloria", True):   {1: "hat", 3: "hat", 6: "top", 7: "top", 9: "hair", 13: "hair", 15: "hair", 12: None},
    # villains: cap/hood and uniform often share colours -> counted as top
    ("RocketGruntMFrlg", False):   {1: "skin", 9: None, 10: None, 12: "top", 13: "top"},
    ("RocketGruntFFrlg", False):   {1: "skin", 8: None, 9: "hair", 10: "hair", 5: None, 14: None, 12: "top", 13: "top"},
    ("LeaderGiovanniFrlg", False): {1: "skin", 4: "hair", 5: None, 6: None, 7: None, 8: None, 10: "top", 11: "top", 12: "top", 13: "top"},
    ("Cyrus", False):              {4: None, 5: "top", 7: "top", 8: "bottom", 9: "bottom", 11: None},
    ("MagmaGruntM", False):        {1: "skin", 10: "top", 11: None, 12: "top", 13: "top"},
    ("MagmaGruntF", False):        {1: "skin", 5: "bottom", 6: "bottom", 7: None, 9: None, 10: "top", 11: None, 12: "top", 13: "top"},
    ("MagmaLeaderMaxie", False):   {1: "skin", 5: None, 6: None, 7: None, 8: "bottom", 10: "top", 12: "top", 13: "top"},
    ("AquaGruntM", False):         {1: "skin", 5: "bottom", 6: "bottom", 7: "bottom", 9: None, 12: None, 13: None, 8: None},
    ("AquaGruntF", False):         {1: "skin", 10: "hair", 12: "hair", 13: "hair", 5: "bottom", 6: "bottom", 7: "bottom", 8: None, 9: None},
    ("AquaLeaderArchie", False):   {1: "skin", 5: "top", 7: "top", 8: "top"},
    # drawn pics with soft shading: outline and dark shading stay, the hands carry the skin colours
    ("Lysandre", False):           {7: None, 12: None, 13: None},  # suit highlights / buttons stay
    ("GalacticGruntM", False):     {1: None, 2: "top", 3: None, 4: "top", 5: "hair", 6: None, 7: "hair", 8: "hair",
                                    9: "top", 10: "skin", 11: "skin", 12: None, 13: "bottom", 14: "skin", 15: "skin"},
}
