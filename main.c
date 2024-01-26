/**
 * @file main.c
 * @author FafNir
 * @brief Codice che realizza il gioco "Domino Lineare"
 * @date 26/01/2024
 * 
 * @section compilazione Compilazione
 * Per eseguire il programma bisogna:
 *  - compilare il file "main.c" eseguendo il comando "gcc -O2 -std=c99 --pedantic *.c -o main"
 *  - eseguire il file generato dalla compilazione "main"
 */

#include "lib.c"

/*
Funzioni per la gestione di vector
*/
vector* create_vector() {
  vector* v = (vector*) malloc(sizeof(vector));
  if(v == NULL) {
    printf("[+]Error: Memory allocation failed. Exiting program");
    exit(EXIT_FAILURE);
  }

  v->size = 0;
  v->capacity = 4;
  v->data = (Tile*)malloc(sizeof(Tile)*v->capacity);
  if(v->data == NULL) {
    printf("[+]Error: Memory allocation failed. Exiting program");
    exit(EXIT_FAILURE);
  }

  return v;
}

void free_vector(vector* v) {
  free(v->data);
  free(v);
}

void resize(vector* v, size_t new_capacity) {
  Tile* new_data = (Tile*)malloc(sizeof(Tile) * new_capacity);
  if (new_data == NULL) {
    printf("[+]Error: Memory allocation failed. Exiting program");
    exit(EXIT_FAILURE);
  }

  for(size_t i=0; i < v->size; i++) {
    new_data[i].left = v->data[i].left;
    new_data[i].right = v->data[i].right;
  }

  free(v->data);

  v->data = new_data;
  v->capacity = new_capacity;
}

void push_back(vector* v, Tile el) {
  if(v->size == v->capacity)
    resize(v, v->size*2);

  v->data[v->size].left = el.left;
  v->data[v->size].right = el.right;
  v->size += 1;
}

void push_front(vector* v, Tile el) {
  if(v->size == v->capacity) {
    resize(v, v->size*2);
  }

  for(size_t i=v->size; i > 0; i--) {
    v->data[i] = v->data[i-1];
  }

  v->size += 1;
  v->data[0] = el;
}

void delete_at(vector* v, size_t index) {
  if(index >= v->size) {
    printf("[+]Error: index %lu is out of bounds", index);
    exit(EXIT_FAILURE);
  }

  for(size_t i=index; i<v->size; i++) {
    v->data[i] = v->data[i+1];
  }
  v->size -= 1;

  if(v->size <= v->capacity/2) {
    resize(v, v->capacity/2);
  }
}

vector* clone(vector const* v) {
  vector* c = (vector*) malloc(sizeof(vector));
  if(c == NULL) {
    printf("[+]Error: Memory allocation failed. Exiting program");
    exit(EXIT_FAILURE);
  }
  c->size = v->size;
  c->capacity = v->capacity;
  c->data = (Tile*) malloc(sizeof(Tile) * c->capacity);
  if(c->data == NULL) {
    printf("[+]Error: Memory allocation failed. Exiting program");
    exit(EXIT_FAILURE);
  }

  memcpy(c->data, v->data, v->size*sizeof(Tile));

  return c;
}

void copy_vector(vector const* src, vector* dest) {
  if(dest->size < src->size) {
    resize(dest, src->size);
  }

  dest->size = 0;
  for(int i=0; i<src->size; i++) {
    push_back(dest, src->data[i]);
  }
}


/*
Funzioni per la gestione di m_vector
*/

m_vector* create_m_vector() {
  m_vector* v = (m_vector*)malloc(sizeof(m_vector));
  if(v == NULL) {
    printf("[+]Error: Memory allocation failed. Exiting program");
    exit(EXIT_FAILURE);
  }

  v->size = 0;
  v->capacity = 4;
  v->data = (char*)malloc(v->capacity);
  if(v->data == NULL) {
    printf("[+]Error: Memory allocation failed. Exiting program");
    exit(EXIT_FAILURE);
  }

  return v;
}

