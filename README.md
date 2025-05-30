Projetto "Simulazione Circuito Quantistico"
Autore: Simone Comignani

Il progetto è stato fatto in modo tale che i file siano il più flessibile possibile, 
ma comunque ci sono dei vincoli da rispettare per far si che tutto funzioni correttamente.

1) nei file l'inizio di un comando è '#' e subito dopo il nome "qubits"/"define"/"circ"/"init"
   regole varie dei comandi:
- è possibile inserire lo stesso comando più volte e dove si vuole nel file, il primo scritto in maniera valida partendo
  dall'inizio del file verrà preso un considerazione
  CASO PARTICOLARE:
  il comando #define ovviamente deve essere preso più volte, quindi solo la prima con il nome giusto con matrice valida
  verrà preso in considerazione.
  esempio: se abbiamo un circ del genere

  #circ X Y *
  #circ X Y I
  #define Y (matrice non valida)
  #define Y (matrice valida) *
  #define X (matrice valida) *
  #define X (matrice valida)
  #define I (matrice valida)

  quelli con accanto '*' saranno quelli presi in considerazione
- nel file circ è possibile dare dei nomi alle matrici come per esempio "pippo" (il nome non deve avere le virgolette)
  il numero di caratteri nel nome come il numero di matrici massime inseribili al momento ha un massimo di 64, tuttavia però
  è facilmente modificabile se si apre il file costants.h in header e si modifica il MAX_MATRIX_NAME.
- il primo comando circ valido che trova detterà quali matrici prendere dal file, se una di queste matrici manca il programma
  terminerà con un errore (gestito)
- il comando init deve avere come inizio definizione del vettore il carattere '(',  è possibile inserire degli spazi tra un
  carattere e l'altro, inserire i numeri complessi divisi da ',' quando si ha terminato di inserire i numeri si chiude con ')'
  
2) i numeri complessi devono avere una logica matematica
