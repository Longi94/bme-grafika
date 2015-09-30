## A házi feladat leírása

Az alábbiakban olvashatod a Számítógépes Grafika tárgy kiadott házi feladatait.

### Első feladat

Adott egy 1000m x 1000m-res mező, amelyet kezdetben a teljes viewport-ban látunk. Az egér bal gomb lenyomásával pontokat helyezünk el a mezőn. A pontokat piros belsejű, fehér körvonalú 5m sugarú körrel jelenítjük meg. Egyrészt a pontok egy fehér, önmagában zárt Catmull-Rom spline kontrollpontjai, amelyekhez tartozó időparaméter a lenyomáskori óra állás. Másrészt az első két pont egy egyenest definiál, a harmadik pedig egy fókusz pontot. A egyenes és fókuszpont által kijelölt parabola a síkot két részre osztja. A parabola fókuszpontot tartalmazó részében a hátteret sárgára, a másik részben türkizre színezzük ki. A Catmull-Rom spline és a parabola egyik metszéspontjában (már ha van) zöld színnel érintő egyenest rajzolunk mind a parabolához, mind pedig a spline-hoz.

A SPACE lenyomásának hatására a kamera ablak 500m x 500m-resre megy össze úgy, hogy a középpontja helyben marad. Ezután (2m/sec,3m/sec) kezdeti sebességgel vándorútra indul. Ha a kameraablak ütközik a mező szélével, onnan biliárd golyó módjára verődik vissza.

Minden, a feladat szövege által nem specifikált részlet szabadon megválasztható.

Beadási határidő: **2015. 10. 17. 23:59**

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