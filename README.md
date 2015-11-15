## A házi feladat leírása

Az alábbiakban olvashatod a Számítógépes Grafika tárgy kiadott házi feladatait.

### Első feladat

Adott egy 1000m x 1000m-res mező, amelyet kezdetben a teljes viewport-ban látunk. Az egér bal gomb lenyomásával pontokat helyezünk el a mezőn. A pontokat piros belsejű, fehér körvonalú 5m sugarú körrel jelenítjük meg. Egyrészt a pontok egy fehér, önmagában zárt Catmull-Rom spline kontrollpontjai, amelyekhez tartozó időparaméter a lenyomáskori óra állás. Másrészt az első két pont egy egyenest definiál, a harmadik pedig egy fókusz pontot. A egyenes és fókuszpont által kijelölt parabola a síkot két részre osztja. A parabola fókuszpontot tartalmazó részében a hátteret sárgára, a másik részben türkizre színezzük ki. A Catmull-Rom spline és a parabola egyik metszéspontjában (már ha van) zöld színnel érintő egyenest rajzolunk mind a parabolához, mind pedig a spline-hoz.

A SPACE lenyomásának hatására a kamera ablak 500m x 500m-resre megy össze úgy, hogy a középpontja helyben marad. Ezután (2m/sec,3m/sec) kezdeti sebességgel vándorútra indul. Ha a kameraablak ütközik a mező szélével, onnan biliárd golyó módjára verődik vissza.

Minden, a feladat szövege által nem specifikált részlet szabadon megválasztható.

Beadási határidő: **2015. 10. 17. 23:59**

### Második feladat

Készítsen sugárkövető programot, amely egy szobát és az abban mozgó üveg ellipszoidot jelenít meg mozgó pontszerű fényforrás megvilágítása mellett.

**A virtuális világban a fény terjedési sebessége 1 m/s!!! (ezen feltételt mellőző megoldások nem fogadhatók el)**

A szoba egyik oldala aranyból van, optikailag sima és forgásparaboloid geometriájú. A szoba többi oldala diffúz és valamilyen mintával textúrázott sík. A szoba mérete 10 m x 10 m x 10 m volna, ha eltekintenénk a paraboloid kitüremkedésétől.

Az üveg ellipszoid tengelyei a világkoordinátarendszer egyik tengelyével sem párhuzamosak. A leghosszabb tengely 2 m, a legrövidebb 0.5 m. Az ellipszoid egyenes vonalú egyenletes mozgást véget a szoba sarkából 0.5 m/sec sebességgel úgy, hogy legalább 10 másodpercig a szoba belsejében tartózkodik, mialatt a kamerából látható.

A pont fényforrás ugyancsak egyenesvonalú egyenletes mozgást végez úgy, hogy az első 10 másodpercben a szoba beljesében van.

A programnak a SPACE lenyomásának időpontjában keletkezett képet kell megjelenítenie.

Az arany törésmutatója és kioltási tényezője: r: 0.17/3.1, g: 0.35/2.7, b: 1.5/1.9

Az üveg törésmutatója és kioltási tényezője: r: 1.5/0, g: 1.5/0, b: 1.5/0

Beadási határidő: **2015. 11. 08. 23:59**

### Harmadik feladat

Készítsen CSIRGURU BOMBÁZÓ játékot. A csirguru a csirke és a kenguru nászából született jószág, csirke teste, csőre, szemei, taraja van, valamint egy lába, amivel kenguruszerűen ugrál. A lába a csípőízületben nem fordul el, a térízületben hátra fordul, a bokában előre (amit a normál csirke esetében hajlamosak vagyunk előre hajló térdnek hinni). Az egy szem lábujj ugyancsak felfelé hajlítható, az ízületek tengelyei egymással párhuzamosak.

A csirguruk testét néhány zárt Bézier görbével definiált keresztmetszetek Catmull-Rom spline-t alkalmazó transzfinit interpolációjával kell létrehozni. A szem, csör, taraj, lábrészek ismert egyenletű paraméteres felületekből (pl. gömb, kúp, henger, paraboloid stb.) összerakhatók.

A csirguruk egy sík, textúrázott és megvilágított terepen a bomba aktuális helye alatt, a program indításától számított másodpercenként születnek meg, majd elkezdenek ugrándozni. A lendületvételi fázisban a csirguru feneke a földhöz közelít, majd a térdízület EGYENES VONALÚ GYORSULÓ MOZGÁST (inverz kinematika) végez, mialatt a lábujj a talajon marad, a térd pedig vízszintesen tartja a csirgurut. A bokaízület kiegyenesedése pillantában a csirguru elszakad a földtől és lendületvétel végén elért kezdősebességű ferde hajítással leírható pályán ugrik, a levegőben előreteszi a lábát, a földetérkezésnél pedig a lendületvételt visszafelé játsza le. Az ugrálást ismételgetheti, az egyes ugrálások között a haladási irányát ízlésesen változtatva.