void free_m_vector(m_vector* v) {
  free(v->data);
  free(v);
}

void resize_m_vector(m_vector* v, size_t new_capacity) {
  char* new_data = (char*)malloc(new_capacity);
  if (new_data == NULL) {
    printf("[+]Error: Memory allocation failed. Exiting program");
    exit(EXIT_FAILURE);
  }

  for(int i=0; i<v->size; i++) {
    new_data[i] = v->data[i];
  }

  free(v->data);

  v->data = new_data;
  v->capacity = new_capacity;
}

/**
 * @brief Aggiunge un carattere in coda al vector fornito
 * @param v L' m_vector in cui inserire il carattere
 * @param c Il carattere da inserire in coda
*/
void _push_back_m_vector(m_vector* v, char c) {
  if(v->size == v->capacity)
    resize_m_vector(v, v->size*2);

  v->data[v->size] = c;
  v->size += 1;
}

void push_back_m_vector(m_vector* v, Tile el, char c) {
  _push_back_m_vector(v, c);
  _push_back_m_vector(v, el.left + '0');
  _push_back_m_vector(v, el.right + '0');
}

void copy_m_vector(m_vector const* src, m_vector* dest) {
  if(dest->size < src->size) {
    resize_m_vector(dest, src->size);
  }

  dest->size = 0;
  for(int i=0; i<src->size; i++) {
    _push_back_m_vector(dest, src->data[i]);
  } 
}


/*
Funzioni per la gestione delle regole di gioco
*/

vector* generate_random_hand() {
  vector* hand = create_vector();
  Tile el;
	char* values[] = {
										"11", "12", "13", "14", "15", "16",
										"21", "22", "23", "24", "25", "26",
										"31", "32", "33", "34", "35", "36",
										"41", "42", "43", "44", "45", "46",
										"51", "52", "53", "54", "55", "56",
										"61", "62", "63", "64", "65", "66",
										"sum", "any", "mirror"
										};

  srand(time(NULL));

  for(int i=0; i<HAND_SIZE; i++) {
    int index = rand()%39;
    int left;
    int right;

    if(strcmp(values[index], "sum") == 0) {
      left = 11;
      right = 11;
    } else if(strcmp(values[index], "any") == 0) {
      left = 0;
      right = 0;
    } else if(strcmp(values[index], "mirror") == 0) {
      left = 12;
      right = 21;
    } else {
      left = values[index][0] - '0';
      right = values[index][1] - '0';
    }

    el.right = right;
    el.left = left;
    push_back(hand, el);
  }

  return hand;
}

bool delete_tile(vector* v, Tile el) {
  int left = el.left;
  int right = el.right;

  for(size_t i=0; i<v->size; i++) {
    if(v->data[i].left == left && v->data[i].right == right) {
      delete_at(v, i);
      return true;
    }
  }

  return false;
}

void move_tile(vector* field, vector* hand, char pos, Tile el) {
  if(valid_move(field, pos, el)) {
    bool present = delete_tile(hand, el);

    if(pos == 'S' && present)
      push_back(field, el);
    
    else if(pos == 'R' && present) {
      if(el.left == 11 && el.right == 11) {
        //Aggiungo +1 in tutto il campo
        for(size_t i=0; i<field->size; i++) {
          field->data[i].left += 1;
          field->data[i].right += 1;
        }

        //imposto la tessera {+1} uguale all'ultima
        el = field->data[field->size-1];
      } else if (el.left == 12 && el.right == 21) {
        //imposto la [12|21] tessera come l'inverso di quella adiacente
        el.right = field->data[field->size-1].left;
        el.left = field->data[field->size-1].right;
      }
      
      push_back(field, el);
    }
    else if(pos == 'L' && present) {
      if(el.left == 11 && el.right == 11) {
        //Aggiungo 1 in tutto il campo
        for(size_t i=0; i<field->size; i++) {
          field->data[i].left += 1;
          field->data[i].right += 1;
        }

        //imposto la tessera {+1} uguale alla prima
        el = field->data[0];
      } else if(el.left == 12 && el.right == 21) {
        el.left = field->data[0].right;
        el.right = field->data[0].left;
      }

      push_front(field, el);
    }
  }
}

