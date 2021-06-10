#include "CGame.hpp"

int main ( void )
{
	CGame game;
	game.start();
	
	return 0;
}

/*! \mainpage
EUGOR (Grafické RPG)
====================
Autor: Jakub Votrubec

==> Téma z Progtestu

Naprogramujte engine pro grafické RPG

Váš engine:

1. ze souboru nahraje definici světa (mapu, předměty, příšery, ...)<br>
2. umožní vytvořit herní postavu (její atributy) a následnou hru<br>
3. implementuje jednoduchý soubojový systém a inventář<br>
4. umožňuje ukládat a načítat rozehrané hry<br>
5. Engine bude fungovat jako realtimová nebo tahová hra.<br>

Jak může vypadat mapa?

'#' označuje zeď, T označuje truhlu s pokladem, > označuje pozici (směr otočení) hráče.

```
###################
# T   #      #    #
#           ###   #
#     ##   #      #
#     #       # > #
###################
```

Kde lze využít polymorfismus? (doporučené)<br>

Soubojové vlastnosti: postava, příšera, stroj (např. past, bomba,...)<br>
Efekty akcí: souboje, události (pád ze stromu,...), přechod do jiné místnosti<br>
Inventář: postava, batoh, toulec, kapsa<br>
Uživatelské rozhraní: konzole, ncurses, SDL, OpenGL (různé varianty), ...<br>

Zadání hry EUGOR
====================

Vytvoříme hru "EUGOR", hráč si bude moci vytvořit postavu a za ní se pak pokusí<br>
uniknout ze zhoubného podzemního sídla lorda Eugora, který tu hráče uvěznil.<br>

Mapa úrovně může vypadat nějak takto:<br>

```
                                                                                              
  ------------                   -------------------------        ------                      
  │.$......H.│                 ##+..........g............+########+...F|                      
  │....@.....|                 # |....................$..│        |..b.|                      
  │.F........+################## |...a...................|        --+---                      
  │.....w....|                 # --------------+----------          #                         
  ------------                 #               #                    #                         
                               #               #                    #    ------------------   
                               #               #                    #####+........s.......|   
                               #        -------+--------------------     |................|   
       ----------------        #        |......b...................|     |..h.............|   
       |..............|        #        |..........................|     -------------+----   
       |......F..b....|        #        |.........>...m............|                  #       
       |..............+####### #        |..........................|             ######       
       -+--------------                 |...................b......|             #            
        #                               --------------------+-------         ----+--          
        #########################                           #                |.....|          
                                #                           #                |.g...|          
  ------------------------------+-                ---------0-+------------   |...b.|          
  |.................s............|           #####+.....................|  ##+.....|          
  |.l............................|           #    |....s....r....s......|  # |.....|          
  |..............................+############    |.....................+### |..H..|          
  --------------------------------                -----------------------    -------          
                                                                                            
```

Písmena a znaky reprezentují jednotlivé předměty, příšery a hráče. Například hráč je<br>
reprezentován znakem '@'<br>

Hráč se pohybuje pomocí šipek na klávesnici, sbírá zlato a předměty, které mu<br>
vylepšují jeho životy, brnění a udělené poškození. Dále může sbírat předměty, které<br>
doplňují jídlo a životy. V podzemí bude muset hráč přemáhat lecjaké příšery, aby<br>
se dostal ven z komplexu.<br>

Všechny složitější části mapy (předměty, příšery, hráč,..) jsou potomkem třídy CTile<br>
a udržují si vždy polohu a písmeno/znak, který je reprezentuje. Dále se díly mapy<br>
rozdělují na ty, které se mohou pohybovat (CMovable) a ty které nemohou(CStatic).<br>
Tím zajišťuji, že mohu integrovat přes všechny postavy, které dědí CMovable<br>
a můžu je přesouvat pomocí děděné metody move(), dle předem dané logiky (základní AI).<br>

Hráč je realizován pomocí třídy CPlayer, která udržuje jeho atributy a polohu. Dále<br>
také tato třída využívá tříd CWeapon, CInventory a CWear pro udržování hráčova vybavení<br>
a inventáře.<br>

== Použití polymorfismu

Jednotlivé věci ve vlastnictví hráče, ale i ty na zemi, jsou realizovány jako potomci<br>
třídy CItem. Tato třída obsahuje abstraktní metodu useItem, kterou každý její potomek<br>
implementuje jinak. Například u objektů typu CWearable je implementována metoda useItem <br>
jako "nasazení" předmětu. Dále u objektů typu CWeapon se metoda use implementuje jako<br>
"vzít do ruky". V neposlední řadě u objektů typu CConsumable metodu use můžeme chápat<br>
jako "sníst/vypít".<br>

Další malé využití polymorfismu je u všech potomků třídy CTile, například u metody draw(). <br>
Každý objekt musí zjistit, zdali je v inventáři nebo na herní ploše. Pokud je na herní<br>
ploše vypíše se jeho specifickou barvou.<br>

Poslední využití  polymorfismu je u pohybu objektů (metoda move()) typu CMovable, CPlayer<br>
se pohybuje jinak, nežli CBeast, který je závislí na proměnné m_Pathfinding, který určuje <br>
jak se příšera pohybuje.<br>
 */