Limitacions: Les potencies nomes poden tenir exponents enters

Decissio de disseny:
- He optat per declarar els arrays sense ; final, d'aquesta manera puc combinarlos amb les variables normals
- En el for el id es imutable i si es modifica el seu valor no afecta a les iteracions restants
- Permetre casts de enters i reals a expressions boleanes
- Permetre ometre break i default en el switch controlant el comportament

Optimitzacions:
- Avaluació d'expressions aritmètiques que nomes tenen literals
- Reutilització de temporals
- Assignacions de boleans