bool valid_move(vector const* field, char pos, Tile el) {
  //Se la posizione scelta non è destra o sinistra, la mossa non è valida
  if(pos != 'R' && pos != 'L' && pos != 'S') return false;
  //Se il campo è vuoto e la tessera non è speciale, la mossa è sempre valida
  if(field->size == 0 && el.left != 11 && el.right != 12) return true;
  //Se la tessera è speciale, la mossa è sempre valida
  if((el.left == 0 && el.right == 0) || (el.left == 11 && el.right == 11) || (el.left == 12 && el.right == 21)) return true;

  int last_el = field->size-1;
  if (pos == 'L') {
    //se il primo numero del campo è uguale a left o right della tessera del giocatore
    if( field->data[0].left == el.left ||
        field->data[0].left == el.right ||
        field->data[0].left == 0)
        return true;
  } else if (pos == 'R') {
    //se l'ultimo numero del campo è uguale a left o right della tessera del giocatore
    if( field->data[last_el].right == el.left ||
        field->data[last_el].right == el.right ||
        field->data[last_el].right == 0)
      return true;
  }

  return false;
}

bool possible_moves(vector const* field, vector const* hand) {
  if(field->size == 0) return true;


  bool possible = false;
  int field_ldata = field->data[0].left;
  int field_rdata = field->data[field->size-1].right;
  
  //per ogni tessera in mano al giocatore controllo se la prima o l'ultima tessera del campo sono compatibili
  for(size_t i=0; i<hand->size && !possible; i++) {
    int hand_ldata = hand->data[i].left;
    int hand_rdata = hand->data[i].right;

    if( hand_ldata == field_ldata ||
        hand_rdata == field_ldata ||
        hand_rdata == field_rdata ||
        hand_ldata == field_rdata ||
        field_rdata == 0 || field_ldata == 0 ||
        (hand_ldata == 0 && hand_rdata == 0) ||
        (hand_ldata == 12 && hand_rdata == 21) ||
        (hand_ldata == 11 && hand_rdata == 11))
      possible = true;
  }

  return possible;
}

/**
 * @brief Funzione ausiliaria che calcola ricorsivamente il punteggio massimo data una tessera di partenza
 * @param field vector che rappresenta il campo di gioco
 * @param hand vector che rappresenta la mano del giocatore
 * @param max_moves m_vector che rappresenta le mosse effettuate per avere il massimo punteggio
 * @param el tessera di partenza di cui calcolare il punteggio massimo
 * @return Il numero di punti massimo effettuabile data la tessera iniziale
*/
int recursive_mode_aux(vector* field, vector* hand, m_vector* max_moves ,char pos, Tile el) {
  if(!possible_moves(field, hand))
    return points(field);

  if(valid_move(field, pos, el)) {
    move_tile(field, hand, pos, el);
    push_back_m_vector(max_moves, el, pos);
  }
  else return 0;

  int max_points = 0;

  for(size_t i=0; i < hand->size; i++) {

    int this_max_points_r = recursive_mode_aux(field, hand, max_moves, 'R', hand->data[i]);
    int this_max_points_l = recursive_mode_aux(field, hand, max_moves, 'L', hand->data[i]);

    if(this_max_points_r > this_max_points_l)
      max_points = this_max_points_r;
    else
      max_points = this_max_points_l;

  }

  return max_points;
}

