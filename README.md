# Game of Life Extended

## Ce face programul?

Acest program implementează Game of Life cu câteva funcții extra. Ai o grilă cu celule care pot fi vii (X) sau moarte (+), și ele evoluează după niște reguli simple. Am adăugat 4 moduri diferite:

1. **Task 1**: Simularea normală - vezi cum evoluează grila pas cu pas
2. **Task 2**: Ca taskul 1, dar îți spune exact care celule s-au schimbat la fiecare pas
3. **Task 3**: Compară evoluția normală cu o regulă alternativă folosind un arbore binar
4. **Task 4**: Găsește cel mai lung "drum" prin celulele vii conectate (lanț hamiltonian)

Pentru mai multe detalii despre regulile Game of Life, consultați [acest link](https://site-pa.netlify.app/proiecte/game_of_life/).

## Cum să îl folosești

### Ce ai nevoie
- Un compilator C (gcc e recomandat)
- Linux/macOS/Windows cu MinGW
- Pentru testare: instaleazaa acest [checker](https://site-pa.netlify.app/proiecte/checker/) - tool care îți verifică automat dacă outputul programului tău este corect pe diverse teste

### Testarea cu checker-ul
**Înainte să predai proiectul, e recomandat să îl testezi cu checker-ul. Acesta:**

- Verifică toate cele 4 moduri de funcționare
- Testează pe multiple seturi de date
- Îți arată exact unde sunt diferențele dacă outputul nu e corect

**Cum să folosești checker-ul:**

1. Compilează programul și asigură-te că funcționează local
2. Rulează checker-ul și analizează rezultatele
3. Corectează eventualele probleme și retestează

### Compilare

```bash
gcc task1nou.c
```

### Rulare checker
```bash
./checker-linux-amd64 -i
```

## Cum e organizat codul

### Structurile principale

**`ca`** - Pentru coordonate:
```c
typedef struct {
    int linie, coloana;
} ca;
```

**`Nod`** - Pentru arborele binar:
```c
typedef struct Nod {
    char **mat;          // starea curentă a grilei
    int h;               // nivelul în arbore
    struct Nod *st, *dr; // copiii
} Nod;
```

**`LantHam`** - Pentru lanțurile hamiltoniene:
```c
typedef struct {
    celula *celule;    // celulele din lanț
    int nr_celule;     // câte sunt
} LantHam;
```

### Funcțiile importante

**Gestionarea memoriei:**
- `aloc_din_mat()` - alocă o matrice nouă
- `free_mem()` - eliberează memoria
- `cop_mat()` - face o copie a matricei

**Game of Life:**
- `numar_vec()` - numără câți vecini are o celulă
- `act_mat()` - aplică regulile pentru o generație
- `act_mat_nou()` - la fel ca `act_mat()` dar ține minte ce s-a schimbat

**I/O:**
- `citire_mat()` - citește configurația din fișier
- `scriere_mat()` - scrie rezultatele
- `afis_stiva()` - salvează coordonatele modificărilor

**Pentru modurile avansate:**
- `regula_noua()` - aplică regula alternativă (celulele cu 2 vecini devin vii)
- `creare_arbor_binar()` - construiește arborele pentru modul 3
- `gasestelantul()` - găsește lanțul hamiltonian maximal

### Ce algoritmii am folosit

1. **Game of Life standard** - regulile clasice
2. **Backtracking** - pentru căutarea lanțurilor hamiltoniene
3. **DFS** - pentru găsirea componentelor conexe
4. **Arbore binar** - pentru compararea celor două seturi de reguli

### Task-urile în detaliu

**Task 1:** Simulare simplă. Aplică regulile Game of Life de câte ori îi spui și salvează toate stările.

**Task 2:** Ca modul 1, plus că îți spune exact care celule s-au schimbat la fiecare pas. Opțional poate să "desfacă" toate modificările pentru a reveni la starea inițială.

**Task 3:** Construiește un arbore unde fiecare nod are două "viitori posibile":
- Stânga: cu regula alternativă
- Dreapta: cu regulile normale

**Task 4:** Pentru fiecare stare din arborele de la modul 3, încearcă să găsească cel mai lung "drum" prin celulele vii care sunt conectate.

## Note importante

- Am încercat să gestionez memoria cât de bine am putut, evitand memory leaks.
- Codul e scris în C clasic.
