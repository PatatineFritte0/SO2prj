Projetto "Simulazione Circuito Quantistico"
Autore: Simone Comignani

Il progetto è stato fatto in modo tale che i file siano il più flessibile possibile, 
ma comunque ci sono dei vincoli da rispettare per far si che tutto funzioni correttamente.

1) nei file l'inizio di un comando è '#' e subito dopo il nome "qubits"/"define"/"circ"/"init"
   regole varie dei comandi:
- è possibile inserire lo stesso comando più volte e dove si vuole nel file, si prendera in considerazione il primo
  comando scritto in maniera valida partendo dall'inizio del file
  CASO PARTICOLARE:
  il comando #define ovviamente deve essere preso più volte, quindi solo la prima con il nome giusto con matrice valida
  verrà preso in considerazione.
  esempio: se avessimo un circ del genere

  #circ X Y *
  #circ X Y I
  #define Y (matrice non valida)
  #define Y (matrice valida) *
  #define X (matrice valida) *
  #define X (matrice valida)
  #define I (matrice valida)

  quelli con accanto '*' saranno quelli presi in considerazione
- nel file circ è possibile dare dei nomi alle matrici come per esempio "pippo" (il nome può non avere le virgolette)
  il numero di caratteri nel nome come il numero di matrici massime inseribili al momento ha un massimo di 64, tuttavia però
  è facilmente modificabile se si apre il file costants.h in header e si modifica il MAX_MATRIX_NAME.
- il primo comando circ valido che trova detterà quali matrici prendere dal file, se una di queste matrici manca il programma
  terminerà con un errore (gestito)
- il comando init deve avere subito dopo la definizione di un vettore
  attenzione: i vettori invece dei caratteri ( e ) bisogna usare \[ e \]
- il comando define deve avere subito dopo il nome della matrice e dopo la definizione di una matrice
  
2) i numeri complessi devono avere una sintassi matematica completa.
- COME DEFINIRE UN NUMERO COMPLESSO: si può inserire un espressione o un dato singolo, l'espressione deve
  rispettare la sintassi matematica e deve essere un informazione atomica.
  esempi:
  - i (si)
  - 1 (si)
  - 1+1 (no)
  - 4+i (si)
  - 4i (si)
  - 4ii (no)
  - 4+4i (si)
  - 4i+1 (si)
  - 4i++1 (no)
  - i4-1 (si)
  - 4(1+i) (no)
- COME DEFINIRE UN VETTORE: inizio definizione del vettore si preannuncia con il carattere '(',  è possibile inserire degli spazi tra un
  dato e l'altro, inserire i numeri complessi divisi da ',', quando si ha terminato di inserire i numeri si chiude con ')'.
  Attenzione, se si va a capo il programma non valutera' valido il dato
- COME DEFINIRE UNA MATRICE: inizio definizione della matrice si preannuncia con il carattere '\[', dopodichè si devono
  definire i vettori uno dopo l'altro secondo le loro regole e infine chiudere con il carattere '\]"
