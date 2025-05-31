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

- Nel file `circ` è possibile dare nomi personalizzati alle matrici, es. `pippo` (senza virgolette).
- Il numero massimo di caratteri per i nomi e il numero di matrici è 64.  
  Può essere modificato cambiando il valore `MAX_MATRIX_NAME` nel file `header/costants.h`.
- Il **primo comando `circ` valido** definisce quali matrici sono necessarie. Se una manca, il programma termina (errore gestito).
- Il comando `init` deve avere **subito dopo** la definizione di un vettore.  
  ⚠️ Attenzione: nei vettori, invece dei caratteri `(` e `)` usare **`\[` e `\]`**
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

- Inizio con `\[`, definire vettori validi in sequenza.
- Terminare con `\]`.

**Esempio:**

```
\[
(1, 0),
(0, 1)
\]
```