A felhasználó egy bombát irányíthat a mező fölött egy síkban az A, W, D, Y billentyűkkel, majd SPACE-re a bomba leesik. Ha a bomba egy csirguru közelében esik le, a csirguru testrészei független ferde hajítást követnek, mialatt midegyik testrész egy-egy forgástengely körül forog.

A színteret egy irányfényforrás (nap) és ambiens fényforrás világítja meg, a csirguruk és a bombák a talajra árnyékot (síkra vetített árnyék algoritmus) vetnek.

Beadási határidő: **2015. 12. 06. 23:59**

### A megoldás módja

A feladatot C++ nyelven kell megvalósítani [**OpenGL**](http://www.opengl.org/) és [**GLUT**](http://www.opengl.org/resources/libraries/glut/) felhasználásával az alábbi [**sablon**](https://cg.iit.bme.hu/grafhazi/content/cg_template_2014_1.cpp) módosításával. A feladat megoldása során implementálni kell az **onInitialization(), onDisplay(), onKeyboard(), onMouse(), onMouseMotion()** és **onIdle()** függvényeket. Amennyiben a feladat megköveteli természetesen létrehozhatsz segédfüggvényeket is. Fontos azonban, hogy csak a jelzett részen belül található programrészeket értékeljük.

#### A forráskód feltöltése

Az elkészült forráskód feltöltésére a [**Kódfeltöltés**](https://cg.iit.bme.hu/grafhazi/index.php?cmd=src_upload) menüpont alatt van lehetőséged. A házi feladat határidejének lejártáig tetszőleges alkalommal tölthetsz fel megoldás, értékelni az utolsó változatot fogjuk.

#### A fordítási és futási eredmények ellenőrzése

A fordítás sikerességét az [**Eredmények**](https://cg.iit.bme.hu/grafhazi/index.php?cmd=chk_results) menüpont alatt tudod ellenőrizni.

### Néhány tanács

A programokat [**GNU g++ 4.4.1 verziószámú fordítóprogrammal**](http://gcc.gnu.org/gcc-4.4/) fordítjuk, mely szigorúbban követi a C++ szabványt mint a Visual Studio különböző változatai, ezért előfordulhat, hogy sikertelen lesz a fordítás annak ellenére, hogy a Visual Studio fordítójával lefordul. A pontos hibáról az [**Eredmények**](https://cg.iit.bme.hu/grafhazi/index.php?cmd=chk_results) menüpont alatt tájékozódhatsz.

A feladatokat ISO8859-1 vagy ISO8859-2 formátumban várjuk, az UTF-16 és UNICODE formátumú programok hibás fordítást okoznak.

A sablonban szereplő elválasztó sorokat (//~~~~~~...) célszerű a beküldött programban is megőrizni. Ellenkező esetben előfeldolgozási hiba lehet, bár a gyakoribb esetekre a parsert felkészítettük.

#### Gyakori hibák

- Gyakori hiba konstans argumentumot átadni referenciát váró függvénynek. Pl. **void fv(int& a)** függvényt **void fv(2)**-vel hívni. Ez nem okoz hibát Visual Studio alatt az alap beállításokkal, de a szabvány szerint hibás.
- A tipikus C++ hibákról jó összefoglalót találhatsz ezen az [**oldalon**](http://www-h.eng.cam.ac.uk/help/tpl/languages/C++/FAQ.html).
- Az OpenGL specifikáció nem rendelkezik a ModelView és Projection mátrixok alapértelmezett értékeiről. Ezek a program indulásakor tetszőlegesek lehetnek. Több beküldött feladatnál tapasztaltuk, hogy hibásan feltételezte az egység mátrixot. A kiadott forráskód-sablonban ezért inicializáljuk a mátrixokat a futás kezdetén.
- Több beküldött megoldásban tapasztaltuk az előfordítói #define direktívával definiált makrók hibás használatát. Semmi sem garantálja, hogy más rendszereken vagy akár csak más GLUT verzióban a numerikus konstansok megegyeznek, ezért hibás programozói gyakorlat ezen numerikus konstansok használata. Helyettük az előfordítói makrókat kell használni.
- Az onIdle() függvény akkor hívódik amikor semilyen esemény nem érkezik az rendszertől. Ennek megfelelően semmi sem garantálja, hogy mikor fog lefutni. Ebből következően itt olyan műveleteket végezni, melyek nélkül a renderelés hibás lesz (pl. a mátrixok beállítása) nem érdemes.
- Nehány hasznos tanács a [**GLUT**](http://www.opengl.org/documentation/specs/glut/spec3/node90.html#SECTION000130000000000000000) használatához.
- Csak a sablonban regisztralt callback fuggvenyeket erdemes hasznalni, mivel a teszt rendszerben a tobbi glut fuggveny meghivasa sikertelen lesz.
