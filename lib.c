/**
 * @file lib.c
 * @author FafNir
 * @brief Libreria che implementa il gioco Domino lineare
 * @date 26/01/2024
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdbool.h>

/// @brief Costante per la selezione della modalità interattiva
#define INTERACTIVE_MODE '1'
/// @brief Costante per la selezione della modalità AI
#define AI_MODE '2'
/// @brief Costante per la dimensione della mano del giocatore quando viene generata in modo random
#define HAND_SIZE 100

/**
 * @struct Tile
 * @brief Definisce il tipo Tile: rappresenta una singola tessera di gioco
*/
typedef struct {
  /** Valore sinistro della tessera */
  int left; 
  /** Valore destro della tessera */
  int right;
} Tile;

/**
 * @struct vector
 * @brief Definisce il tipo vector: array dinamico usato per memorizzare un insieme di Tile, per descrivere la mano del giocatore e il campo di gioco
*/
typedef struct {
  /** Dimensione attuale del vector */
  size_t size;
  /** Capacità totale del vector */ 
  size_t capacity;
  /** Array di Tile */ 
  Tile* data;
} vector;

/**
 * @struct m_vector
 * @brief Definisce il tipo m_vector: array dinamico usato per memorizzare le mosse compiute AI
*/
typedef struct {
  /** Dimensione attuale del m_vector */
  size_t size; 
  /** Capacità totale del m_vector */
  size_t capacity;
  /** Array di caratteri */ 
  char* data; 
} m_vector;

// Funzioni per la gestione di vector

/**
 * @brief Funzione per l'allocazione di un nuovo array dinamico di Tiles
 * @return Il nuovo array dinamico allocato
*/
vector* create_vector();


/**
 * @brief Funzione per la deallocazione del vector passato
 * @param v Il vector da deallocare
*/
void free_vector(vector* v);

/**
 * @brief Modifica la capacità del vector fornito
 * @param v Il vector di cui si vuole modificare la capacità
 * @param new_capacity La nuova capacità del vector
*/
void resize(vector* v, size_t new_capacity);

/**
 * @brief Inserisce un elemento in coda al vector v
 * @param v Il vector da modificare
 * @param el L'elemento da inserire in coda a v
*/
void push_back(vector* v, Tile el);

/**
 * @brief Inserisce un elemento in testa al vector v
 * @param v Il vector da modificare
 * @param el L'elemento da inserire in testa a v
*/
void push_front(vector* v, Tile el);

/**
 * @brief Elimina l'elemento del vector v all'indice fornito
 * @param v Il vector da modificare
 * @param index L'indice dell'elemento da eliminare
*/
void delete_at(vector* v, size_t index);

/**
 * @brief Alloca un nuovo vector che è la copia di quello fornito
 * @param v Il vector di cui si vuole creare una copia
 * @return Il nuovo vector copia di v
*/
vector* clone(vector const* v);

/**
 * @brief Copia gli elementi di src in dest sovrascrivendoli
 * @param src Vector di cui si vogliono copiare gli elementi
 * @param dest Vector in cui si vogliono copiare gli elemetni di src
*/
void copy_vector(vector const* src, vector* dest);

// Funzioni per la gestione di m_vector

/**
 * @brief Funzione per la creazione di un nuovo array dinamico di mosse
 * @return Il nuovo m_vector allocato
*/
m_vector* create_m_vector();

/**
 * @brief Funzione per la deallocazione di un m_vector
 * @param v L' m_vector da deallocare
*/
void free_m_vector(m_vector* v);

/**
 * @brief Modifica la capacità del m_vector fornito
 * @param v L' m_vector di cui si vuole modificare la capacità
 * @param new_capacity La nuova capacità dell' m_vector
*/
void resize_m_vector(m_vector* v, size_t new_capacity);

/**
 * @brief Inserisce un elemento in coda al m_vector v
 * @param v L' m_vector da modificare
 * @param el Il Tile da inserire in coda
 * @param c Il carattere della mossa effettuata
*/
void push_back_m_vector(m_vector* v, Tile el, char c);

/**
 * @brief Copia gli elementi di src in dest sovrascrivendoli
 * @param src m_vector di cui si vogliono copiare gli elementi
 * @param dest m_vector in cui si vogliono copiare gli elemetni di src
*/
void copy_m_vector(m_vector const* src, m_vector* dest);

//Funzioni per la gestione delle regole di gioco

/**
 * @brief Funzione che alloca un nuovo vector di Tile casuali
 * @return vector di Tile casuali
*/
vector* generate_random_hand();

/**
 * @brief Funzione che elimina la prima occorrenza (se esistono duplicati) della tessera el dal vector v, se presente
 * @param v vector in cui si vuole eliminare la tessera
 * @param el Tile da eliminare
 * @return true se la tessera è stata trovata ed eliminata, false altrimenti
*/
bool delete_tile(vector* v, Tile el);

/**
 * @brief Funzione che permette di spostare una tessera dalla propria mano nel campo di gioco
 * @param field vector che rappresenta il campo di gioco, dove verrà inserita la tessera
 * @param hand vector che rappresenta la mano del giocatore, da dove verrà eliminata la tessera
 * @param el tessera da spostare
*/
void move_tile(vector* field, vector* hand, char pos, Tile el);

/**
 * @brief Funzione che valuta se la mossa effettuata dal giocatore è valida o meno
 * @param field vector che rappresenta il campo di gioco
 * @param pos mossa effettuata dal giocatore
 * @param el tessera mossa dal giocatore
 * @return true se la mossa è valida, false altrimenti
*/
bool valid_move(vector const* field, char pos, Tile el);

/**
 * @brief Funzione che valuta se esistono mosse valide effettuabili dal giocatore può effettuare ancora mosse valide
 * @param field vector che rappresenta il campo di gioco
 * @param hand vector che rappresenta la mano del giocatore
 * @return true se esistono mosse valide, false altrimenti
*/
bool possible_moves(vector const* field, vector const* hand);

/**
 * @brief Funzione che calcola ricorsivamente il massimo punteggio realizzabile 
 * @param field vector che rappresenta il campo di gioco
 * @param hand vector che rappresenta la mano del giocatore
 * @return Il punteggio massimo calcolato
*/
int recursive_mode(vector* field, vector* hand);

/**
 * @brief Funzione che calcola il punteggio totale del vector fornito
 * @param v vector di cui si vuole calcolare il punteggio
 * @return Il punteggio del vector
*/
int points(vector const* v);

/**
 * @brief Funzione che stampa il campo di gioco
 * @param field vector che rappresenta il campo di gioco
 * @param hand vector che rappresenta la mano del giocatore
*/
void print_field(vector const* field, vector const* hand);

/**
 * @brief Funzione che stampa le mosse effettuate dal giocatore
 * @param moves m_vector che rappresenta le mosse effettuate 
*/
void print_moves(m_vector const* moves);

/**
 * @brief Funzione che stampa la barra di caricamento
 * @param p Valore che rappresenta lo step di iterazione a cui si è arrivati
 * @param width Valore che rappresenta il numero totale di iterazioni
*/
void progress_bar(int p, int width);