int recursive_mode(vector* field, vector* hand) {
  int max = 0;
  
  //creazione di vettori ausiliari
  vector* mem_field = clone(field);
  vector* mem_hand = clone(hand);
  vector* max_field = create_vector();
  vector* max_hand = create_vector();
  m_vector* max_moves = create_m_vector();
  

  for(size_t i=0; i<hand->size; i++) {
    m_vector* this_max_moves = create_m_vector();
    progress_bar(i, hand->size);
    int this_max = recursive_mode_aux(field, hand, this_max_moves, 'S', hand->data[i]);
    
    if(max < this_max) {
      max = this_max;
      copy_vector(field, max_field);
      copy_vector(hand, max_hand);
      copy_m_vector(this_max_moves, max_moves);
    }

    //Imposta il campo e la mano allo stato iniziale
    copy_vector(mem_field, field);
    copy_vector(mem_hand, hand);
    free_m_vector(this_max_moves);
  }

  print_field(max_field, max_hand);
  print_moves(max_moves);
  free_vector(mem_field);
  free_vector(mem_hand);
  free_vector(max_field);
  free_vector(max_hand);
  free_m_vector(max_moves);
  
  return max;
}

int points(vector const* v) {
  int sum = 0;

  for(size_t i=0; i<v->size; i++)
    sum = sum + v->data[i].left + v->data[i].right;

  return sum;
}

void progress_bar(int p, int width) {
  printf("\033[1;1H\033[2J");
  printf("\n");

  int progress = (int)((float)p/width * 100);

  for(int i=1; i<100; i++) {
    if(i < progress) printf("%c", '|');
    else printf("%c", '_');
  }
  printf("\nLoading: %d%% \n", progress);
}

void print_field(vector const* field, vector const* hand) {
  printf("\033[1;1H\033[2J");
  printf("Field: ");
  for(size_t i=0; i<field->size; i++) {
    printf("[%d|%d]", field->data[i].left, field->data[i].right);
  }

  printf("\n");

  printf("Hand: ");
  for(size_t i=0; i<hand->size; i++) {
    printf("[%d|%d]", hand->data[i].left, hand->data[i].right);
  }

  printf("\n");
}

void print_moves(m_vector const* moves) {
  printf("Moves: ");
  for(int i=0; i<moves->size; i++) {    
    if(moves->data[i] == 59) {
      printf("11 ");
    }
    else if (moves->data[i] == 60) {
      printf("12 ");
    } else if (moves->data[i] == 69) {
      printf("21 ");
      } else {
      printf("%c ", moves->data[i]);
    }
  }
}

int main() {
  char in;
  vector* player_hand = create_vector();


	printf("Any input tiles? y/n\n");
	scanf(" %c", &in);

  if (in == 'y'|| in == 'Y') {
		int input_size;
		scanf(" %d", &input_size);

		//Lettura delle tessere da stdin
		for (int i = 0; i < input_size; i++) {
			Tile el;
      int left, right;
			scanf("%d %d", &left, &right);
      el.left = left;
      el.right = right;  
      push_back(player_hand, el);
    }
	} 
  else if (in == 'n' || in == 'N') {
    free_vector(player_hand);
    player_hand = generate_random_hand();
  }

  printf("Mode selection: \n1. Interactive\n2. AI\n");
	scanf(" %c", &in);

  vector* field = create_vector();
  if(in == INTERACTIVE_MODE) {
		int left;
		int right;
		char pos;

		//main game loop
		while(possible_moves(field, player_hand)) {
			print_field(field, player_hand);
			printf("\nPlayer move: ");
			scanf(" %c %d %d", &pos, &left, &right);
      
      //creazione della Tile in base all'input del giocatore
      Tile el;
      el.left = left;
      el.right = right;

			move_tile(field, player_hand, pos, el);
		}

    print_field(field, player_hand);
		printf("Points: %d", points(field));
	} else if (in == AI_MODE) {
    printf("\nPoints: %d\n", recursive_mode(field, player_hand));
	}

	free_vector(player_hand);
	free_vector(field);
	return 0;
}