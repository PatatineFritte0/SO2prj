# Progetto "Simulazione Circuito Quantistico"

**Autore:** Simone Comignani

Il progetto è stato fatto in modo tale che i file siano il più flessibili possibile,  
ma comunque ci sono dei vincoli da rispettare per far sì che tutto funzioni correttamente.

---

## 1. Comandi e Struttura dei File

Nei file, l'inizio di un comando è `#` seguito dal nome: `"qubits"`, `"define"`, `"circ"`, `"init"`.

### Regole varie dei comandi:

- È possibile inserire lo stesso comando più volte e ovunque nel file.  
  Si prenderà in considerazione **solo il primo comando valido** a partire dall'inizio del file.

#### **Caso particolare – `#define`:**

Il comando `#define` può e deve essere usato più volte.  
Solo la **prima definizione valida con nome corretto e matrice valida** sarà considerata.

**Esempio:**

```
#circ X Y *  
#circ X Y I  
#define Y (matrice non valida)  
#define Y (matrice valida) *  
#define X (matrice valida) *  
#define X (matrice valida)  
#define I (matrice valida)
```

Gli elementi segnati con `*` saranno quelli presi in considerazione.

- Nel file `circ` è possibile dare nomi personalizzati alle matrici, es. `pippo`.
- Il numero massimo di caratteri per i nomi e il numero di matrici è 64.  
  Può essere modificato cambiando il valore `MAX_MATRIX_NAME` nel file `header/costants.h`.
- Il **primo comando `circ` valido** definisce quali matrici sono necessarie. Se una manca, il programma termina (errore gestito).
- Il comando `init` deve avere **subito dopo** la definizione di un vettore.  
  ⚠️ Attenzione: nei vettori, invece dei caratteri `(` e `)` usare **`[` e `]`**
- Il comando `define` deve avere subito dopo **il nome della matrice**, seguito dalla definizione della matrice.

---

## 2. Numeri Complessi

I numeri complessi devono avere una sintassi matematica **completa**.

### Come definire un numero complesso

Si può inserire un'espressione o un dato singolo. L'espressione deve essere **atomica** (cioè senza parentesi annidate o operazioni non supportate).

**Esempi validi e non:**

| Espressione | Valida? |
|-------------|---------|
| `i`         | ✅       |
| `1`         | ✅       |
| `1+1`       | ❌       |
| `4+i`       | ✅       |
| `4i`        | ✅       |
| `4ii`       | ❌       |
| `4+4i`      | ✅       |
| `4i+1`      | ✅       |
| `4i++1`     | ❌       |
| `i4-1`      | ✅       |
| `4(1+i)`    | ❌       |

---

## 3. Definizione di Vettori e Matrici

### Vettori

- Inizio con `(`, separati da virgole, spazi ammessi.
- Devono rimanere **su una singola riga**, altrimenti non saranno validi.
- Terminare con `)`.

**Esempio:**

```
(1, i, 2+3i)
```

### Matrici

- Inizio con `[`, definire vettori validi in sequenza.
- Devono rimanere **su una singola riga**, altrimenti non saranno validi.
- Terminare con `]`.

**Esempio:**

```
[ (1, 0), (0, 1) ]
```

---

## 4. Struttura del Progetto

La struttura del progetto è organizzata in più cartelle per separare i file di input, il codice sorgente e gli header:

<pre>
.
├── data/                  # Contiene i file di input
│   ├── init               # Vettore di inizializzazione
│   └── circ               # Descrizione del circuito e delle matrici
│
├── source/                # Codice sorgente in C
│   ├── dataRecovery.c     # Funzioni per il recupero dati dalle stringe
|   ├── fileManagement.c   # Funzioni per la gestione dei file
|   ├── matrixOperation.c  # Funzioni per le operazione tra matrici (anche per dichiararle)
|   ├── utils.c            # Funzioni utili per la pulizia del codice e debugging
│   └── structure.c        # Funzioni relative alla struttura `Complex`
│
├── header/                # File header
│   ├── dataRecovery.h     # prototipi per le funzioni per il recupero dati dalle stringe
|   ├── fileManagement.h   # prototipi per le funzioni per la gestione dei file
|   ├── matrixOperation.h  # Funzioni per le operazione tra matrici (anche per dichiararle)
|   ├── utils.h            # prototipi per le funzioni utili per la pulizia del codice e debugging
│   ├── structure.h        # Prototipi e definizione della struttura `Complex`
│   └── costants.h         # Costanti utilizzate nel progetto, definite con `#define`
│
├── main.c                 # Entry point del programma
├── Makefile               # Per compilare il progetto
</pre>

### Indicazioni

- I file `init` e `circ` **devono essere posizionati nella cartella `data/`** per poter essere letti correttamente dal programma.
- Il progetto è pensato per essere compilato con il `Makefile` presente nella root del progetto.  
  Basta eseguire `make` da terminale per generare l'eseguibile, eseguirlo e pulire tutto.
- I percorsi sono gestiti nel codice in modo da fare riferimento a `data/init` e `data/circ` **relativamente alla root del progetto**.
