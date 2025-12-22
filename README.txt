Limitacions: 
- Les potencies nomes poden tenir exponents enters

Decissions de disseny:
- En el for el id es imutable i si es modifica el seu valor no afecta a les iteracions restants
- Permetre casts de enters i reals a expressions boleanes
- Permetre ometre break i default en el switch controlant el comportament
- No permetre cast de arrays sensers

Optimitzacions:
- Avaluació d'expressions aritmètiques que nomes tenen literals
- Reutilització de temporals
- Assignacions de boleans
- Optimització algebràica 